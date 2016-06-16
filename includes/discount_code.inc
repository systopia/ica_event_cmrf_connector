<?php

/**
 * This file deals with discount codes
 */

include_once('includes/webform_integration.inc');


/**
 * AJAX call handler for 'discount code field changed'
 */
function ica_event_discount_code_ajax_callback($form, $form_state) {
  $commands = array();

  $commands[] = ajax_command_replace('#edit-submitted-' . str_replace('_', '-', ICA_EVENT_DISCOUNTCODE), 'CODE ACCEPTED');
  ajax_deliver(array('#type' => 'ajax', '#commands' => $commands));
  exit;
}

