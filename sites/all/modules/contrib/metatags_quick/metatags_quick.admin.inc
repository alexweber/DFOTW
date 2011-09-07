<?php
function metatags_quick_admin_settings() {
  $current_settings = variable_get('metatags_quick_settings', _metatags_quick_settings_default());
  $form['front_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Front page meta tags'),
    '#collapsible' => TRUE,
  );
  $form['front_settings']['use_front'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use front page meta tags'),
    '#description' => t('Don\'t use that option if your front page is a fieldable entity (i.e. single node, taxonomy term etc.)'),
    '#default_value' => $current_settings['use_front'],
    '#return_value' => 1,
  );
  $fields = field_info_fields();
  $metatags_found = FALSE;
  foreach ($fields as $key => $field) {
    if ($field['module'] != 'metatags_quick') {
      continue;
    }
    $metatags_found = TRUE;
    $form_key = $field['settings']['meta_name'];
    $field_form = array(
      '#title' => $key,
      '#type' => 'textarea',
      '#default_value' => empty($current_settings['front_values'][$form_key]) ? '' : $current_settings['front_values'][$form_key],
    );
    $form['front_settings'][$form_key] = $field_form;
  }
  if (!$metatags_found) {
    $form['front_settings']['basic_init'] = array(
      '#markup' => t('No meta tags found in your installation'),
    );
    $form['front_settings']['basic_init_op'] = array(
      '#type' => 'submit',
      '#value' => t('Create basic meta tags'),
    );
  }
  $form['op'] = array(
    '#value' => t('Submit'),
    '#type' => 'submit', 
  );
  return $form;
}

function metatags_quick_admin_settings_submit($form, &$form_state) {
  $new_settings = array(
    'use_front' => !empty($form_state['values']['use_front']),
  );
  if (isset($form_state['values']['basic_init_op'])) {
    _metatags_quick_init_basic_fields();
  }
  else {
    foreach (field_info_fields() as $field_name => $field) {
      if ($field['module'] != 'metatags_quick') {
        continue;
      }
      $form_key = $field['settings']['meta_name'];
      $new_settings['front_values'][$form_key] = $form_state['values'][$form_key];
    }
  }
  variable_set('metatags_quick_settings', $new_settings);
  drupal_set_message(t('Meta tags (quick) settings saved'), 'status');
}