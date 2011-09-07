<?php
/**
 * @file
 * dfotw.context.inc
 */

/**
 * Implementation of hook_context_default_contexts().
 */
function dfotw_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'contact';
  $context->description = 'Contact form';
  $context->tag = '';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'contact' => 'contact',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'dfotw_tweaks-dfotw_user_submission' => array(
          'module' => 'dfotw_tweaks',
          'delta' => 'dfotw_user_submission',
          'region' => 'content_top',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Contact form');
  $export['contact'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'dfotw';
  $context->description = 'Sitewide context';
  $context->tag = '';
  $context->conditions = array(
    'sitewide' => array(
      'values' => array(
        1 => 1,
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'system-main-menu' => array(
          'module' => 'system',
          'delta' => 'main-menu',
          'region' => 'header_menu',
          'weight' => '-10',
        ),
        'system-powered-by' => array(
          'module' => 'system',
          'delta' => 'powered-by',
          'region' => 'footer_menu',
          'weight' => '-10',
        ),
        'dfotw_tweaks-dfotw_share_global' => array(
          'module' => 'dfotw_tweaks',
          'delta' => 'dfotw_share_global',
          'region' => 'sidebar_second',
          'weight' => '-12',
        ),
        'search-form' => array(
          'module' => 'search',
          'delta' => 'form',
          'region' => 'sidebar_second',
          'weight' => '-11',
        ),
        'dfotw_tweaks-dfotw_recent_content' => array(
          'module' => 'dfotw_tweaks',
          'delta' => 'dfotw_recent_content',
          'region' => 'sidebar_second',
          'weight' => '-10',
        ),
        'dfotw_tweaks-dfotw_browse_version' => array(
          'module' => 'dfotw_tweaks',
          'delta' => 'dfotw_browse_version',
          'region' => 'sidebar_second',
          'weight' => '-9',
        ),
        'comment-recent' => array(
          'module' => 'comment',
          'delta' => 'recent',
          'region' => 'sidebar_second',
          'weight' => '-8',
        ),
        'system-main' => array(
          'module' => 'system',
          'delta' => 'main',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Sitewide context');
  $export['dfotw'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'function';
  $context->description = 'Function Node Pages';
  $context->tag = '';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'function' => 'function',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'dfotw_tweaks-dfotw_share_post' => array(
          'module' => 'dfotw_tweaks',
          'delta' => 'dfotw_share_post',
          'region' => 'content_top',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Function Node Pages');
  $export['function'] = $context;

  return $export;
}
