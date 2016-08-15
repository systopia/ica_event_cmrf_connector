<?php

/**
 * This file extracts the registrations from the give webform data
 */

include_once('webform_integration.inc');


/**
 * This is the algorithm to calculate the registration fee for
 * a given set of registration data
 *
 * @return array: registration data
 */
function ica_event_calculate_registrations(&$form, &$form_state) {
  // FIRST DEFINE SOME BASIC CLASSES / FIELDS
  $classes = array(
    'participant' => array( 'participant_fee_amount'   => ICA_EVENT_FEE_PARTICIPANT,
                            'participant_fee_currency' => ICA_DEFAULT_CURRENCY,
                            '_class'                   => 'participant',
                            'contact_type'             => 'Individual',
                            'participant_status'       => 'Registered',
                            'participant_role'         => 'Participant',
                            'participant_fee_level'    => 'Participant'),
    'member_int'  => array( 'participant_fee_amount'   => ICA_EVENT_FEE_MEMBER_INT,
                            '_class'                   => 'member_int',
                            'participant_fee_currency' => ICA_DEFAULT_CURRENCY,
                            'contact_type'             => 'Individual',
                            'participant_status'       => 'Registered',
                            'participant_role'         => 'Participant',
                            'participant_fee_level'    => 'International Member'),
    'local'       => array( 'participant_fee_amount'   => ICA_EVENT_FEE_LOCAL,
                            'participant_fee_currency' => ICA_DEFAULT_CURRENCY,
                            '_class'                   => 'local',
                            'contact_type'             => 'Individual',
                            'participant_status'       => 'Registered',
                            'participant_role'         => 'Participant',
                            'participant_fee_level'    => 'Local Member'),
    'youth'       => array( 'participant_fee_amount'   => ICA_EVENT_FEE_YOUTH,
                            'participant_fee_currency' => ICA_DEFAULT_CURRENCY,
                            '_class'                   => 'youth',
                            'contact_type'             => 'Individual',
                            'participant_status'       => 'Registered',
                            'participant_role'         => 'Participant',
                            'participant_fee_level'    => 'Youth'),
    'partner'     => array( 'participant_fee_amount'   => ICA_EVENT_FEE_PARTNER,
                            'participant_fee_currency' => ICA_DEFAULT_CURRENCY,
                            '_class'                   => 'partner',
                            'contact_type'             => 'Individual',
                            'participant_status'       => 'Registered',
                            'participant_role'         => 'Partner',
                            'participant_fee_level'    => 'Partner'),
    'organisation'=> array( '_class'                   => 'organisation',
                            'contact_type'             => 'Organization',
                            ),
    );
  
  // get some values
  $registrations      = array();
  $current_values     = ica_event_get_form_values($form, $form_state);
  $organisation_id    = ica_event_get_value(ICA_MEMBER_ORGANISATION, $current_values);
  $registrant_country = ica_event_get_value(ICA_REGISTRANT_COUNTRY, $current_values);
  
  
  // FIRST: create organisation
  global $ica_event_registration_properties;
  $organisation = $classes['organisation'];
  ica_event_copy_fields($organisation, $current_values, $ica_event_registration_properties, ICA_ORGANISATION_PREFIX);
  $registrations['organisation'] = $organisation;


  // THEN: create main contact
  $main_contact_youth = ica_event_get_value(ICA_EVENT_BIRTH_DATE, $current_values);
  if (ica_event_is_youth($main_contact_youth)) {
    // YOUNG PARTICIPANT
    $class = 'youth';

  } elseif ($registrant_country == ICA_REGISTRANT_COUNTRY_LOCAL) {
    // LOCAL PARTICIPANT
    $class = 'local';

  } elseif ($organisation_id) {
    // INTERNATIONAL MEMBER
    $class = 'member_int';

  } else {
    // PARTICIPANT
    $class = 'participant';
  }
  $main_registration = $classes[$class]; 
  ica_event_copy_fields($main_registration, $current_values, $ica_event_registration_properties, ICA_REGISTRANT_PREFIX);

  // add the partner
  $main_contact_has_partner = ica_event_get_value(ICA_EVENT_FEE_PARTNER, $current_values);
  if ($main_contact_has_partner) {
    $partner_registration = $classes['partner'];
    $partner_registration['participant_role'] = 'Partner/Spouse';
    $partner_registration['partner_of'] = 'main';
    ica_event_copy_fields($partner_registration, $current_values, $ica_event_registration_properties, ICA_PARTNER_PREFIX);
    $registrations['main_partner'] = $partner_registration;
  }

  $main_contact_attending = ica_event_get_value(ICA_REGISTRANT_ATTENDING, $current_values);
  if ($main_contact_attending) {
    // main contact attending
    $main_registration['participant_role'] = 'Main Contact,Participant';

  } else {
    // Main contact NOT attending
    $main_registration['participant_role'] = 'Main Contact';
  }
  $registrations['main'] = $main_registration;



  // THEN: process all people in the matrix;
  global $ica_event_matrix_fields_group_registration;
  $matrix_serialised = ica_event_get_value(ICA_EVENT_MATRIX, $current_values);
  $matrix_data = ica_event_get_matrix_values($matrix_serialised, $ica_event_matrix_fields_group_registration);
  
  if ($matrix_data) {
    foreach ($matrix_data as $row_nr => $values) {
      $registrant_key = "additional_{$row_nr}";
      if (!empty($values[ICA_EVENT_YOUTH])) {
        // YOUNG PARTICIPANT
        $class = 'youth';

      } elseif ($registrant_country == ICA_REGISTRANT_COUNTRY_LOCAL) {
        // LOCAL PARTICIPANT
        $class = 'local';

      } elseif ($organisation_id) {
        // INTERNATIONAL MEMBER
        $class = 'member_int';

      } else {
        // PARTICIPANT
        $class = 'participant';
      }

      $registration = $classes[$class];
      ica_event_copy_fields($registration, $values, $ica_event_registration_properties, ICA_REGISTRANT_PREFIX);
      $registrations[$registrant_key] = $registration;

      if (!empty($values[ICA_EVENT_PARTNER])) {
        $partner_key = "additional_{$row_nr}_partner";
        $partner_registration = $classes['partner'];
        $partner_registration['partner_of'] = $registrant_key;
        ica_event_copy_fields($partner_registration, $values, $ica_event_registration_properties, ICA_PARTNER_PREFIX);
        $registrations[$partner_key] = $partner_registration;
      }
    }
  }

  error_log(json_encode($registrations));
  return $registrations;
}


/**
 * copy a set of fields into the registration data (first argument)
 */
function ica_event_copy_fields(&$registration, &$values, $fields, $prefix = '') {
  foreach ($fields as $data_key => $registration_key) {
    $data_key = $prefix . $data_key;
    if (isset($values[$data_key])) {
      $registration[$registration_key] = $values[$data_key];
    } else {
      $registration[$registration_key] = '';
    }
  }
}


/**
 * safe way to get a value from the variables
 */
function ica_event_get_value($key, &$variables) {
  if (isset($variables[$key])) {
    return $variables[$key];
  } else {
    return NULL;
  }
}


function ica_event_is_local_member_org($organisation_id) {
  // TODO: implement
  return FALSE;
}

/**
 * check whether the given birth date will grant 'youth' status
 */
function ica_event_is_youth($date) {
  if (is_array($date)) {
    if (empty($date['year']) || empty($date['month']) || empty($date['day'])) {
      return FALSE;
    } else {
      $date = sprintf("%04d-%02d-%02d", $date['year'], $date['month'], $date['day']);
    }
  }

  return $date >= ICA_EVENT_YOUTH_MAX_BIRTHDAY;
}