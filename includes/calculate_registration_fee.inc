<?php

/**
 * @file
 * Install and uninstall code for Custom Module.
 *
 * Long description here.
 */

/**
 * This is the algorithm to calculate the registration fee for
 * a given set of registration data
 *
 * @return array: 'amount'       => total_amount
 *                'errors'       => array (problems)
 *                'explanations' => array (reasons)
 */
function ica_event_caclulate_registration_fee(&$form) {
  $fee = array(
    'amount'       => 1000.0,
    'errors'       => array(),
    'explanations' => array());

  // test:
  error_log("NOW");

  return $fee;
}


function ica_event_caclulate_registration_fee_ajax_callback($form, $form_state) {
  $fee = ica_event_caclulate_registration_fee($form);

  $commands = array();
  $commands[] = ajax_command_replace('#edit-submitted-amount', $fee['amount']);
  ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
  exit;
}

