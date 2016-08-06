<?php

/**
 * Defines how a registration is submitted to CiviCRM
 */

include_once('webform_integration.inc');



/**
 * AJAX call handler for 'discount code field changed'
 */
function ica_event_submission($form_values) {
  error_log(print_r($form_values,1));

  // TODO: calculate the registration fee
  $registration_fees = ica_event_calculate_registration_fee($form, $form_state);

  // compile registration data from the form_values
  $submission = array();
  $submission['submission_date']         = date('YmdHis');
  $submission['registration_id']         = "TODO-" . rand();
  $submission['event_id']                = 1; // TODO
  $submission['additional_participants'] = array();

  // process main participant (+partner)
  $main_participant = ica_event_submission_extract_participant($form_values, ICA_REGISTRANT_PREFIX);
  $main_participant['participant_status']
  $main_participant['participant_role']
  $main_participant['participant_fee_amount']
  $main_participant['participant_fee_currency']
  $main_participant['participant_fee_level']
  $submission['participant'] = 
  if (!empty($form_values[ICA_EVENT_SPOUSE])) {
    $partner = ica_event_submission_extract_participant($form_values, ICA_PARTNER_PREFIX);
    $partner['participant_status']
    $partner['participant_role']
    $partner['participant_fee_amount']
    $partner['participant_fee_currency']
    $partner['participant_fee_level']
    $submission['additional_participants'][] = $partner;
  }

  // process additional participants (+partner)
  if 




  // return civicrm_api3_create_success(civicrm_api3('Registration', 'create', array(
  //   'submission_date'  => date('YmdHis'),
  //   'registration_id'  => 'TEST-' . rand(),
  //   'event_id'         => 1,
  //   'participant' => array(
  //       'first_name'               => 'Harold',
  //       'last_name'                => 'Testbloke',
  //       'email'                    => 'harold@testblo.ke',
  //       'contact_type'             => 'Individual',
  //       'participant_status'       => 'Registered',
  //       'participant_role'         => 'Attendee',
  //       'participant_fee_amount'   => '100',
  //       'participant_fee_currency' => 'EUR',
  //       'participant_fee_level'    => 'International',
  //     ),
  //   'additional_participants' => array(
  //     0 => array(
  //         'first_name'               => 'George',
  //         'last_name'                => 'Testbloke',
  //         'email'                    => 'george@testblo.ke',
  //         'contact_type'             => 'Individual',
  //         'participant_status'       => 'Registered',
  //         'participant_role'         => 'Attendee',
  //         'participant_fee_amount'   => '200',
  //         'participant_fee_currency' => 'EUR',
  //         'participant_fee_level'    => 'Youth',
  //       ),
  //     1 => array(
  //         'first_name'               => 'Sarah',
  //         'last_name'                => 'Testblokette',
  //         'email'                    => 'sarah@testbloke.te',
  //         'contact_type'             => 'Individual',
  //         'participant_status'       => 'Registered',
  //         'participant_role'         => 'Partner',
  //         'participant_fee_amount'   => '400',
  //         'participant_fee_currency' => 'EUR',
  //         'participant_fee_level'    => 'Spouse',
  //       ),
  //   ))));
  // TODO: values not directly accessible in $form_values. Maybe we have to load 
  //   them from the DB via the 'node'.



  //  1) call XCM to get contact ID
  // $main_contact = ica_event_cmrf_connector_sendCall('Contact', 'getorcreate', 
  //                         array(
  //                           'contact_type' => 'Individual',
  //                           'first_name'   => '',
  //                         ),
  //                         array());


  //  2) create all participation objects / payment information

  // TODO: use API chaining for performance
}

function ica_event_submission_extract_participant($form_values, $prefix) {

}

