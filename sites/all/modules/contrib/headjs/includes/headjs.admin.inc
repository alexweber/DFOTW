<?php
/**
 * @file HeadJS Admin Functions
 */

/**
 * Admin settings form
 */
function headjs_settings() {
  $form = array();

  // Clear the javascript cache when settings are updated
  $form['#submit'][] = 'drupal_clear_js_cache';

  $form['headjs_enable'] = array(
    '#type' => 'checkbox',
    '#name' => 'headjs_enable',
    '#title' => t('Enable head.js'),
    '#default_value' => variable_get('headjs_enable', TRUE),
    '#weight' => '-3',
  );

  $form['headjs_version'] = array(
    '#type' => 'radios',
    '#title' => t('Choose head.js version'),
    '#options' => array(
      'loader' => t('Loader only'),
      'full' => t('Loader + Tools'),
    ),
    '#description' => t('The loader option only includes only the head.js loader for parallel asynchronous script downloading. The "full" version includes a CSS Modernizer, HTML5 Enabler, Screen size detector, CSS Router, Browser Detector and JavaScript feature detection. For more information, please see the <a href="http://headjs.com">head.js website</a>.'),
    '#default_value' => variable_get('headjs_version', 'loader'),
    '#weight' => '-2',
  );

  $form['headjs_embed'] = array(
    '#type' => 'checkbox',
    '#name' => 'headjs_embed',
    '#title' => t('Embed script instead of including'),
    '#default_value' => variable_get('headjs_embed', FALSE),
    '#description' => t('Embed head.js inline instead of including it externally for an even greater performance boost.'),
    '#weight' => '1'
  );

  return system_settings_form($form);
}