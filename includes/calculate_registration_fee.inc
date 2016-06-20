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
function ica_event_calculate_registration_fee(&$form, $form_state) {
  $registered_persons = array(
    'participant'  => 0,
    'member_int'   => 0,
    'member_local' => 0,
    'youth'        => 0,
    'spouse'       => 0
    );

  // get some basic information
  $organisation_id    = ica_event_get_form_value(ICA_MEMBER_ORGANISATION, $form, $form_state);
  $local_organisation = ica_event_is_local_member_org($organisation_id);

  // first, process the main contact
  $main_contact_youth = ica_event_get_form_value(ICA_EVENT_YOUTH, $form, $form_state);
  if ($main_contact_youth) {
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
  $main_contact_has_spouse = ica_event_get_form_value(ICA_EVENT_FEE_SPOUSE, $form, $form_state);
  if ($main_contact_has_spouse) {
    $registered_persons['spouse'] += 1;
  }

  // THEN: process all people in the matrix;
  $matrix = ica_event_get_form_value(ICA_EVENT_MATRIX, $form, $form_state);
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


function ica_event_get_form_value($variable, &$form, &$form_state) {
  if (isset($form_state['input']['submitted'][$variable])) {
    return $form_state['input']['submitted'][$variable];
  } else {
    // TODO: look at $form['submitted'][$variable]['#value']?
    return NULL;
  }
}


function ica_event_is_local_member_org($organisation_id) {
  // TODO: implement
  return FALSE;
}
