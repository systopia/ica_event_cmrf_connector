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

/**
 * get the name of an organisation by ID
 *  if possible, return the acronym
 */
function ica_member_get_organisation($organisation_id, $acronym=FALSE) {
  $organisations = ica_member_organisations();
  $full_name = $organisations[$organisation_id];
  
  // try to find an acronym
  if ($acronym && preg_match('/\((?P<acronym>\w+)\)$/', $full_name, $matches)) {
    return $matches['acronym'];
  } else {
    return $full_name;
  }
}
