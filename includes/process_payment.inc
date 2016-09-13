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
 * This file deals with completed payment process events
 */
include_once('webform_integration.inc');

/**
 * send payment completion to CiviCRM
 */
function ica_event_process_payment($order) {
  // Compile payment data
  // TODO: trxn_id? 
  $payment_info = array(
    'registration_id'  => $order->order_number,
    'timestamp'        => date('YmdHis'),
    'status'           => 'Completed',
    // 'trxn_id'          => rand(),
    );

  // Transmit to CiviCRM
  $transmission_policy = array(
    'retry' => 1
  );
  ica_event_cmrf_connector_sendCall('Registration', 'payment', $payment_info, $transmission_policy);
}

