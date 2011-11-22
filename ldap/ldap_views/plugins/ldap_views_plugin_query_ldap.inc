<?php

/**
 * @file
 * Defines the default query object which builds and execute a ldap query
 */
class ldap_views_plugin_query_ldap extends views_plugin_query {

  /**
   * The base dn for the LDAP search
   */
  private $basedn = '';

  /**
   * A list of filters to apply to the LDAP search
   */
  private $filter = array();

  /**
   * Builds the necessary info to execute the query.
   */
  function build(&$view) {
    $view->init_pager($view);

    // Let the pager modify the query to add limits.
    $this->pager->query();
  }

  function add_field($table, $field, $alias = '', $params = array()) {
    // We check for this specifically because it gets a special alias.
    if ($table == $this->base_table && $field == $this->base_field && empty($alias)) {
      $alias = $this->base_field;
    }

    if (!$alias && $table) {
      $alias = $table . '_' . $field;
    }

    // Make sure an alias is assigned
    $alias = $alias ? $alias : $field;

    // PostgreSQL truncates aliases to 63 characters: http://drupal.org/node/571548

    // We limit the length of the original alias up to 60 characters
    // to get a unique alias later if its have duplicates
    $alias = drupal_substr($alias, 0, 60);

    // Create a field info array.
    $field_info = array(
      'field' => $field,
      'table' => $table,
      'alias' => $alias,
    ) + $params;

    // Test to see if the field is actually the same or not. Due to
    // differing parameters changing the aggregation function, we need
    // to do some automatic alias collision detection:
    $base = $alias;
    $counter = 0;
    while (!empty($this->fields[$alias]) && $this->fields[$alias] != $field_info) {
      $field_info['alias'] = $alias = $base . '_' . ++$counter;
    }

    if (empty($this->fields[$alias])) {
      $this->fields[$alias] = $field_info;
    }

    return $alias;
  }

  function add_orderby($table, $field, $order, $alias = '', $params = array()) {
    $this->orderby[] = array(
      'field' => $field,
      'direction' => drupal_strtoupper($order)
    );
  }

  function add_basedn($basedn) {
    $this->basedn = !empty($this->basedn) ? $this->basedn : $basedn;
  }

  function add_filter($filter) {
    if (empty($filter)) {
      return;
    }
    $this->filter[] = $filter;
  }

  /**
   * Add a simple WHERE clause to the query. The caller is responsible for
   * ensuring that all fields are fully qualified (TABLE.FIELD) and that
   * the table already exists in the query.
   *
   * @param $group
   *   The WHERE group to add these to; groups are used to create AND/OR
   *   sections. Groups cannot be nested. Use 0 as the default group.
   *   If the group does not yet exist it will be created as an AND group.
   * @param $field
   *   The name of the field to check.
   * @param $value
   *   The value to test the field against. In most cases, this is a scalar. For more
   *   complex options, it is an array. The meaning of each element in the array is
   *   dependent on the $operator.
   * @param $operator
   *   The comparison operator, such as =, <, or >=. It also accepts more complex
   *   options such as IN, LIKE, or BETWEEN. Defaults to IN if $value is an array
   *   = otherwise. If $field is a string you have to use 'formula' here.
   *
   * @see QueryConditionInterface::condition()
   */
  function add_where($group, $field, $value = NULL, $operator = NULL) {
    // Ensure all variants of 0 are actually 0. Thus '', 0 and NULL are all
    // the default group.
    if (empty($group)) {
      $group = 0;
    }

    // Check for a group.
    if (!isset($this->where[$group])) {
      $this->set_where_group('AND', $group);
    }

    $this->where[$group]['conditions'][] = array(
      'field' => $field,
      'value' => $value,
      'operator' => ltrim($operator, '!'),
      'negate' => drupal_substr($operator, 0, 1) == '!',
    );
  }

  /**
   * Construct the filter
   *
   * @param $where
   *   'where' or 'having'.
   */
  function build_condition() {
    $operator = array('AND' => '&', 'OR' => '|');
    $main_group = '';
    foreach ($this->where as $group => $info) {
      if (!empty($info['conditions'])) {
        $sub_group = '';
        foreach ($info['conditions'] as $key => $clause) {
          $item       = '(' . $clause['field'] . $clause['operator'] . $clause['value'] . ')';
          $sub_group .= $clause['negate'] ? "(!$item)" : $item;
        }
        $main_group .= count($info['conditions']) <= 1 ? $sub_group : '(' . $operator[$info['type']] . $sub_group . ')';
      }
    }
    return count($this->where) <= 1 ? $main_group : '(' . $operator[$this->group_operator] . $main_group . ')';
  }

  function build_ldap_basedn($basedn) {
    return !empty($this->basedn) ? array($this->basedn) : $basedn;
  }

  function build_contextual_filter() {
    $contextual_filter = '';
    foreach ($this->filter as $condition) {
      $contextual_filter .= drupal_substr($condition, 0, 1) != '(' ? "($condition)" : $condition;
    }
    return $contextual_filter;
  }

  function build_ldap_filter($filter) {
    $condition     = $this->build_condition();
    $contextual    = $this->build_contextual_filter();
    $search_filter = !empty($contextual) && !empty($condition)? '(&' . $condition . $contextual . ')' : $condition . $contextual;
    return !empty($search_filter) ? $search_filter : $filter;
  }

  /**
   * Ensure a table exists in the queue; if it already exists it won't
   * do anything, but if it doesn't it will add the table queue. It will ensure
   * a path leads back to the relationship table.
   *
   * @param $table
   *   The unaliased name of the table to ensure.
   * @param $relationship
   *   The relationship to ensure the table links to. Each relationship will
   *   get a unique instance of the table being added. If not specified,
   *   will be the primary table.
   * @param $join
   *   A views_join object (or derived object) to join the alias in.
   *
   * @return
   *   The alias used to refer to this specific table, or NULL if the table
   *   cannot be ensured.
   */
  function ensure_table($table, $relationship = NULL, $join = NULL) {
    return $table;
  }

  /**
   * Executes the query and fills the associated view object with according
   * values.
   *
   * Values to set: $view->result, $view->total_rows, $view->execute_time,
   * $view->pager['current_page'].
   *
   * $view->result should contain an array of objects.
   */
  function execute(&$view) {
    $start       = microtime();
    $entries     = array();
    $num_entries = 0;

    if (empty($this->options['qid'])) {
      watchdog('ldap_views', t("Query definition is empty"));
      return;
    }
    foreach ($this->fields as $field) {
      $attributes[$field['alias']] = $field['field'];
      $field_alias[$field['alias']] = drupal_strtolower($field['field']);
    }

    $ldap_data   = new LdapQuery(ldap_views_get_qid($view));
    $ldap_server = new LdapServer($ldap_data->sid);
    $ldap_server->connect();
    $ldap_server->bind();
    // TODO: Can't use sizelimit if it must be ordered || cache?
    // $ldap_server->search() hasn't orderby (ldap_sort)
    // But we can't use ldap_sort because there's no DESC option
    // $entries     = $ldap_server->search($this->build_ldap_basedn($ldap_data->basedn), $this->build_ldap_filter($ldap_data->filter), array_values($attributes));
    // TODO: this $functions array should be changed with a $ldap_server or a future $ldap_api method. Now $ldap_server->search only do a ldap_search() search,
    //   there's no ldap_list() or ldap_read()
    $functions = array('LDAP_SCOPE_BASE' => 'ldap_read', 'LDAP_SCOPE_ONELEVEL' => 'ldap_list', 'LDAP_SCOPE_SUBTREE' => 'ldap_search');
    $function  = isset($ldap_data->scope) && array_key_exists($ldap_data->scope, $functions) ? $functions[$ldap_data->scope] : $functions['LDAP_SCOPE_ONELEVEL']; // $functions['LDAP_SCOPE_SUBTREE']
    foreach ($this->build_ldap_basedn($ldap_data->baseDn) as $basedn) {
      
    $result    = @$function($ldap_server->connection, $basedn, $this->build_ldap_filter($ldap_data->filter), array_values($attributes));
    if (!$result) {
      $msg = t("Could not retrieve data from LDAP. (Error %errno: %error) (BaseDN: %basedn) (Filter: %filter) (Attrs: %attrs).",
        array(
          '%errno'  => ldap_errno($ldap_server->connection),
          '%error'  => ldap_error($ldap_server->connection),
          '%basedn' => $this->build_ldap_basedn($ldap_data->basedn),
          '%filter' => $this->build_ldap_filter($ldap_data->filter),
          '%attrs'  => implode(',', array_values($attributes))
        )
      );
      watchdog('ldap_views', $msg);
      return;
    }
/***
    // ldap_sort can't be used because there's no DESC option
    if (!empty($this->orderby)) {
      // Array reverse, because the most specific are first - PHP works the opposite way of SQL.
      foreach (array_reverse($this->orderby) as $field) {
        @ldap_sort($ldap_server->connection, $result, $field['field']);
      }
    }
 */
    $tmp1         = ldap_get_entries($ldap_server->connection, $result);
    $num_entries += array_shift($tmp1);
    $tmp2         = array_merge($entries, $tmp1);
    $entries      = $tmp2;
    unset($tmp1);
    unset($tmp2);
    }
    
//    $num_entries = array_shift($entries);
    $limit       = $view->query->limit;
    $offset      = $view->query->offset;
    $result      = array();
    $sort_fields = array();
    if (!empty($this->orderby)) {
      foreach ($this->orderby as $orderby) {
        $sort_fields[drupal_strtolower($orderby['field'])]['direction'] = $orderby['direction'];
        $sort_fields[drupal_strtolower($orderby['field'])]['data']      = array();
      }

    }

    foreach ($entries as $key => &$entry) {
        foreach ($view->field as $field) {
          if (array_key_exists($field_alias[$field->field_alias], $entry)) {
            if (is_array($entry[$field_alias[$field->field_alias]])) {
              switch ($field->options['multivalue']) {
                case 'v-all':
                  // remove 'count' index
                  array_shift($entry[$field_alias[$field->field_alias]]);
                  $entry[$field_alias[$field->field_alias]] = implode($field->options['value_separator'], $entry[$field_alias[$field->field_alias]]);
                  break;
                case 'v-count':
                  $entry[$field_alias[$field->field_alias]] = $entry[$field_alias[$field->field_alias]]['count'];
                  break;
                case 'v-index':
                  $index = $field->options['index_value'] >= 0 ? intval($field->options['index_value']) : $entry[$field_alias[$field->field_alias]]['count'] + $field->options['index_value'];
                  $entry[$field_alias[$field->field_alias]] = array_key_exists($index, $entry[$field_alias[$field->field_alias]]) ? $entry[$field_alias[$field->field_alias]][$index] :
                                                                                                                                    $entry[$field_alias[$field->field_alias]][0];
                  break;
            }
          }
          // order criteria
          if (array_key_exists($field_alias[$field->field_alias], $sort_fields)) {
            $sort_fields[$field_alias[$field->field_alias]]['data'][$key] = $entry[$field_alias[$field->field_alias]];
          }
        }
      }
    }
    if (!empty($this->orderby) && !empty($entries)) {
      $params = array();
      // In PHP 5.3 every parameter in the array has to be a reference when calling array_multisort() with call_user_func_array().
      $asc  = SORT_ASC;
      $desc = SORT_DESC;
      foreach ($sort_fields as &$field) {
        $params[] = &$field['data'];
        if (drupal_strtoupper($field['direction']) == 'ASC') {
          $params[] = &$asc;
        }
        else {
          $params[] = &$desc;
        }
      }
      $params[] = &$entries;
      call_user_func_array('array_multisort', $params);
    }

    for ($i = 0; $i < $limit && $offset + $i < $num_entries; $i++) {
      $row = array();
      $entry = &$entries[$offset + $i];
      foreach ($view->field as $field) {
        if (array_key_exists($field_alias[$field->field_alias], $entry)) {
          $row[$field->field_alias] = $entry[$field_alias[$field->field_alias]];
        }
      }
      $result[] = $row;
    }

    $view->result       = $result;
    $view->total_rows   = $num_entries;
    $view->execute_time = microtime() - $start;
    $view->query->pager->total_items  = $num_entries;
    $view->query->pager->update_page_info();

  }

  function option_definition() {
    $options = parent::option_definition();
    $options['qid'] = array('default' => '');

    return $options;
  }

  function options_form(&$form, &$form_state) {
/*
    $ldap_data = entity_load('ldap_data', FALSE);
    $options   = array();
    foreach ($ldap_data as $data) {
        $options[$data->qid] = $data->name;
    }
*/
    $queries = array();
    $queries['all'] = LdapQueryAdmin::getLdapQueryObjects();

  foreach ($queries['all'] as $_sid => $ldap_query) {
    if ($ldap_query->status == 1) {
      //$queries['enabled'][$_qid] = $ldap_query;
      $options[$ldap_query->qid] = $ldap_query->name;
    }
  }
// ******************************************************
      $form['qid'] = array(
      '#type' => 'select',
      '#title' => t('LDAP Search'),
      '#options' => $options,
      '#default_value' => $this->options['qid'],
      '#description' => t("The LDAP server to query."),
    );
  }

/* Only when adding dynamic fields in ldap_views_views_data_alter()
  function options_submit(&$form, &$form_state) {
    parent::options_submit(&$form, &$form_state);
    if ($form_state['values']['query']['options']['qid'] != $form_state['view']->query->options['qid']) {
      views_invalidate_cache();
    }
  }
 */
}