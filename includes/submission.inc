<?php

/**
 * Defines how a registration is submitted to CiviCRM
 */

include_once('webform_integration.inc');



/**
 * AJAX call handler for 'discount code field changed'
 */
function ica_event_submission($form_values) {
  // TODO: values not directly accessible in $form_values. Maybe we have to load 
  //   them from the DB via the 'node'.

  //  1) call XCM to get contact ID
  // $main_contact = ica_event_cmrf_connector_sendCall('Contact', 'getorcreate', 
  //                         array(
  //                           'contact_type' => 'Individual',
  //                           'first_name'   => '',
  //                         ),
  //                         array());


  //  2) create all participation objects / payment information

  // TODO: use API chaining for performance
}

