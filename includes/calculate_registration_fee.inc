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
function ica_event_calculate_registration_fee(&$form, $form_state) {
  $fee = array(
    'amount'       => 9001.00,
    'errors'       => array(),
    'explanations' => array());

  return $fee;
}
