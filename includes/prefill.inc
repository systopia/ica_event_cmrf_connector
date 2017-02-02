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

include_once('webform_integration.inc');

/**
 * 
 */
function ica_event_process_prefill(&$form, $form_state) {
  global $user;

  $prefill_cross_page = array(
    ICA_MEMBER_ORGANISATION                             => ICA_ORGANISATION_PREFIX . 'organization_name',
    ICA_MEMBER_ORGANISATION                             => ICA_ORGANISATION_PREFIX . 'badge',
    ICA_GENERAL_COUNTRY                                 => ICA_ORGANISATION_PREFIX . 'country',
    ICA_ORGANISATION_PREFIX . 'street_address'          => ICA_BILLING_PREFIX . 'street_address',
    ICA_ORGANISATION_PREFIX . 'supplemental_address_1'  => ICA_BILLING_PREFIX . 'supplemental_address_1',
    ICA_ORGANISATION_PREFIX . 'supplemental_address_2'  => ICA_BILLING_PREFIX . 'supplemental_address_2',
    ICA_ORGANISATION_PREFIX . 'postal_code'             => ICA_BILLING_PREFIX . 'postal_code',
    ICA_ORGANISATION_PREFIX . 'city'                    => ICA_BILLING_PREFIX . 'city',
    ICA_ORGANISATION_PREFIX . 'country'                 => ICA_BILLING_PREFIX . 'country',
    ICA_ORGANISATION_PREFIX . 'organization_name'       => ICA_REGISTRANT_ORG_BADGE,
  );

  $prefill_same_page = array(
    ICA_REGISTRANT_PREFIX . 'badge',
    ICA_REGISTRANT_PREFIX . 'organisation_badge',
    ICA_PARTNER_PREFIX    . 'badge',
    ICA_REGISTRANT_PREFIX . 'email',
    ICA_EVENT_MATRIX);


  // run cross-page prefill on the form data
  $current_values = ica_event_get_form_values($form, $form_state);
  // error_log("storage: " . $form_state['storage']['submitted'][76]);
  // error_log("values: " . $form_state['values']['submitted'][76]);
  // error_log("input: " . $form_state['input']['submitted']['billing_street_address']);
  // error_log("current: " . $current_values['billing_street_address']);
  foreach ($prefill_cross_page as $source_field_name => $target_field_name) {
    if (!empty($current_values[$target_field_name])) {
      continue; // no prefill for existing values
    }
    if (substr($source_field_name, 0, 9) == 'CONSTANT:') {
      $current_value = substr($source_field_name, 9);
    } elseif (!empty($current_values[$source_field_name])) {
      $current_value = $current_values[$source_field_name];
    } else {
      $current_value = NULL;
    }

    // resolve organisation ID
    if ($current_value && $source_field_name == ICA_MEMBER_ORGANISATION) {
      include_once('optionlists.inc');
      $current_value = ica_member_get_organisation_short($current_value);
    }
    
    if (!empty($current_value)) {
      $form['submitted'][$target_field_name]['#default_value'] = $current_value;
    }
  }

  // run same-page prefill by appending JS
  foreach ($prefill_same_page as $field_name) {
    if (isset($form['submitted'][$field_name])) {
      // element is there => attach JS
      drupal_add_js(array('ica_event_cmrf_connector' => array('variables' => array('email' => $user->mail))), 'setting');
      $form['#attached']['js'][] = drupal_get_path('module', 'ica_event_cmrf_connector') . '/js/prefill.js';
      break;
    }
  }
}

