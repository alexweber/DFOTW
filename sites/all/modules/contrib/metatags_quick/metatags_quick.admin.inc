<?php
function metatags_quick_admin_settings() {
  $current_settings = variable_get('metatags_quick_settings', _metatags_quick_settings_default());
  $module_path = drupal_get_path('module', 'metatags_quick');
  $fields = field_info_fields();
  $metatags_found = FALSE;

  include_once $module_path . '/known_tags.inc';
  $known_tags = _metatags_quick_known_fields();
  
  $form['path_based'] = array(
    '#type' => 'fieldset',
    '#title' => t('Path-based meta tags'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  foreach ($fields as $key => $field) {
    if ($field['module'] != 'metatags_quick') {
      continue;
    }
    $metatags_found = TRUE;
  }
  if (!$metatags_found) {
    $form['path_based']['basic_init'] = array(
      '#markup' => t('No meta tags found in your installation') . '<br/>',
    );
  }
  
  $form['path_based']['use_path_based'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use path-based meta tags'),
    '#default_value' => variable_get('metatags_quick_use_path_based', 1),
    '#return_value' => 1,
  );
  
  $form['standard_tags'] = array(
    '#type' => 'fieldset',
    '#title' => t('Create and attach'),
    '#description' => t('The following meta tags are known to the module and can be created automatically. However, you are not limited to this list and can define tags of your own using the Fields UI.'),
    '#collapsible' => TRUE,
    '#attached' => array(
      'js' => array($module_path . '/js/admin.js'),
      'css' => array($module_path . '/css/admin.css'),
    ),
  );
  $form['standard_tags']['hint'] = array(
    '#markup' => t('Hint: press on the bundle name to see individual entity types'),
  );
  
  $field_instances = field_info_instances();

  // Build the sortable table header.
  $header = array(
    'title' => array('data' => t('Bundle/entity')),
  );
  foreach ($known_tags as $name => $tag) {
    $header[$name] = $tag['title'];
  }
  //$header['_select_all'] = t('Select all');
  
  foreach (field_info_bundles() as $entity_type => $bundles) {
    $entity_info = entity_get_info($entity_type);
    if (!$entity_info['fieldable']) {
      continue;
    }
    $options[$entity_type]['data'] = array(
      'title' => array(
        'class' => array('entity_type_collapsible', 'entity_type_collapsed', "entity_name_$entity_type"),
        'data' => $entity_info['label']
      )
    );
    foreach ($known_tags as $name => $tag) {
      $bundle_workable[$name] = FALSE;
      $options[$entity_type]['data'][$name] = array(
        'data' => array(
          '#type' => 'checkbox',
          '#attributes' => array('class' => array('cb_bundle_parent', "cb_bundle_name_{$entity_type}_{$name}")),      
          '#return_value' => 1,
          '#checked' => FALSE,
        ),
      );
    }
    /*$options[$entity_type]['data']['_select_all'] = array(
      'data' => array(
      '#type' => 'checkbox',
      '#return_value' => 1,
      '#checked' => FALSE,
      ),
    );*/
    
    // How do we mark that specific meta is already attached to bundle
    $checked_markup = array(
      '#markup' => theme('image', 
        array(
          'path' => 'misc/watchdog-ok.png',
          'width' => 18,
          'height' => 18,
          'alt' => 'ok',
          'title' => 'ok',
        )),
    );
        
    foreach ($bundles as $key => $bundle) {
      // Which meta tags are set for this bundle?
      $meta_set = array();
      foreach ($field_instances[$entity_type][$key] as $bundle_instance) {
        if ($bundle_instance['widget']['module'] == 'metatags_quick') {
          $field_info = field_info_field_by_id($bundle_instance['field_id']);
          $meta_set[$field_info['settings']['meta_name']] = 1;
        }        
      }
      
      $options[$entity_type . '_' . $key] = array(
        'class' => array('entity_type_children', "entity_child_$entity_type"),
        'style' => 'display: none',
        'data' => array(
          'title' => array(
            'class' => array('entity_type_child_title'),
            'data' => $bundle['label'],
          ),
        ),
      );
      foreach ($known_tags as $name => $tag) {
        if (empty($meta_set[$name])) {
          // Mark parent bundle as workable - display checkbox.
          $bundle_workable[$name] = TRUE;
          $options[$entity_type . '_' . $key]['data'][$name] = array(
            'data' => array(
              '#name' => $entity_type . '[' . $key . '][' . $name . ']',
              '#type' => 'checkbox',
              '#attributes' => array('class' => array('cb_bundle_child', "cb_child_{$entity_type}_{$name}")),
              '#return_value' => 1,
              '#checked' => FALSE,
            )
          );
        }
        else {
          $options[$entity_type . '_' . $key]['data'][$name]['data'] = $checked_markup; 
        }
      }
      /*$options[$entity_type . '_' . $key]['data']['_select_all']['data'] = array(
        '#type' => 'checkbox',
        '#return_value' => 1,
        '#checked' => FALSE,
      );*/
    }
    // Now check if we have completely set bundles
    foreach ($known_tags as $name => $tag) {
      if (!$bundle_workable[$name]) {
        $options[$entity_type]['data'][$name]['data'] = $checked_markup; 
      }
    }
  }
  
  $form['standard_tags']['existing'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#empty' => t('No content available.'),
  );
  
  $form['standard_tags']['basic_init_op'] = array(
    '#type' => 'submit',
    '#value' => t('Attach'),
  );
    
  $form['op'] = array(
    '#value' => t('Submit'),
    '#type' => 'submit', 
  );
  return $form;
}

function metatags_quick_admin_settings_submit($form, &$form_state) {
  variable_set('metatags_quick_use_path_based', $form_state['values']['use_path_based']);
  if ($form_state['clicked_button']['#value'] == t('Attach')) {
    _metatags_quick_admin_fields_create_attach($form_state['input']);
  }
  else {
  }
  drupal_set_message(t('Meta tags (quick) settings saved'), 'status');
}

/**
 * Path based form callback
 */
function metatags_quick_admin_path_page() {
  if (!empty($_GET['path']) && !empty($_GET['lang'])) {
    return drupal_get_form('metatags_quick_admin_path', $_GET['lang'], $_GET['path']);
  }
  else {
    return MENU_ACCESS_DENIED;
  }
}

/*
 * Path-based meta tags editing form.
 * @param string $path - path being edited.
 */
function metatags_quick_admin_path($form, &$form_state) {
  $args = func_get_args();
  if (count($args) > 3) {
    $lang = $args[2];
    $path = $args[3];
  }
  else {
    return MENU_ACCESS_DENIED;
  }
  
  $controller = new DrupalDefaultEntityController('metatags_path_based');
  $entity_id = db_select('metatags_quick_path_based', 'm')
    ->fields('m', array('id'))
    ->condition('lang', $lang)
    ->condition('path', $path)
    ->execute()
    ->fetchField();
  if (!$entity_id) {
    $entity_id = db_insert('metatags_quick_path_based')
      ->fields(array('lang' => $lang, 'path' => $path))
      ->execute();
  }
  $entities = $controller->load(array($entity_id));
  $form_state['entity'] = $entities[$entity_id];
  field_attach_form('metatags_path_based', $entities[$entity_id], $form, $form_state);
  // Do we have any fields attached?
  $childen = element_children($form);
  if (!$childen) {
    $form['empty_message'] = array(
      '#markup' => t('No fields attached to the path-based meta tags. Please !define them first in the module settings screen',
        array('!define' => l(t('define'), 'admin/config/search/metatags_quick'))) . '<br/>',
    );
  }
  $form['op'] = array(
    '#value' => t('Submit'),
    '#type' => 'submit', 
  );  
  return $form;
}

function metatags_quick_admin_path_validate($form, &$form_state) {
  entity_form_field_validate('metatags_path_based', $form, $form_state);
}

function metatags_quick_admin_path_submit($form, &$form_state) {
  if (!$form_state['entity']) {
    form_set_error();
    drupal_set_message(t('Wrong path'), 'error');
    return;
  }
  
  $entity = $form_state['entity'];
  entity_form_submit_build_entity('metatags_path_based', $entity, $form, $form_state);
  field_attach_update('metatags_path_based', $entity);
  
  drupal_set_message('Meta tags updated', 'status');
  $form_state['redirect'] = FALSE;
}

// Create known meta fields.
function _metatags_quick_admin_fields_create_attach($input) {
  foreach (field_info_bundles() as $entity_type => $bundles) {
    $entity_info = entity_get_info($entity_type);
    if (!$entity_info['fieldable']) {
      continue;
    }
    
    foreach ($bundles as $key => $bundle) {
      if (isset($input[$entity_type][$key])) {
        foreach ($input[$entity_type][$key] as $meta_name => $meta_value) {
          _metatags_quick_field_attach($entity_type, $key, $meta_name);
        }
      }
    }
  }
}
  
function _metatags_quick_field_attach($entity_type, $bundle, $meta_name) {
  static $meta_fields = FALSE;
  static $field_id = array();
  static $known_tags = FALSE;
  
  // Get metatags_quick fields info
  if (!$meta_fields) {
    include_once drupal_get_path('module', 'metatags_quick') . '/known_tags.inc';
    $known_tags = _metatags_quick_known_fields();
    
    foreach(field_info_fields() as $name => $field_info) {
      if ($field_info['module'] == 'metatags_quick') {
        $meta_fields[$field_info['settings']['meta_name']] = $field_info;
        $field_id[$field_info['settings']['meta_name']] = $field_info['id'];
      }
    }
  }
  
    // Ignore unknown tags.
  if (!isset($known_tags[$meta_name])) {
    return;
  }
  
  // Check if meta field exists, create if necessary.
  if (empty($field_id[$meta_name])) {
    $field = array(
      'field_name' => "meta_$meta_name", 
      'type' => 'metatags_quick', 
      'module' => 'metatags_quick',
      'settings' => array('meta_name' => (isset($known_tags[$meta_name]['meta_name']) ? $known_tags[$meta_name]['meta_name'] : $meta_name)), 
      'cardinality' => 1,
    );
    $field = field_create_field($field);
    $field_id[$meta_name] = $field['id'];
    $meta_fields[$meta_name] = $field;
  }
  else {
    $field = $meta_fields[$meta_name];
  }
  
  // Do nothing if instance already exists.
  if (isset($field['bundles'][$entity_type])
    && in_array($bundle, $field['bundles'][$entity_type])) {
    return;
  }
  
  // Now create field instance attached to requested bundle
  $instance = array(
    'field_name' => $field['field_name'],
    'entity_type' => $entity_type,
    'bundle' => $bundle,
    'label' => $known_tags[$meta_name]['title'],
    'formatter' => 'metatags_quick_default',
    'widget' => array(
      'type' => $known_tags[$meta_name]['widget'],
      'weight' => 0,
    ),
  );
  if (isset($known_tags[$meta_name]['options'])) {
    $instance['settings']['options'] = $known_tags[$meta_name]['options'];
  }
  field_create_instance($instance);
}
