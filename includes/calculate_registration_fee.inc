<?php

/**
 * This file deals with the calculation of the registration fee
 */

include_once('webform_integration.inc');
include_once('calculate_registrations.inc');

/**
 * This is the algorithm to calculate the registration fee for
 * a given set of registration data
 *
 * @return array: 'amount'       => total_amount
 *                'errors'       => array (problems)
 *                'explanations' => array (reasons)
 */
function ica_event_calculate_registration_fee(&$form, &$form_state) {
  $registrations = ica_event_calculate_registrations_with_form_data($form, $form_state);

  // start compiling the overview
  $registration_info = array(
    'participant'   => 0,
    'member_int'    => 0,
    'local'         => 0,
    'youth'         => 0,
    'partner'       => 0,
    );

  $registration_info['fees'] = array(
    'participant' => ICA_EVENT_FEE_PARTICIPANT,
    'member_int'  => ICA_EVENT_FEE_MEMBER_INT,
    'local'       => ICA_EVENT_FEE_LOCAL,
    'youth'       => ICA_EVENT_FEE_YOUTH,
    'partner'     => ICA_EVENT_FEE_PARTNER,
    );

  // count and calculate the registrations
  foreach ($registrations as $registration) {
    if (isset($registration_info[$registration['_class']])) {
      $registration_info[$registration['_class']] += 1;
    }
  }

  $registration_info['amount'] =
      $registration_info['participant']  * ICA_EVENT_FEE_PARTICIPANT
    + $registration_info['member_int']   * ICA_EVENT_FEE_MEMBER_INT
    + $registration_info['local']        * ICA_EVENT_FEE_LOCAL
    + $registration_info['youth']        * ICA_EVENT_FEE_YOUTH
    + $registration_info['partner']      * ICA_EVENT_FEE_PARTNER;

  // error_log(json_encode($registration_info));
  return $registration_info;
}
