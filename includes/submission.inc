<?php
/*-------------------------------------------------------+
| ICA Event Registration Module                          |
| Copyright (C) 2016 SYSTOPIA                            |
+--------------------------------------------------------+
| This program is released as free software under the    |
| Affero GPL license. You can redistribute it and/or     |
| modify it under the terms of this license which you    |
| can read by viewing the included agpl.txt or online    |
| at www.gnu.org/licenses/agpl.html. Removal of this     |
| copyright header is strictly prohibited without        |
| written permission from the original author(s).        |
+--------------------------------------------------------*/

/**
 * Defines how a registration is submitted to CiviCRM
 */

include_once('calculate_registrations.inc');


/**
 * AJAX call handler for 'discount code field changed'
 */
function ica_event_process_submission($node, $submission_data) {
  // we don't do anything until it's actually submitted
  if (empty($submission_data['completed'])) return;

  // calculate the registration info
  $registrations = ica_event_calculate_registrations_with_submission($node, $submission_data);

  // compile registration data from the form_values
  $submission = array();
  $submission['submission_date']         = date('YmdHis');
  $submission['registration_id']         = ICA_EVENT_SUBMISSION_PREFIX . "-{$submission_data->nid}-{$submission_data->sid}";
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
  error_log("SUBMISSION: " . json_encode($submission));
  // $submission_policy = array();
  // ica_event_cmrf_connector_sendCall('Registration', 'create', $submission);
}

