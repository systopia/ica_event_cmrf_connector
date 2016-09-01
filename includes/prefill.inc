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
  $prefill_cross_page = array(
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
    ICA_EVENT_MATRIX);


  // run cross-page prefill on the form data
  $current_values = ica_event_get_form_values($form, $form_state);
  foreach ($prefill_cross_page as $source_field_name => $target_field_name) {
    if (!empty($current_values[$source_field_name])) {
      _ica_event_do_prefill($form, $current_values[$source_field_name], $target_field_name);
    }
  }

  // run same-page prefill by appending JS
  foreach ($prefill_same_page as $field_name) {
    if (isset($form['submitted'][$field_name])) {
      // element is there => attach JS
      $form['#attached']['js'][] = drupal_get_path('module', 'ica_event_cmrf_connector') . '/js/prefill.js';
      break;
    }
  }
}


function _ica_event_do_prefill(&$form, $value, $field_name) {
  // only if target is being rendered:
  if (isset($form['submitted'][$field_name])) {
    // only if it's not already set
    if (empty($form['submitted'][$field_name]['#value'])) {
      $form['submitted'][$field_name]['#value'] = $value;
    }
  }
}
