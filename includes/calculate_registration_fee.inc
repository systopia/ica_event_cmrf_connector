<?php

/**
 * This file deals with the calculation of the registration fee
 */

include_once('webform_integration.inc');


/**
 * This is the algorithm to calculate the registration fee for
 * a given set of registration data
 *
 * @return array: 'amount'       => total_amount
 *                'errors'       => array (problems)
 *                'explanations' => array (reasons)
 */
function ica_event_calculate_registration_fee(&$form, &$form_state) {
  // list the registration classes
  $classes = array(
    'participant' => array( 'amount' => 

      ))

  // start compiling the overview
  $registered_persons = array(
    'participant'  => 0,
    'member_int'   => 0,
    'local'        => 0,
    'youth'        => 0,
    'spouse'       => 0,
    );

  // add a mapping of person reference to info
  //  ()
  $registered_persons['mapping'] = array();
      '_mapping'     => array(),


  // get some basic information
  $current_values     = ica_event_get_form_values($form, $form_state);
  $organisation_id    = ica_event_get_form_value(ICA_MEMBER_ORGANISATION, $current_values);
  $registrant_country = ica_event_get_form_value(ICA_REGISTRANT_COUNTRY, $current_values);

  // first, process the main contact
  $main_contact_attending = ica_event_get_form_value(ICA_REGISTRANT_ATTENDING, $current_values);
  if ($main_contact_attending) {

    $main_contact_youth = ica_event_get_form_value(ICA_EVENT_YOUTH, $current_values);
    if (ica_event_is_youth($main_contact_youth)) {
      // YOUNG PARTICIPANT
      $registered_persons['youth'] += 1;
      $registered_persons['youth'] += 1;

    } elseif ($registrant_country == ICA_REGISTRANT_COUNTRY_LOCAL) {
      // LOCAL PARTICIPANT
      $registered_persons['local'] += 1;

    } elseif ($organisation_id) {
      // INTERNATIONAL MEMBER
      $registered_persons['member_int'] += 1;

    } else {
      // PARTICIPANT
      $registered_persons['participant'] += 1;
    }

    // add the spouse
    $main_contact_has_spouse = ica_event_get_form_value(ICA_EVENT_FEE_SPOUSE, $current_values);
    if ($main_contact_has_spouse) {
      $registered_persons['spouse'] += 1;
    }
  }

  // THEN: process all people in the matrix;
  $matrix_serialised = ica_event_get_form_value(ICA_EVENT_MATRIX, $current_values);
  if ($matrix_serialised) {
    // sometimes it's an array - sometimes not :/
    if (!is_array($matrix_serialised)) {
      $matrix_serialised = array(0 => $matrix_serialised);
    }
    
    $matrix = array();
    foreach ($matrix_serialised as $matrix_block) {
      $matrix += unserialize($matrix_block);
    }

    foreach ($matrix as $row_nr => $values) {
      if (!empty($values[ICA_EVENT_MATRIX_YOUTH_IDX])) {
        // YOUNG PARTICIPANT
        $registered_persons['youth'] += 1;

      } elseif ($registrant_country == ICA_REGISTRANT_COUNTRY_LOCAL) {
        // LOCAL PARTICIPANT
        $registered_persons['local'] += 1;

      } elseif ($organisation_id) {
        // INTERNATIONAL MEMBER
        $registered_persons['member_int'] += 1;

      } else {
        // PARTICIPANT
        $registered_persons['participant'] += 1;
      }

      if (!empty($values[ICA_EVENT_MATRIX_SPOUSE_IDX])) {
        $registered_persons['spouse'] += 1;
      }
    }
  }

  $registered_persons['fees'] = array(
    'participant' => ICA_EVENT_FEE_PARTICIPANT,
    'member_int'  => ICA_EVENT_FEE_MEMBER_INT,
    'local'       => ICA_EVENT_FEE_LOCAL,
    'youth'       => ICA_EVENT_FEE_YOUTH,
    'spouse'      => ICA_EVENT_FEE_SPOUSE,
    );

  $registered_persons['amount'] =
      $registered_persons['participant']  * ICA_EVENT_FEE_PARTICIPANT
    + $registered_persons['member_int']   * ICA_EVENT_FEE_MEMBER_INT
    + $registered_persons['local']        * ICA_EVENT_FEE_LOCAL
    + $registered_persons['youth']        * ICA_EVENT_FEE_YOUTH
    + $registered_persons['spouse']       * ICA_EVENT_FEE_SPOUSE;

  // error_log(json_encode($registered_persons));
  return $registered_persons;
}


function ica_event_get_form_value($key, &$variables) {
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
