<?php

/**
 * Pull certain dropdown option lists from CiviCMR via CMRF
 */



/**
 * Provide a list of all active ICA member organisations
 */
function ica_member_organisations() {
  
  // FIRST: find active memberships
  $call1 = ica_event_cmrf_connector_sendCall('Membership', 'get', 
                          array(
                            'owner_membership_id' => array('IS NULL' => 1),
                            'active_only' => 1,
                            'return' => 'contact_id'
                          ),
                          array(
                            'limit'  => 2000,
                            'cache'  => '12 hours',
                          ));

  $contact_ids = array();
  foreach ($call1->getValues() as $entry) {
    $contact_ids[] = $entry['contact_id'];
  }


  // THEN: load all members' display_name
  $call2 = ica_event_cmrf_connector_sendCall('Contact', 'get', 
                          array(
                            'id' => array('IN' => $contact_ids),
                            'is_deleted' => 0,
                            'return' => 'display_name,id'
                          ),
                          array(
                            'limit'  => 2000,
                            'sort'   => 'display_name',
                            'cache'  => '12 hours',
                          ));


  // FINALLY: compile webform variable set
  $organisations = $call2->getValues();
  $variables = array();
  foreach ($organisations as $organisation) {
    $variables[$organisation['id']] = $organisation['display_name'];
  }

  return $variables;
}