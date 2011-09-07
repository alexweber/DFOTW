<?php
/**
 * @file HeadJS auxiliary functions
 */

/**
 * Auxiliary function
 * Filters inline scripts
 */
function _headjs_filter_inline($val) {
  return (is_array($val) && isset($val['type']) && $val['type'] === 'inline');
}

/**
 * Auxiliary function
 * Filters script includes
 */
function _headjs_filter_script($val) {
  return !is_numeric($val);
}

/**
 * Auxiliary function
 * Filters scripts in header scope
 */
function _headjs_filter_head($val) {
  return (is_array($val) && isset($val['scope']) && $val['scope'] === 'header');
}

/**
 * Auxiliary function
 * Filters scripts in footer scope
 */
function _headjs_filter_foot($val) {
  return (is_array($val) && isset($val['scope']) && $val['scope'] !== 'header');
}

/**
 * Auxiliary function
 * Wrapper for file_create_url() to ease treating inline scripts
 */
function _headjs_create_url($val) {
  return (is_numeric($val) ? $val : file_create_url($val));
}

/**
 * Get the path of head.min.js file
 * @return string
 */
function headjs_get_path() {
  static $path;

  if (!$path) {
    // decide which head.js to use; the loader-only or loader + tools & get file path
    $headjs_version = (variable_get('headjs_version', 'full') === 'full') ? HEADJS_FULL : HEADJS_LOADER;
    $path = headjs_path() . $headjs_version;
  }

  return $path;
}

/**
 * Get the path of head.js library
 * Integrates with Libraries API, if present
 *
 * @param bool
 * @return string
 */
function headjs_path($refresh = FALSE) {
  static $path;

  if ($refresh === TRUE || !isset($path)) {
    $path = function_exists('libraries_get_path') ? libraries_get_path('headjs') : 'sites/all/libraries/headjs';
  }

  if (file_exists("$path/dist/head.min.js")) {
    $path .= "/dist/";
  } else if ($path) {
    $path .= '/';
  }

  return $path;
}

/**
 * Centralize warning message here, since it's called more than once from different places
 */
function _headjs_warning_message() {
  drupal_set_message(t('Please <a href="http://headjs.com/#download">download</a> and copy the head.js library to your "/sites/all/libraries/headjs" folder'), 'error', FALSE);
}

/**
 * Return the version of head.js that is currently installed.
 * This can be used by other modules' hook_requirements() to ensure that the proper version of head.js is installed.
 */
function headjs_get_version() {
  $version = 0;
  $headjs = file_get_contents(headjs_get_path());

  // newer versions have different spacing in the docblock
  if (preg_match('#    Version     ([0-9\.a-z]+)#', $headjs, $matches)) {
    $version = $matches[1];
  } elseif (preg_match('#	Version		([0-9\.a-z]+)#', $headjs, $matches)) {
    $version = $matches[1];
  }

  return $version;
}

/**
 * Replace inline scripts with calls to head.js()
 *
 * @param string $content
 * @param array $headscripts
 */
function headjs_replace(&$content, &$headscripts) {
  $matches = array();
  preg_match('#<script .+?</script>#s', $content, $matches);

  foreach ($matches as $match) {
    $matches_scriptname = array();
    if (preg_match('#src="(.+?)"#', $match, $matches_scriptname)) {
      $headscripts[] = $matches_scriptname[1];
      $content = str_replace($match, '', $content);
    }
  }
}

/*
 * Integration with "Do"
 * http://drupal.org/project/do
 */

/**
 * Enable HeadJS via Do
 */
function _headjs_enable() {
  variable_set('headjs_enable', TRUE);
  drupal_set_message(t('HeadJS enabled!'));
  return array(
    'ok' => TRUE,
    'redirect' => TRUE
  );
}

/**
 * Disable HeadJS via Do
 */
function _headjs_disable() {
  variable_set('headjs_enable', FALSE);
  drupal_set_message(t('HeadJS disabled!'));
  return array(
    'ok' => TRUE,
    'redirect' => TRUE
  );
}
