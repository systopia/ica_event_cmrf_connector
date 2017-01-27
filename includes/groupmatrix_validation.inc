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
  error_log(json_encode($items));
  error_log(json_encode($components));
  error_log(json_encode($rule));
}


// /**
//  * Additional validation for the group registration matrix
//  */
// function ica_event_groupmatrix_validation($validator_name, $items, $components, $rule) {
//   // if ($items) {
//   //   switch ($validator_name) {
//   //     case 'validate_id':
//   //       $valid_ids = array(1, 2, 3); // get this from database source instead
//   //       $errors = array();
//   //       foreach ($items as $key => $val) {
//   //         if ($val && (!in_array($val, $valid_ids))) {
//   //           $errors[$key] = t('%item is not a valid ID', array('%item' => $components[$key]['name']));
//   //         }
//   //       }
//   //       return $errors;
//   //       break;
//   //   }
//   // }
// }
