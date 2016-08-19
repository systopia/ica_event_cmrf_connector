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
 * This file deals with discount codes
 */

include_once('webform_integration.inc');


/**
 * AJAX call handler for 'discount code field changed'
 */
function ica_event_discount_code_ajax_callback($form, $form_state) {
  $commands = array();

  $commands[] = ajax_command_replace('#edit-submitted-' . str_replace('_', '-', ICA_EVENT_DISCOUNTCODE), 'CODE ACCEPTED');
  ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
  exit;
}

