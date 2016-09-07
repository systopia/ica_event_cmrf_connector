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
    'participant' => ica_event_get_registration_fee('participant'),
    'member_int'  => ica_event_get_registration_fee('member_int'),
    'local'       => ica_event_get_registration_fee('local'),
    'youth'       => ica_event_get_registration_fee('youth'),
    'partner'     => ica_event_get_registration_fee('partner'),
    );

  // count and calculate the registrations
  foreach ($registrations as $registration) {
    if (isset($registration_info[$registration['_class']])) {
      $registration_info[$registration['_class']] += 1;
    }
  }

  $registration_info['amount'] =
      $registration_info['participant']  * $registration_info['participant']
    + $registration_info['member_int']   * $registration_info['member_int']
    + $registration_info['local']        * $registration_info['local']
    + $registration_info['youth']        * $registration_info['youth']
    + $registration_info['partner']      * $registration_info['partner'];

  // error_log(json_encode($registration_info));
  return $registration_info;
}
