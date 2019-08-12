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
  if (!empty($submission_data->is_draft)) return;
  if (empty($submission_data->completed)) return;

  if ($submission_data->completed != $submission_data->modified) {
    // TODO: we don't process updates/edits
    return;
  }

  global $user;
  global $language;

  // calculate the registration info
  $submitted_values = array();
  $registrations = ica_event_calculate_registrations_with_submission($node, $submission_data, $submitted_values);

  // compile registration data from the form_values
  $submission = array();
  $submission['submission_date']         = date('YmdHis');
  $submission['registration_id']         = ICA_EVENT_SUBMISSION_PREFIX . "-{$submission_data->nid}-{$submission_data->sid}";
  $submission['registration_language']   = $language->name;
  //$submission['event_id']                = ICA_CIVICRM_EVENT_ID; // not needed, default is set by receiver
  $submission['payment_mode']            = $submitted_values[ICA_PAYMENT_MODE];
  $submission['additional_participants'] = array();

  // add version info
  $info_file   = drupal_get_path('module', 'ica_event_cmrf_connector') . '/ica_event_cmrf_connector.info';
  $module_info = drupal_parse_info_file($info_file);
  $submission['created_version'] = $module_info['version'];

  foreach ($registrations as $registration_key => $registration) {
    // remove unecessary stuff
    if (isset($registration['_class'])) unset($registration['_class']);

    // set they key
    $registration['participant_key'] = $registration_key;

    // add to lists
    if ($registration_key == 'main') {
      $submission['participant'] = $registration;
    } elseif ($registration_key == 'organisation') {
      $submission['organisation'] = $registration;
    } else {
      $submission['additional_participants'][] = $registration;
    }
  }

  // SUBMIT TO CIVICRM
  // error_log("SUBMISSION: " . json_encode($submission));
  $submission_policy = array(
    'retry' => 1
  );
  ica_event_cmrf_connector_sendCall('Registration', 'create', $submission, $submission_policy);



  // NEXT: CREATE an order in Commerce
  if (function_exists('commerce_order_new')) {
    // TODO: get all products/quantities (array: product_id => count)
    $registration_products = array();
    foreach ($registrations as $registration_key => $registration) {
      if ($registration['_class'] == 'organisation') continue;
      $sku = ICA_EVENT_SUBMISSION_PREFIX . '-' . $registration['_class'];
      $product = commerce_product_load_by_sku($sku);
      if ($product) {
        // watchdog(WATCHDOG_DEBUG, "Product found!");
        if (!isset($registration_products[$product->product_id])) {
          $registration_products[$product->product_id] = 1;
        } else {
          $registration_products[$product->product_id] += 1;
        }
      } else {
        watchdog(WATCHDOG_WARNING, "Product NOT found!");
      }

    }
    // watchdog(WATCHDOG_DEBUG, json_encode($registration_products));

    // now create the order
    if (!empty($registration_products)) {
      // watchdog(WATCHDOG_DEBUG, "Creating order {$submission['registration_id']}...");
      
      // create a new order
      $order = commerce_order_new($user->uid, 'checkout_checkout');
      $order->order_number = $submission['registration_id'];
      commerce_order_save($order);
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

      foreach ($registration_products as $product_id => $quantity) {
        $product = commerce_product_load($product_id);
        $line_item = commerce_product_line_item_new($product, $quantity, $order->order_id);
        commerce_line_item_save($line_item);

        $order_wrapper->commerce_line_items[] = $line_item;
      }

      // Add a customer profile reference.
      $customer_billing = commerce_customer_profile_new('billing', $order->id);
      // TODO: Add commerce_customer_address field to billing object.
      $customer_billing->commerce_customer_address = array($language->language => array(
        0 => array(
          'country' => '',
          'name_line' => '',
          'first_name' => '',
          'last_name' => '',
          'organisation_name' => '',
          'administrative_area' => '',
          'sub_administrative_area' => '',
          'locality' => '',
          'dependent_locality' => '',
          'postal_code' => '',
          'thoroughfare' => '',
          'premise' => '',
          'sub_premise' => '',
          'data' => '',
        )));
      commerce_customer_profile_save($customer_billing);
      $order->commerce_customer_billing[$language->language][0]['profile_id'] = $customer_billing->profile_id;
      
      commerce_order_save($order);
      // commerce_cart_order_session_save($order->order_id);
      // watchdog(WATCHDOG_INFO, 'Order created: checkout/' . $order->order_id);

      if ($submitted_values[ICA_PAYMENT_MODE] == ICA_PAYMENT_MODE_ONLINE) {  
        // redirect to payment page if online payment selected
        drupal_goto('checkout/' . $order->order_id);
      }
    }
  } else {
    watchdog(WATCHDOG_WARNING, "Commerce module not found!");
  }
}

