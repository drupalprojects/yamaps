<?php

/**
 * Implementation of hook_views_plugins().
 */
function views_yandexmaps_views_plugins() {
  return array(
    'module' => 'views_yandexmaps',
    'style' => array(
      'default_map' => array(
        'title' => t('Yandex Maps'),
        'help' => t('Display Yandex.Map'),
        'handler' => 'views_yandexmaps_plugin_style_default_map',
        'path' => drupal_get_path('module', 'views_yandexmaps') . '/handlers',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses grouping' => FALSE,
        'even empty' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}