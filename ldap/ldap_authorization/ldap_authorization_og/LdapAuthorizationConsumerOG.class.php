<?php
// $Id: LdapAuthorizationConsumerOG.class.php,v 1.3.2.1 2011/02/08 20:05:42 johnbarclay Exp $



/**
 * @file
 * abstract class to represent an ldap_authorization consumer
 * such as drupal_role, og_group, etc.
 *
 */
require_once(drupal_get_path('module', 'ldap_authorization') . '/LdapAuthorizationConsumerAbstract.class.php');
class LdapAuthorizationConsumerOG extends LdapAuthorizationConsumerAbstract {

  public $consumerType = 'og_group';
  public $allowSynchBothDirections = FALSE;
  public $allowConsumerObjectCreation = TRUE;
  public $onlyApplyToLdapAuthenticatedDefault = TRUE;
  public $useMappingsAsFilterDefault = TRUE;
  public $synchOnLogonDefault = TRUE;
  public $synchManuallyDefault = TRUE;
  public $revokeLdapProvisionedDefault = TRUE;
  public $regrantLdapProvisionedDefault = TRUE;
  public $createContainersDefault = TRUE;
  public $ogByName = array();

 /**
   * Constructor Method
   *
   */
  function __construct($consumer_type = NULL) {
    $params = ldap_authorization_og_ldap_authorization_consumer();
    $this->refreshConsumerIDs();
    parent::__construct('og_group', $params['og_group']);
  }

  public function refreshConsumerIDs() {
    // assemble a groups array for use in two ways, by name and by ID
    $ogByName = array();
    $ogEntities = og_load_multiple(og_get_all_group());
    foreach($ogEntities as $group) {
      $ogByName[$group->gid] = $group->label;
    }

    $this->ogByName = array_flip($ogByName);
    $this->_availableConsumerIDs = $ogByName;
  }

  public function availableConsumerIDs($reset = FALSE) {
    if ($reset || ! is_array($this->_availableConsumerIDs)) {
      $this->refreshConsumerIDs();
    }
    return $this->_availableConsumerIDs;
  }

  /**
   * extends createConsumer method of base class
   *
   * creates of organic groups may be mixed case.  drupal doesn't
   * differentiate, so case is ignored in comparing, but preserved
   * for the actual created group name saved.
   *
   * **/

  public function createConsumers($creates_mixed_case) {
try{

	watchdog('ldap_authorization_og','creating consumers',array(),WATCHDOG_DEBUG);

    // 1. determines difference between existing organic groups and ones that are requested to be created
    $existing_groups_mixed_case = $this->availableConsumerIDs();
    $creates_lower_case = array_map('drupal_strtolower', $creates_mixed_case);
    $existing_groups_lower_case = array_map('drupal_strtolower', $existing_groups_mixed_case);
    $groups_map_lc_to_mixed_case = array_combine($creates_lower_case, $creates_mixed_case);
    $groups_to_create = array_diff($creates_lower_case, $existing_groups_lower_case);

    // 2. create each group that is needed
    foreach ($groups_to_create as $i => $group_name_lowercase) {

      // create a new Group programatically...
      //
      // create a node of type determined from a configuration setting (default is 'group')
      // created node must have requested name
      // "group type" flag must be on
      // body doesn't matter
      // add user to this group later

      $group_node = new stdClass();
      $group_node->type = variable_get('ldap_authorization_og_node_type', 'group');
      $group_node->title    = $groups_map_lc_to_mixed_case[$group_name_lowercase];

      $group_node->created         = time();
      $group_node->changed        = $group_node->created;
      $group_node->language = LANGUAGE_NONE;
      // TODO: use configuration parameter to determine user that creates this node
      $group_node->uid   = token_replace('[current-user:uid]');

      $group_node->body[$group_node->language][0]['value']   = '';
      $group_node->body[$group_node->language][0]['format']  = 'filtered_html';

      // sets "group type" flag to "on"
      $group_node->{'group_group'}[$group_node->language][0]['value'] = 1;

      node_submit($group_node);
      node_save($group_node);

      $created[] = $group_node->title;
      watchdog('user', 'OG group %group in ldap_authorizations module', array('%group' => $group_node->title));

    }

    // 3. return all existing groups and flush cache of consumer ids.
    $refreshed_available_consumer_ids = $this->availableConsumerIDs(TRUE);
    if ($this->detailedWatchdogLog) {
    	$watchdog_tokens = array();
      $watchdog_tokens['%groups_to_create'] = join(", ", $groups_to_create);
      $watchdog_tokens['%existing_groups'] = join(", ", $existing_groups_mixed_case);
      $watchdog_tokens['%refreshed_available_consumer_ids'] = join(", ", $refreshed_available_consumer_ids);
      watchdog('ldap_authorization_og',
        'LdapAuthorizationConsumerOG.createConsumers() <br />
        <hr />groups to create: %groups_to_create
        <hr />existing groups: %existing_groups
        <hr />available groups after createConsumers call: %refreshed_available_consumer_ids',
        $watchdog_tokens,
        WATCHDOG_DEBUG);
    }


    return $refreshed_available_consumer_ids;  // return actual groups that exist, in case of failure

  } catch (Exception $e) {
      watchdog('ldap_authorization_og',$e,array(),WATCHDOG_DEBUG);
  }
  }

  // not currently executed properly (?)
  public function revokeSingleAuthorization(&$user, $group_name, &$user_auth_data) {

    // use og_ungroup() [ & og_role_revoke() ?]
    // need $gid, $entity_type (user), $entity
    // og_ungroup(1,'user',user_load(1));

    // get $gid from $group_name
    //

    $gid = FALSE;

    if(in_array($group_name, $this->ogByName)) {
      $gid = $this->ogByName[$group_name];
    }

    if($gid!==FALSE) {
    	// get membership entity
    	$membership_entity = og_get_group_membership($gid,'user',$user->uid);
    	// unsubscribe user
      og_ungroup($gid,'user',$user);
      // delete membership entity
      og_membership_delete_multiple(array($membership_entity->id));
      $result = TRUE;
    }else {
      // could not find group name.. handle error please?
      $result = FALSE;
    }

    if ($this->detailedWatchdogLog) {
      watchdog('ldap_authorization_og', 'LdapAuthorizationConsumerOG.revokeSingleAuthorization()
        revoked:  gid=%gid, group_name=%group_name for username=%username, result=%result',
        array('%gid' => $gid, '%group_name' => $group_name, '%username'=> $user->name,
          '%result' => $result), WATCHDOG_DEBUG);
    }

    return $result;

  }

  /**
   * add user to group and grant a role.
   *
   * extends grantSingleAuthorization()
   */
  public function grantSingleAuthorization(&$user, $group_name, &$user_auth_data) {

      $result = FALSE;

      watchdog('ldap_authorization_og','LdapAuthorizationConsumerOG.grantSingleAuthorization() <hr />
                beginning to grant authorization for $group_name=%group_name to user %username',
                  array('%group_name'=>$group_name, '%username'=>$user->name),
                  WATCHDOG_DEBUG);

    $gid = FALSE;

    $ogEntities = og_load_multiple(og_get_all_group());
    foreach($ogEntities as $group) {
      if($group_name === $group->label){

        $gid = $group->gid;
      }
    }

    if($gid!==FALSE) {


      // check to make sure that user does not already belong to this group
      if(!og_get_group_membership($gid,'user',$user->uid)) {
      	// user added to group
        og_group($gid, array('entity_type'=>'user', 'entity'=>$user));

        // 'membership fields' => array(),

        // get role IDs for group
        $OGroles = og_roles($gid);

        $OGroleAssigned = variable_get('ldap_authorization_og_role_assigned', 2);

        // user given a role in group
        og_role_grant($gid, $user->uid, $OGroleAssigned);

        // modify group_audience field for user

				$settings = array(
				        'group_audience' => array(
				            'entity_id' => $user->uid,
				            'group_audience_gid' => $gid,
				            'group_audience_state' => '1',
				        ),
				    );
		    $output = field_bundle_settings("user", "user", $settings);

        $result = TRUE;
        if ($this->detailedWatchdogLog) {
          watchdog('ldap_authorization_og', 'LdapAuthorizationConsumerOG.grantSingleAuthorization()
                    <hr />granted: gid=%gid, group_name=%group_name for username=%username, result=%result, '
                    . 'OG role granted=%OGroles, settings output =  ' . $output,
                     array('%gid' => $gid, '%group_name' => $group_name, '%username'=> $user->name,
                     '%result' => $result, '%OGroles' => $OGroles[$OGroleAssigned]), WATCHDOG_DEBUG);
        }
      } else {
        // user already added to group.
        $result = TRUE;
          watchdog('ldap_authorization_og', 'LdapAuthorizationConsumerOG.grantSingleAuthorization()
                    <hr />not granted: gid=%gid, group_name=%group_name for username=%username, result=%result
                    <br />because user already belongs to group',
                     array('%gid' => $gid, '%group_name' => $group_name, '%username'=> $user->name,
                     '%result' => $result), WATCHDOG_DEBUG);

      }
    }else {
      $result = FALSE;
      watchdog('ldap_authorization_og', 'LdapAuthorizationConsumerOG.grantSingleAuthorization()
                failed to grant %username the group %group_name because group does not exist',
                array('%group_name' => $group_name, '%username'=> $user->name),
                WATCHDOG_ERROR);
    }

    return $result;
  }

  // returns an array of the values/names of the groups to which the user belongs
  public function usersAuthorizations(&$user) {

    $users_groups = array();

    if(isset($some_user->group_audience['und'][0])) {
      foreach($user->group_audience['und'] as $users_group) {
		    if( in_array($users_group['gid'], $this->availableConsumerIDs()) ) {
		      $users_groups[$users_group['gid']] = $this->ogByName[$users_group['gid']];
        }
      }
    }

    return $users_groups;
  }


}
