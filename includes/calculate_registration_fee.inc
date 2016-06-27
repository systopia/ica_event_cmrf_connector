<?php

/**
 * This file deals with the calculation of the registration fee
 */

include_once('webform_integration.inc');


define('ICA_EVENT_FEE_PARTICIPANT',   950.00);
define('ICA_EVENT_FEE_MEMBER_INT',    750.00);
define('ICA_EVENT_FEE_MEMBER_LOCAL',  250.00);
define('ICA_EVENT_FEE_YOUTH',         200.00);
define('ICA_EVENT_FEE_SPOUSE',        100.00);


/**
 * This is the algorithm to calculate the registration fee for
 * a given set of registration data
 *
 * @return array: 'amount'       => total_amount
 *                'errors'       => array (problems)
 *                'explanations' => array (reasons)
 */
function ica_event_calculate_registration_fee(&$form, &$form_state) {
  $registered_persons = array(
    'participant'  => 0,
    'member_int'   => 0,
    'member_local' => 0,
    'youth'        => 0,
    'spouse'       => 0
    );

  // get some basic information
  $current_values     = ica_event_get_form_values($form, $form_state);
  $organisation_id    = ica_event_get_form_value(ICA_MEMBER_ORGANISATION, $current_values);
  $local_organisation = ica_event_is_local_member_org($organisation_id);

  // first, process the main contact
  $main_contact_attending = ica_event_get_form_value(ICA_REGISTRANT_ATTENDING, $current_values);
  if ($main_contact_attending) {
    $main_contact_youth = ica_event_get_form_value(ICA_EVENT_YOUTH, $current_values);
    if (ica_event_is_youth($main_contact_youth)) {
      // YOUTH
      $registered_persons['youth'] += 1;

    } elseif ($organisation_id) {
      // MEMBER...
      if ($local_organisation) {
        // LOCAL MEMBER
        $registered_persons['member_local'] += 1;
      } else {
        // INTERNATIONAL MEMBER
        $registered_persons['member_int'] += 1;
      }
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
  $matrix = ica_event_get_form_value(ICA_EVENT_MATRIX, $current_values);
  if ($matrix) {
    foreach ($matrix as $row_nr => $values) {
      if (!empty($values[ICA_EVENT_MATRIX_YOUTH_IDX])) {
        // YOUTH
        $registered_persons['youth'] += 1;
      } elseif ($organisation_id) {
        // MEMBER...
        if ($local_organisation) {
          // LOCAL MEMBER
          $registered_persons['member_local'] += 1;
        } else {
          // INTERNATIONAL MEMBER
          $registered_persons['member_int'] += 1;
        }
      } else {
        // PARTICIPANT
        $registered_persons['participant'] += 1;
      }

      if (!empty($values[ICA_EVENT_MATRIX_SPOUSE_IDX])) {
        $registered_persons['spouse'] += 1;
      }
    }
  }

  $registered_persons['amount'] =
      $registered_persons['participant']  * ICA_EVENT_FEE_PARTICIPANT
    + $registered_persons['member_int']   * ICA_EVENT_FEE_MEMBER_INT
    + $registered_persons['member_local'] * ICA_EVENT_FEE_MEMBER_LOCAL
    + $registered_persons['youth']        * ICA_EVENT_FEE_YOUTH
    + $registered_persons['spouse']       * ICA_EVENT_FEE_SPOUSE;

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
