<?php

/**
 * Defines how a registration is submitted to CiviCRM
 */

include_once('calculate_registrations.inc');



/**
 * AJAX call handler for 'discount code field changed'
 */
function ica_event_submission(&$form, &$form_state) {

  // TODO: calculate the registration fee
  $registrations = ica_event_calculate_registrations($form, $form_state);

  // compile registration data from the form_values
  $submission = array();
  $submission['submission_date']         = date('YmdHis');
  $submission['registration_id']         = "TODO-" . $form['#node']->id . '-' . rand();
  $submission['event_id']                = ICA_CIVICRM_EVENT_ID;
  $submission['additional_participants'] = array();

  foreach ($registrations as $registration_key => $registration) {
    // remove unecessary stuff
    if (isset($registration['_class'])) unset($registration['_class']);

    // add to lists
    if ($registration_key == 'main') {
      $submission['participant'] = $registration;
    } elseif ($registration_key == 'organisation') {
      $submission['organisation'] = $registration;
    } else {
      $submission['additional_participants'][] = $registration;
    }
  }

  // SUBMIT
  error_log(json_encode($submission));
  // $submission_policy = array();
  // ica_event_cmrf_connector_sendCall('Registration', 'create', $submission);
}
