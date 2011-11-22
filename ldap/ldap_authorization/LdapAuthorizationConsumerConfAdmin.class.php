<?php
// $Id: LdapAuthorizationConsumerConfAdmin.class.php,v 1.6.2.1 2011/02/08 06:01:00 johnbarclay Exp $

  /**
   * @file
   * class to encapsulate an ldap authorization ldap entry to authorization ids mapping
   *
   */
    require_once('LdapAuthorizationConsumerConf.class.php');

  /**
   * LDAP Authorization Consumer Configration Admin Class
   */
class LdapAuthorizationConsumerConfAdmin extends LdapAuthorizationConsumerConf {


  public function save() {

    $op = $this->inDatabase ? 'edit' : 'insert';
    $values = $this;
    $values->sid = $this->sid;
    $values->consumer_type = $this->consumerType;
    $values->consumer_module = $this->consumer->consumerModule;
    $values->description = $this->description;
    $values->status = (int)$this->status;
    $values->only_ldap_authenticated = (int)$this->onlyApplyToLdapAuthenticated;
    $values->derive_from_dn = (int)$this->deriveFromDn;
    $values->derive_from_dn_attr = $this->deriveFromDnAttr;
    $values->derive_from_attr = (int)$this->deriveFromAttr;
    $values->derive_from_attr_attr = $this->arrayToLines($this->deriveFromAttrAttr);
    $values->derive_from_attr_use_first_attr = (int)$this->deriveFromAttrUseFirstAttr;
    $values->derive_from_entry = (int)$this->deriveFromEntry;
    $values->derive_from_entry_search_all = (int)$this->deriveFromEntrySearchAll;
    $values->derive_from_entry_entries = $this->arrayToLines($this->deriveFromEntryEntries);
    $values->derive_from_entry_attr = $this->deriveFromEntryAttr;
    $values->mappings = $this->arrayToPipeList($this->mappings);
    $values->use_filter = (int)$this->useMappingsAsFilter;
    $values->synch_to_ldap = (int)$this->synchToLdap;
    $values->synch_on_logon = (int)$this->synchOnLogon;
    $values->revoke_ldap_provisioned = (int)$this->revokeLdapProvisioned;
    $values->create_consumers = (int)$this->createConsumers;
    $values->regrant_ldap_provisioned = (int)$this->regrantLdapProvisioned;

    if (module_exists('ctools')) {
      ctools_include('export');
      // Populate our object with ctool's properties
      $object = ctools_export_crud_new('ldap_authorization');
      foreach ($object as $property => $value) {
        if (!isset($values->$property)) {
          $values->$property = $value;
        }
      }
      $result = ctools_export_crud_save('ldap_authorization', $values);
    }
    elseif ($op == 'edit') {
      $result = drupal_write_record('ldap_authorization', $values, 'consumer_type');
    }
    else { // insert
      $result = drupal_write_record('ldap_authorization', $values);
    }

    if ($result) {
      $this->inDatabase = TRUE;
    }
    else {
      drupal_set_message(t('Failed to write LDAP Authorization to the database.'));
    }

    // rever mappings to array and remove temporary properties from ctools export
    $this->mappings = $this->pipeListToArray($values->mappings);
    foreach (array('consumer_type', 'consumer_module', 'only_ldap_authenticated',
      'derive_from_dn', 'derive_from_dn_attr', 'derive_from_attr', 'derive_from_attr_attr', 'derive_from_attr_use_first_attr',
      'derive_from_entry', 'derive_from_entry_entries', 'derive_from_entry_attr', 'derive_from_entry_search_all', 'use_filter',
      'synch_to_ldap', 'synch_on_logon', 'revoke_ldap_provisioned', 'create_consumers',
      'regrant_ldap_provisioned') as $prop_name) {
      unset($this->{$prop_name});
    }
  }

  public $fields;
  public $consumers;

  public function delete() {
    if ($this->consumerType) {
      $this->inDatabase = FALSE;
      return db_delete('ldap_authorization')->condition('consumer_type', $this->consumerType)->execute();
    }
    else {
      return FALSE;
    }
  }

  public function __construct(&$consumer = NULL, $new = FALSE) {
    parent::__construct($consumer, $new);
    $this->fields = $this->fields();
    $this->consumers = ldap_authorization_get_consumers(NULL, TRUE);

    if ($new) {
      foreach ($this->consumer->defaultableConsumerConfProperties as $property) {
        $default_prop_name = $property . 'Default';
        $this->$property = $this->consumer->$default_prop_name;
      }
    }
  }

  public function drupalForm($server_options, $op) {

    $consumer_tokens = ldap_authorization_tokens($this->consumer);
    $form['intro'] = array(
        '#type' => 'item',
        '#markup' => t('<h1>LDAP to !consumer_name Configuration</h1>', $consumer_tokens),
    );

    $form['status_intro'] = array(
        '#type' => 'item',
        '#title' => t('Part I.  Core Configuration.', $consumer_tokens),
    );

    $form['status'] = array(
      '#type' => 'fieldset',
      '#title' => t('Core Configuration', $consumer_tokens),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    $form['status']['sid'] = array(
      '#type' => 'radios',
      '#title' => t('LDAP Server used in !consumer_name configuration.', $consumer_tokens),
      '#required' => 1,
      '#default_value' => $this->sid,
      '#options' => $server_options,
    );

    $form['status']['consumer_type'] = array(
      '#type' => 'hidden',
      '#value' => $this->consumerType,
      '#required' => 1,
    );

    $form['status']['description'] = array(
      '#type' => 'textfield',
      '#title' => t('Short description for this !consumer_shortName configuration.', $consumer_tokens),
      '#default_value' => $this->description,
      '#size' => 60,
      '#maxlength' => 60,
    );

    $form['status']['status'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable this configuration', $consumer_tokens),
      '#default_value' =>  $this->status,
    );

    $form['status']['only_ldap_authenticated'] = array(
      '#type' => 'checkbox',
      '#title' => t('Only apply the following LDAP to !consumer_name configuration to users authenticated via LDAP.', $consumer_tokens),
      '#default_value' =>  $this->onlyApplyToLdapAuthenticated,
    );


    $form['mapping_intro'] = array(
        '#type' => 'item',
        '#title' => t('Part II.  How are !consumer_name !consumer_namePlural derived from LDAP data?', $consumer_tokens),
        '#markup' => t('First, we need to configure how LDAP data is used to derive !consumer_name !consumer_namePlural.
          One or more of the following 3 approaches may be used.', $consumer_tokens),
    );
    /**
     *  derive from DN option
     */
    $form['derive_from_dn'] = array(
      '#type' => 'fieldset',
      '#title' => t('II.A. Derive !consumer_name from DN', $consumer_tokens),
      '#collapsible' => TRUE,
      '#collapsed' => !$this->deriveFromDn,
    );

    $form['derive_from_dn']['derive_from_dn'] = array(
      '#type' => 'checkbox',
      '#title' => t('!consumer_name is derived from user\'s DN', $consumer_tokens),
      '#default_value' => $this->deriveFromDn,
      '#description' => t('<p>Check this option if your users\' DNs look like <code>cn=jdoe,<strong>ou=Group1</strong>,cn=example,cn=com</code> and <code>Group1</code> turns out to be the !consumer_name_short you want.</p>', $consumer_tokens),
    );

    $form['derive_from_dn']['derive_from_dn_attr'] = array(
      '#type' => 'textfield',
      '#title' => t('Attribute of the DN which contains the !consumer_shortName name', $consumer_tokens),
      '#default_value' => $this->deriveFromDnAttr,
      '#size' => 50,
      '#maxlength' => 255,
      '#description' => t('The name of the attribute which contains the !consumer_shortName name. In the example above, it would be
        <code>ou</code>, as the DN contains the string <code>ou=Group1</code> and <code>Group1</code>
        happens to be the desired !consumer_short_name name.', $consumer_tokens),
    );

     /**
     *  derive from attributes option
     */

    $form['derive_from_attr'] = array(
      '#type' => 'fieldset',
      '#title' => t('II.B. Derive !consumer_namePlural by attribute', $consumer_tokens),
      '#collapsible' => TRUE,
      '#collapsed' => !$this->deriveFromAttr,
    );

    $form['derive_from_attr']['derive_from_attr'] = array(
      '#type' => 'checkbox',
      '#title' => t('!consumer_shortNamePlural are specified by LDAP attributes', $consumer_tokens),
      '#default_value' => $this->deriveFromAttr,
    );

    $form['derive_from_attr']['derive_from_attr_attr'] = array(
      '#type' => 'textarea',
      '#title' => t('Attribute names (one per line)'),
      '#default_value' => $this->arrayToLines($this->deriveFromAttrAttr),
      '#cols' => 50,
      '#rows' => 6,
      '#description' => t('If the !consumer_shortNamePlural are stored in the user entries, along with the rest of their data, then enter here a list of attributes which may contain them.', $consumer_tokens),
    );

    $form['derive_from_attr']['derive_from_attr_use_first_attr'] = array(
      '#type' => 'checkbox',
      '#title' => t('Convert full dn to value of first attribute.  e.g.  <code>cn=admin group,ou=it,dc=ad,dc=nebraska,dc=edu</code> would be converted to <code>admin group</code>', $consumer_tokens),
      '#default_value' => $this->deriveFromAttrUseFirstAttr,
    );


     /**
     *  derive from attributes option
     */

    $form['derive_from_entry'] = array(
      '#type' => 'fieldset',
      '#title' => t('II.C. Derive !consumer_namePlural from entry', $consumer_tokens),
      '#collapsible' => TRUE,
      '#collapsed' => !$this->deriveFromEntry,
    );

    $form['derive_from_entry']['derive_from_entry'] = array(
      '#type' => 'checkbox',
      '#title' => t('!consumer_shortNamePlural exist as LDAP entries where a multivalued attribute contains the members\' CNs', $consumer_tokens),
      '#default_value' => $this->deriveFromEntry,
    );


    $form['derive_from_entry']['derive_from_entry_entries'] = array(
      '#type' => 'textarea',
      '#title' => t('LDAP DNs containing !consumer_shortNamePlural (one per line)', $consumer_tokens),
      '#default_value' => $this->arrayToLines($this->deriveFromEntryEntries),
      '#cols' => 50,
      '#rows' => 6,
      '#description' => t('Enter here a list of LDAP nodes from where !consumer_shortNamePlural should be searched for.
        The module will look them up recursively from the given nodes.', $consumer_tokens),
    );




    $form['derive_from_entry']['derive_from_entry_attr'] = array(
      '#type' => 'textfield',
      '#title' => t('Attribute holding !consumer_shortNamePlural members', $consumer_tokens),
      '#default_value' => $this->deriveFromEntryAttr,
      '#size' => 50,
      '#maxlength' => 255,
      '#description' => t('Name of the multivalued attribute which holds the CNs of !consumer_shortNamePlural members,
         for example: memberUid', $consumer_tokens),
    );


    $form['derive_from_entry']['derive_from_entry_search_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Search all enabled LDAP servers for matching users.  (Enables roles on one server referencing users on another)'),
      '#default_value' => $this->deriveFromEntrySearchAll,
    );


     /**
     *  filter and whitelist
     */

    $form['filter_intro'] = array(
      '#type' => 'item',
      '#title' => t('Part III.  Mapping and White List.', $consumer_tokens),
      '#markup' => t('The rules in Part I. will create a list of !consumer_name !consumer_namePlural.
        The next field allows you to transform the !consumer_name !consumer_namePlural derived in Part I.
        By checking the checkbox below it, the same list can be used as a white list to limit which !consumer_name !consumer_namePlural
        are mapped from LDAP.', $consumer_tokens),
      );

    $form['filter_and_mappings'] = array(
      '#type' => 'fieldset',
      '#title' => t('III.A. LDAP to !consumer_name mapping and filtering', $consumer_tokens),
      '#description' => t('
The settings in part II generate a list of "raw authorization ids" which
need to be converted to !consumer_namePlural.
Raw authorization ids might look like the following depending on which of the options in II. are chosen:
<ul>
<li>ou=Underlings,dc=myorg,dc=mytld,dc=edu</li>
<li>ou=IT,dc=myorg,dc=mytld,dc=edu</li>
<li>Campus Accounts</li>
</ul>

<p>(II.B and II.C. should generate DNs for raw authorization ids as in the first 2 examples.
II.A. will generate a simple string such
as the third example.)</p>

<p><strong>The mapping specified below will convert these raw authorization ids to !consumer_namePlural.</strong></p>

Mappings to deal with the above 3 examples might be:
<pre>
ou=Underlings,dc=myorg,dc=mytld|underlings
ou=IT,dc=myorg,dc=mytld,dc=edu|administrator
Campus Accounts|authenticated user
</pre>

Enter one mapping per line with an <code>|</code> separating the raw authorization id and its !consumer_name.
', $consumer_tokens),
      '#collapsible' => TRUE,
      '#collapsed' => !($this->mappings || $this->useMappingsAsFilter),
    );

    $form['filter_and_mappings']['mappings'] = array(
      '#type' => 'textarea',
      '#title' => t('Mapping of LDAP to !consumer_name', $consumer_tokens),
      '#default_value' => $this->arrayToPipeList($this->mappings),
      '#cols' => 50,
      '#rows' => 5,
    );
    $form['filter_and_mappings']['use_filter'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use LDAP group to !consumer_namePlural filtering', $consumer_tokens),
      '#default_value' => $this->useMappingsAsFilter,
      '#description' => t('If enabled, only above mapped !consumer_namePlural will be assigned.
        <strong>If not checked, many !consumer_namePlural may be created.</strong>', $consumer_tokens)
    );

    $form['advanced_intro'] = array(
        '#type' => 'item',
        '#title' => t('Part IV.  More Settings.', $consumer_tokens),
        '#markup' => t('', $consumer_tokens),
    );

/**
 *
 * @todo for 7.x-2.x
  $form['advanced_intro'] = array(
        '#type' => 'item',
        '#title' => t('IV.A. Map in both directions.', $consumer_tokens),
        '#markup' => t('', $consumer_tokens),
    );


   $form['misc_settings']['allow_synch_both_directions'] = array(
      '#type' => 'checkbox',
      '#disabled' => !$this->consumer->allowSynchBothDirections,
      '#default_value' => $this->synchToLdap,
      '#title' => t('Check this option if you want LDAP data to be modified if a user
        has a !consumer_name.  In other words, synchronize both ways.  For this to work the ldap server
        needs to writeable, the right side of the mappings list must be unique, and I.B or I.C.
        derivation must be used.', $consumer_tokens),
    );
 */

    $synchronization_modes = array();
    if ($this->synchOnLogon)  {
      $synchronization_modes[] = 'user_logon';
    }
    $form['misc_settings']['synchronization_modes'] = array(
      '#type' => 'checkboxes',
      '#title' => t('IV.B. When should !consumer_namePlural be granted/revoked from user?', $consumer_tokens),
      '#options' => array(
          'user_logon' => t('When a user logs on'),
          'manually' => t('Manually or via another module')
      ),
      '#default_value' => $synchronization_modes,
      '#description' => t('<p>"When a user logs on" is the common way to do this.</p>', $consumer_tokens),
    );

    $synchronization_actions = array();
    if ($this->revokeLdapProvisioned)  {
      $synchronization_actions[] = 'revoke_ldap_provisioned';
    }
    if ($this->createConsumers)  {
      $synchronization_actions[] = 'create_consumers';
    }
    if ($this->regrantLdapProvisioned)  {
      $synchronization_actions[] = 'regrant_ldap_provisioned';
    }
    $form['misc_settings']['synchronization_actions'] = array(
      '#type' => 'checkboxes',
      '#title' => t('IV.C. What actions would you like performed when !consumer_namePlural are granted/revoked from user?', $consumer_tokens),
      '#options' => array(
          'revoke_ldap_provisioned' => t('Revoke !consumer_namePlural previously granted by LDAP Authorization but no longer valid.', $consumer_tokens),
          'regrant_ldap_provisioned' => t('Re grant !consumer_namePlural previously granted by LDAP Authorization but removed manually.', $consumer_tokens),
          'create_consumers' => t('Create !consumer_namePlural if they do not exist.', $consumer_tokens),
      ),
      '#default_value' => $synchronization_actions,
    );
    /**
     * @todo  some general options for an individual mapping (perhaps in an advance tab).
     *
     * - on synchronization allow: revoking authorizations made by this module, authorizations made outside of this module
     * - on synchronization create authorization contexts not in existance when needed (drupal roles etc)
     * - synchronize actual authorizations (not cached) when granting authorizations
     */

    switch ($op) {
      case 'add':
      $action = 'Add';
      break;

      case 'edit':
      $action = 'Edit';
      break;

      case 'delete':
      $action = 'Delete';
      break;
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => $action,
    );

  return $form;
  }


  protected function loadFromForm($values, $op) {

  }

  public function getLdapAuthorizationConsumerActions() {
    $actions = array();
    $actions[] =  l(t('edit'), LDAP_SERVERS_MENU_BASE_PATH . '/authorization/edit/' . $this->consumerType);
    if (property_exists($this, 'type')) {
      if ($this->type == 'Overridden') {
          $actions[] = l(t('revert'), LDAP_SERVERS_MENU_BASE_PATH . '/authorization/delete/' . $this->consumerType);
      }
      if ($this->type == 'Normal') {
          $actions[] = l(t('delete'), LDAP_SERVERS_MENU_BASE_PATH . '/authorization/delete/' . $this->consumerType);
      }
    }
    else {
        $actions[] = l(t('delete'), LDAP_SERVERS_MENU_BASE_PATH . '/authorization/delete/' . $this->consumerType);
    }
    $actions[] = l(t('test'), LDAP_SERVERS_MENU_BASE_PATH . '/authorization/test/' . $this->consumerType);
    return $actions;
  }

  public function drupalFormValidate($op, $values)  {
    $errors = array();

    if ($op == 'delete') {
      if (!$this->consumerType) {
        $errors['consumer_type_missing'] = 'Consumer type is missing from delete form.';
      }
    }
    else {

      $this->populateFromDrupalForm($op, $values);


      $errors = $this->validate();
      if (count($this->mappings) == 0 && trim($values['mappings'])) {
        $errors['mappings'] = t('Bad mapping syntax.  Text entered but not able to convert to array.');
      }

    }
    return $errors;
  }

  public function validate() {
    $errors = array();

    if (!$this->consumerType) {
      $errors['consumer_type'] = t('Consumer type is missing.');
    }

    if ($this->inDatabase  && (!$this->consumerType)) {
      $errors['consumer_type'] = t('Edit or delete called without consumer type in form.');
    }

    // are correct values available for selected mapping approach
    if ($this->deriveFromDn && !trim($this->deriveFromDnAttr)) {
      $errors['derive_from_dn'] = t('DN attribute is missing.');
    }
    if ($this->deriveFromAttr && !count($this->deriveFromAttrAttr)) {
      $errors['derive_from_attr'] = t('Attribute names are missing.');
    }
    if ($this->deriveFromEntry && !count($this->deriveFromEntryEntries)) {
      $errors['derive_from_entry'] = t('Nodes are missing.');
    }
    if ($this->deriveFromEntry && !trim($this->deriveFromEntryAttr)) {
      $errors['derive_from_entry_attribute'] = t('Attribute is missing.');
    }

    if (count($this->mappings) > 0) {
      foreach ($this->mappings as $mapping_item) {
        list($map_from, $map_to) = $mapping_item;
      // validate $mapto is valid mapping consumer as much as possible.  perhaps alphanum or call hook validate to provider
      }
    }
    if ($this->useMappingsAsFilter && !count($this->mappings)) {
      $errors['mappings'] = t('Mappings are missing.');
    }
    return $errors;
  }

  protected function populateFromDrupalForm($op, $values) {

    $this->inDatabase = (drupal_strtolower($op) == 'edit');
    $values['mappings'] = $this->pipeListToArray($values['mappings']);
    $values['derive_from_attr_attr'] = $this->linesToArray($values['derive_from_attr_attr']);
    $values['derive_from_entry_entries'] = $this->linesToArray($values['derive_from_entry_entries']);

    $this->sid = $values['sid'];
    $this->consumerType = $values['consumer_type'];
    $this->description = $values['description'];
    $this->status = (bool)$values['status'];
    $this->onlyApplyToLdapAuthenticated  = (bool)(@$values['only_ldap_authenticated']);

    $this->deriveFromDn  = (bool)(@$values['derive_from_dn']);
    $this->deriveFromDnAttr = $values['derive_from_dn_attr'];

    $this->deriveFromAttr  = (bool)($values['derive_from_attr']);
    $this->deriveFromAttrAttr = $values['derive_from_attr_attr'];
    $this->deriveFromAttrUseFirstAttr  = (bool)($values['derive_from_attr_use_first_attr']);
    $this->deriveFromEntrySearchAll  = (bool)($values['derive_from_entry_search_all']);

    $this->deriveFromEntry  = (bool)(@$values['derive_from_entry']);
    $this->deriveFromEntryEntries = $values['derive_from_entry_entries'];
    $this->deriveFromEntryAttr = $values['derive_from_entry_attr'];

    $this->mappings = $values['mappings'];
    $this->useMappingsAsFilter  = (bool)(@$values['use_filter']);


    $this->synchOnLogon = (bool)(@$values['synchronization_modes']['user_logon']);
    $this->regrantLdapProvisioned = (bool)(@$values['synchronization_actions']['regrant_ldap_provisioned']);
    $this->revokeLdapProvisioned = (bool)(@$values['synchronization_actions']['revoke_ldap_provisioned']);
    $this->createConsumers = (bool)(@$values['synchronization_actions']['create_consumers']);

  }

  public function drupalFormSubmit($op, $values) {

    $this->populateFromDrupalForm($op, $values);
    if ($op == 'delete') {
      $this->delete();
    }
    else { // add or edit

      try {
        $save_result = $this->save();
      }
      catch (Exception $e) {
        $this->errorName = 'Save Error';
        $this->errorMsg = t('Failed to save object.  Your form data was not saved.');
        $this->hasError = TRUE;
      }
    }
  }


  public static function fields() {

     /**
     * consumer_type is tag (unique alphanumeric id) of consuming authorization such as
     *   drupal_roles, og_groups, civicrm_memberships
     */
    $fields = array(
      'numeric_consumer_conf_id' => array(
          'schema' => array(
            'type' => 'serial',
            'unsigned' => TRUE,
            'not null' => TRUE,
            'description' => 'Primary ID field for the table.  Only used internally.',
            'no export' => TRUE,
          ),
        ),
      'sid' => array(
        'schema' => array(
          'type' => 'varchar',
          'length' => 20,
          'not null' => TRUE,
        )
      ),
      'consumer_type' => array(
         'schema' => array(
            'type' => 'varchar',
            'length' => 20,
            'not null' => TRUE,
        )
      ),
     'consumer_module' => array(
         'schema' => array(
            'type' => 'varchar',
            'length' => 30,
            'not null' => TRUE,
        )
      ),

      'description' => array(
        'schema' => array(
          'type' => 'varchar',
          'length' => '60',
          'not null' => FALSE
        )
      ),

      'status' => array(
          'schema' => array(
            'type' => 'int',
            'size' => 'tiny',
            'not null' => TRUE,
            'default' => 0,
          )
      ),
      'only_ldap_authenticated' => array(
        'schema' => array(
          'type' => 'int',
          'size' => 'tiny',
          'not null' => TRUE,
          'default' => 1,
        )
      ),
      'derive_from_dn' => array(
        'schema' => array(
          'type' => 'int',
          'size' => 'tiny',
          'not null' => TRUE,
          'default' => 0,
        )
      ),
      'derive_from_dn_attr' => array(
        'schema' => array(
          'type' => 'varchar',
          'length' => 255,
          'default' => NULL,
        )
      ),
      'derive_from_attr' => array(
        'schema' => array(
          'type' => 'int',
          'size' => 'tiny',
          'not null' => TRUE,
          'default' => 0,
        )
      ),
      'derive_from_attr_attr' => array(
        'schema' => array(
          'type' => 'varchar',
          'length' => 255,
          'default' => NULL,
        )
      ),
      'derive_from_attr_use_first_attr' => array(
        'schema' => array(
          'type' => 'int',
          'size' => 'tiny',
          'not null' => TRUE,
          'default' => 0,
        )
      ),
      'derive_from_entry'  => array(
          'schema' => array(
            'type' => 'int',
            'size' => 'tiny',
            'not null' => TRUE,
            'default' => 0,
        )
      ),
      'derive_from_entry_entries' => array(
        'form_default' => array(),
        'schema' => array(
          'default' => NULL,
          'type' => 'text',
        )
      ),

      'derive_from_entry_attr' => array(
        'schema' => array(
          'type' => 'varchar',
          'length' => 255,
          'default' => NULL,
        )
      ),

      'derive_from_entry_search_all'  => array(
          'schema' => array(
            'type' => 'int',
            'size' => 'tiny',
           'not null' => TRUE,
            'default' => 0,
        )
      ),

      'mappings'  => array(
        'form_default' => array(),
        'schema' => array(
          'type' => 'text',
          'not null' => FALSE,
          'default' => NULL,
        )
      ),

      'use_filter' => array(
        'schema' => array(
          'type' => 'int',
          'size' => 'tiny',
          'not null' => TRUE,
          'default' => 1,
        )
      ),

      'synchronization_modes' => array(
        'form_default' =>  array('user_logon'),
      ),

      'synchronization_actions' => array(
        'form_default' =>  array('revoke_ldap_provisioned', 'create_consumers'),
      ),

      'synch_to_ldap'  => array(
        'schema' => array(
          'type' => 'int',
          'size' => 'tiny',
          'not null' => TRUE,
          'default' => '0',
        ),
      ),

      'synch_on_logon'  => array(
        'schema' => array(
          'type' => 'int',
          'size' => 'tiny',
          'not null' => TRUE,
          'default' => '0',
        ),
      ),

      'revoke_ldap_provisioned'  => array(
        'schema' => array(
          'type' => 'int',
          'size' => 'tiny',
          'not null' => TRUE,
          'default' => '0',
        ),
      ),

     'create_consumers'  => array(
        'schema' => array(
          'type' => 'int',
          'size' => 'tiny',
          'not null' => TRUE,
          'default' => '0',
        ),
      ),

     'regrant_ldap_provisioned'  => array(
        'schema' => array(
          'type' => 'int',
          'size' => 'tiny',
          'not null' => TRUE,
          'default' => '0',
        ),
      ),
    );
    return $fields;
  }




  protected function arrayToPipeList($array) {
    $result_text = "";
    foreach ($array as $map_pair) {
      $result_text .= $map_pair[0] . '|' . $map_pair[1] . "\n";
    }
    return $result_text;
  }

  protected function arrayToLines($array) {
        $lines = "";
        if (is_array($array)) {
          $lines = join("\n", $array);
        }
        elseif (is_array(@unserialize($array))) {
          $lines = join("\n", unserialize($array));
        }
        return $lines;
      }



}
