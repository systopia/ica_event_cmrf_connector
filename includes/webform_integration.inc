<?php

/**
 * Defines the expected webform fields
 */

// constants: general
define('ICA_EVENT_FORM_IDS',           'webform_client_form_1,webform_client_form_3,webform_client_form_5');
define('ICA_EVENT_YOUTH_MAX_BIRTHDAY', '1987-09-30');
define('ICA_REGISTRANT_COUNTRY_LOCAL', 'MY');
define('ICA_DEFAULT_CURRENCY',         'EUR');


// constants: registration fees
define('ICA_EVENT_FEE_YOUTH',          200.00);
define('ICA_EVENT_FEE_LOCAL',          250.00);
define('ICA_EVENT_FEE_MEMBER_INT',     750.00);
define('ICA_EVENT_FEE_PARTICIPANT',    950.00);
define('ICA_EVENT_FEE_PARTNER',        100.00);



// active fields
define('ICA_EVENT_AMOUNT',             'amount');
define('ICA_EVENT_DISCOUNTCODE',       'discount_code');

// relevant fields for price calculation
define('ICA_EVENT_PARTNER',            'partner');
define('ICA_REGISTRANT_ATTENDING',     'registrant_attending');
define('ICA_REGISTRANT_COUNTRY',       'country');
define('ICA_EVENT_YOUTH',              'youth');
define('ICA_MEMBER_ORGANISATION',      'member_organisation');

// matrix
define('ICA_EVENT_MATRIX',             'group_registration_table');

// submission data
define('ICA_REGISTRANT_PREFIX',        'registrant_');
define('ICA_PARTNER_PREFIX',           'partner_');

// DEFINE matrix fields
global $ica_event_matrix_fields_group_registration;
$ica_event_matrix_fields_group_registration = array(
    1  => 'registrant_title',
    2  => 'registrant_prefix',
    3  => 'registrant_last_name',
    4  => 'registrant_first_name',
    5  => 'registrant_badge_name',
    6  => 'registrant_email',
    7  => 'registrant_languages',
    8  => 'partner',
    9  => 'partner_badge_name',
    10 => 'comments',
    11 => 'youth',
    12 => 'registrant_organization_name',
    13 => 'registrant_position',
  );

global $ica_event_registration_properties;
$ica_event_registration_properties = array(
        'first_name' => 'first_name', 
        'last_name'  => 'last_name',
        'prefix'     => 'prefix_id', 
        'badge'      => 'custom_badge',
        'email'      => 'email');

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
      $values[$key] = $value;
    }
  }

  // TODO: add $form['submitted']??
  // error_log(json_encode($values));
  return $values;
}

