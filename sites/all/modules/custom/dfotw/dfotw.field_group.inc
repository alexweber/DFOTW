<?php
/**
 * @file
 * dfotw.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dfotw_field_group_info() {
  $export = array();

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_function_info|node|function|form';
  $field_group->group_name = 'group_function_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'function';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Function Information',
    'weight' => '4',
    'children' => array(
      0 => 'field_function_docs',
      1 => 'field_function_version',
      2 => 'field_function_desc',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_function_info|node|function|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_function_layout|node|function|form';
  $field_group->group_name = 'group_function_layout';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'function';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Page Layout',
    'weight' => '3',
    'children' => array(
      0 => 'body',
      1 => 'field_function_intro',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_function_layout|node|function|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_function_meta|node|function|form';
  $field_group->group_name = 'group_function_meta';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'function';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Meta Tags',
    'weight' => '6',
    'children' => array(
      0 => 'meta_keywords',
      1 => 'meta_description',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_function_meta|node|function|form'] = $field_group;

  $field_group = new stdClass;
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_function_usage|node|function|form';
  $field_group->group_name = 'group_function_usage';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'function';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Function Usage',
    'weight' => '5',
    'children' => array(
      0 => 'field_function_example',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Function Usage',
      'instance_settings' => array(
        'required_fields' => 0,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'closed',
    ),
  );
  $export['group_function_usage|node|function|form'] = $field_group;

  return $export;
}
