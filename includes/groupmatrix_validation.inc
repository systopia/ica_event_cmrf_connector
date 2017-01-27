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
 * Custom validation for the group registration matrix
 */

include_once('webform_integration.inc');


/**
 * Validate the group matrix entries
 */
function ica_event_groupmatrix_validation($items, $components, $rule) {
  global $ica_event_matrix_fields_group_registration;
  $field_indexes = array_flip($ica_event_matrix_fields_group_registration);

  $all_errors = array();
  foreach ($items as $key => $rows) {
    $errors = array();
    foreach ($rows as $row_nr => $row_data_raw) {
      if (!is_numeric($row_nr)) continue;
      $row_data = ica_event_resolve_matrixrow_idexes($row_data_raw);
      if (!empty($row_data['partner'])) {
        // there is a partner, it should have a name
        if (empty($row_data['partner_first_name'])) {
          $errors[] = t('Partner in row %row_nr needs a first name', array('%row_nr' => $row_nr));
        }

        if (empty($row_data['partner_last_name'])) {
          $errors[] = t('Partner in row %row_nr needs a last name', array('%row_nr' => $row_nr));
        }

        if (empty($row_data['partner_prefix'])) {
          $errors[] = t('Partner in row %row_nr needs a prefix', array('%row_nr' => $row_nr));
        }
      }
    }
    if (!empty($errors)) {
      $all_errors[$key] = '<ul><li>' . implode('</li><li>', $errors) . '</li></ul>';
    }
  }

  return $all_errors;
}
