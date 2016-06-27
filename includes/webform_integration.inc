<?php

/**
 * Defines the expected webform fields
 */

// constants: general
define('ICA_EVENT_FORM_IDS',           'webform_client_form_1,webform_client_form_3');
define('ICA_EVENT_YOUTH_MAX_BIRTHDAY', '1987-09-30');
define('ICA_REGISTRANT_COUNTRY_LOCAL', 'MY');

// constants: registration fees
define('ICA_EVENT_FEE_YOUTH',          200.00);
define('ICA_EVENT_FEE_LOCAL',          250.00);
define('ICA_EVENT_FEE_MEMBER_INT',     750.00);
define('ICA_EVENT_FEE_PARTICIPANT',    950.00);
define('ICA_EVENT_FEE_SPOUSE',         100.00);


// active fields
define('ICA_EVENT_AMOUNT',             'amount');
define('ICA_EVENT_DISCOUNTCODE',       'discount_code');

// relevant fields for price calculation
define('ICA_EVENT_SPOUSE',             'spouse');
define('ICA_REGISTRANT_ATTENDING',     'registrant_attending');
define('ICA_REGISTRANT_COUNTRY',       'country');
define('ICA_EVENT_YOUTH',              'youth');
define('ICA_MEMBER_ORGANISATION',      'member_organisation');

// matrix fields
define('ICA_EVENT_MATRIX',             'group_registration_table');
define('ICA_EVENT_MATRIX_SPOUSE_IDX',  8);
define('ICA_EVENT_MATRIX_YOUTH_IDX',   11);



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