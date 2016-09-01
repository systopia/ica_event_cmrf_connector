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
 * Defines the expected webform fields
 */

// constants: general
define('ICA_EVENT_SUBMISSION_PREFIX',  'GA2017');
define('ICA_EVENT_FORM_IDS',           'webform_client_form_1,webform_client_form_3,webform_client_form_5');
define('ICA_EVENT_YOUTH_MAX_BIRTHDAY', '1987-09-30');
define('ICA_REGISTRANT_COUNTRY_LOCAL', 'MY');
define('ICA_DEFAULT_CURRENCY',         'EUR');
define('ICA_CIVICRM_EVENT_ID',         '1');


// constants: registration fees
define('ICA_EVENT_FEE_YOUTH',          200.00);
define('ICA_EVENT_FEE_LOCAL',          250.00);
define('ICA_EVENT_FEE_MEMBER_INT',     750.00);
define('ICA_EVENT_FEE_PARTICIPANT',    950.00);
define('ICA_EVENT_FEE_PARTNER',        100.00);



// active fields
define('ICA_EVENT_AMOUNT',             'amount');
define('ICA_EVENT_DISCOUNTCODE',       'discount_code');

// relevant fields for prefill
define('ICA_GENERAL_COUNTRY',          'general_country');
define('ICA_REGISTRANT_ORG_BADGE',     'registrant_organisation_badge');

// relevant fields for price calculation
define('ICA_EVENT_PARTNER',            'partner');
define('ICA_REGISTRANT_ATTENDING',     'registrant_attending');
define('ICA_REGISTRANT_COUNTRY',       'country');
define('ICA_EVENT_BIRTH_DATE',         'birth_date');
define('ICA_EVENT_YOUTH',              'youth');
define('ICA_MEMBER_ORGANISATION',      'registrant_represents');

// matrix
define('ICA_EVENT_MATRIX',             'group_registration_table');

// submission data
define('ICA_REGISTRANT_PREFIX',        'registrant_');
define('ICA_ORGANISATION_PREFIX',      'organisation_');
define('ICA_BILLING_PREFIX',           'billing_');
define('ICA_PARTNER_PREFIX',           'partner_');

// DEFINE matrix fields
global $ica_event_matrix_fields_group_registration;
$ica_event_matrix_fields_group_registration = array(
    1  => 'registrant_prefix',
    2  => 'registrant_formal_title',
    3  => 'registrant_last_name',
    4  => 'registrant_first_name',
    5  => 'registrant_badge_name',
    6  => 'registrant_email',
    7  => 'registrant_languages',
    8  => 'registrant_position',
    9  => 'registrant_organisation_badge',
    10 => 'birth_date',
    11 => 'partner',
    12 => 'partner_prefix',
    13 => 'partner_title',
    14 => 'partner_first_name',
    15 => 'partner_last_name',
    16 => 'partner_badge',
    17 => 'registrant_comments',
  );

global $ica_event_registration_properties;
$ica_event_registration_properties = array(
        'first_name'             => 'first_name', 
        'last_name'              => 'last_name',
        'organization_name'      => 'organization_name',
        'prefix'                 => 'prefix_id', 
        'position'               => 'custom_Position',
        'languages'              => 'custom_Languages',
        'represents'             => 'custom_represented_organisation',
        'badge'                  => 'custom_badge',
        'organisation_badge'     => 'custom_organisation_badge',
        'comments'               => 'participant_note',
        'youth'                  => 'birth_date',
        'email'                  => 'email',
        'phone'                  => 'phone',
        'street_address'         => 'street_address',
        'supplemental_address_1' => 'supplemental_address_1',
        'supplemental_address_2' => 'supplemental_address_2',
        'postal_code'            => 'postal_code',
        'city'                   => 'city',
        'country'                => 'country',
        );

/**
 * translate a the matrix's indices to field_names using $ica_event_matrix_fields constant
 */
function ica_event_get_matrix_values($raw_matrix_data, $matrix_fields) {
  // first: prepare matrix data:
  if (!$raw_matrix_data) return array();

  if (!is_array($raw_matrix_data)) {
    $raw_matrix_data = array(0 => $raw_matrix_data);
  }

  $matrix_data = array();
  foreach ($raw_matrix_data as $matrix_block) {
    $matrix_data += unserialize($matrix_block);
  }


  // then: relsove the fields
  $matrix_values = array();
  foreach ($matrix_data as $key => $values) {
    $matrix_values[$key] = array();
    foreach ($values as $element_index => $value) {
      $field_name = $matrix_fields[$element_index];
      $matrix_values[$key][$field_name] = $value;
    }
  }

  return $matrix_values;
}


/**
 * get all known values of a multipage webform
 */
function ica_event_get_form_values(&$form, &$form_state) {
  $values = array();

  // check if it's a webform
  if (!is_object($form['#node']) || !isset($form['#node']->webform)) {
    return $values;
  }
  
  // first, extract values from $form_state
  $webform = $form['#node']->webform;
  $components = $webform['components'];
  $form_state_values = array('values/submitted', 'input/submitted', 'storage/submitted');

  foreach ($form_state_values as $value_path) {
    $path_elements = explode('/', $value_path);
    $element = $form_state;
    foreach ($path_elements as $path) {
      if (isset($element[$path])) {
        $element = $element[$path];
      } else {
        continue 2;
      }
    }

    // $element should have all values now
    foreach ($element as $key => $value) {
      if (isset($components[$key])) {
        $key = $components[$key]['form_key'];
      } else {
        continue;
      }

      if (is_array($value) && count($value) == 1) {
        $values[$key] = reset($value);
      } else {
        $values[$key] = $value;
      }
    }
  }

  // TODO: add $form['submitted']??
  // error_log(json_encode($values));
  return $values;
}

