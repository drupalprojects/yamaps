<?php

/**
 * @file
 * Yandex.Maps block.
 */

/**
 * Implements hook_block_info().
 */
function yamaps_block_info() {
  $blocks[YAMAPS_DEFAULT_BLOCK_DELTA] = array(
    'info' => t('Yandex.Map #1'),
    'cache' => DRUPAL_NO_CACHE,
  );

  $blocks_amount = variable_get('yamaps_blocks_amount', YAMAPS_DEFAULT_BLOCK_AMOUNT);
  ++$blocks_amount;
  for ($block_number = 2; $block_number <= $blocks_amount; $block_number++) {
    $blocks[YAMAPS_DEFAULT_BLOCK_DELTA . '_' . $block_number] = array(
      'info' => t('Yandex.Map #!block_number', array('!block_number' => $block_number)),
      'cache' => DRUPAL_NO_CACHE,
    );
  }
  return $blocks;
}

/**
 * Implements hook_block_configure().
 * 
 * @see yamaps_field_process()
 */
function yamaps_block_configure($delta = YAMAPS_DEFAULT_BLOCK_DELTA) {

  $form[$delta . '_block_controls'] = array(
    '#title' => t('Show controls'),
    '#type' => 'checkbox',
    '#default_value' => variable_get($delta . '_block_controls', TRUE),
  );
  $form[$delta . '_block_traffic'] = array(
    '#title' => t('Show traffic'),
    '#type' => 'checkbox',
    '#default_value' => variable_get($delta . '_block_traffic', FALSE),
  );
  $form[$delta . '_block_behaviors'] = array(
    '#title' => t('Mouse event settings'),
    '#type' => 'checkboxes',
    '#options' => yamaps_get_behaviors_list(),
    '#default_value' => variable_get($delta . '_block_behaviors', array()),
  );
  $form[$delta . '_block_width'] = array(
    '#title' => t('Map width'),
    '#field_suffix' => ' ' . t('in pixels or in percentage'),
    '#type' => 'textfield',
    '#default_value' => variable_get($delta . '_block_width', '168px'),
    '#size' => 5,
    '#element_validate' => array('yamaps_field_validate_pixels_percentage'),
    '#required' => TRUE,
  );
  $form[$delta . '_block_height'] = array(
    '#title' => t('Map height'),
    '#field_suffix' => ' ' . t('in pixels or in percentage'),
    '#type' => 'textfield',
    '#default_value' => variable_get($delta . '_block_height', '200px'),
    '#size' => 5,
    '#element_validate' => array('yamaps_field_validate_pixels_percentage'),
    '#required' => TRUE,
  );

  // Add elements from default field edit form.
  $element = array(
    '#delta' => 'block-' . $delta,
    '#required' => FALSE,
    '#value' => array(
      'coords' => variable_get($delta . '_block_coords', NULL),
      'type' => variable_get($delta . '_block_type', 'yandex#map'),
      'placemarks' => variable_get($delta . '_block_placemarks', NULL),
      'lines' => variable_get($delta . '_block_lines', NULL),
      'polygons' => variable_get($delta . '_block_polygons', NULL),
      'routes' => variable_get($delta . '_block_routes', NULL),
    ),
  );
  $form += yamaps_field_process($element, NULL, NULL);

  return $form;
}

/**
 * Implements hook_block_save().
 */
function yamaps_block_save($delta = YAMAPS_DEFAULT_BLOCK_DELTA, $edit = array()) {
  variable_set($delta . '_block_controls', $edit[$delta . '_block_controls']);
  variable_set($delta . '_block_traffic', $edit[$delta . '_block_traffic']);
  variable_set($delta . '_block_behaviors', $edit[$delta . '_block_behaviors']);
  variable_set($delta . '_block_width', $edit[$delta . '_block_width']);
  variable_set($delta . '_block_height', $edit[$delta . '_block_height']);
  variable_set($delta . '_block_coords', $edit['coords']);
  variable_set($delta . '_block_type', $edit['type']);
  variable_set($delta . '_block_placemarks', $edit['placemarks']);
  variable_set($delta . '_block_lines', $edit['lines']);
  variable_set($delta . '_block_polygons', $edit['polygons']);
  variable_set($delta . '_block_routes', $edit['routes']);
}

/**
 * Implements hook_block_view().
 */
function yamaps_block_view($delta = YAMAPS_DEFAULT_BLOCK_DELTA) {
  $block = array();
  $block['subject'] = t('Yandex.Map');
  $block['content'] = yamaps_block_content($delta);
  return $block;
}

/**
 * Returns block content.
 *
 * @see yamaps_block_view()
 */
function yamaps_block_content($delta) {
  // Map id.
  $id = drupal_html_id(implode('-', array('ymap', 'block', $delta)));

  // Map information.
  $coords = drupal_json_decode(variable_get($delta . '_block_coords', NULL));
  $width = variable_get($delta . '_block_width', '168px');
  $height = variable_get($delta . '_block_height', '200px');

  // Map initialization parameters.
  $map = array(
    'init' => array(
      'center' => $coords['center'],
      'zoom' => $coords['zoom'],
      'type' => variable_get($delta . '_block_type', 'yandex#map'),
      'behaviors' => array_values(array_filter(variable_get($delta . '_block_behaviors', array()))),
    ),
    'controls' => variable_get($delta . '_block_controls', TRUE),
    'traffic' => variable_get($delta . '_block_traffic', FALSE),
    'placemarks' => drupal_json_decode(variable_get($delta . '_block_placemarks', NULL)),
    'lines' => drupal_json_decode(variable_get($delta . '_block_lines', NULL)),
    'polygons' => drupal_json_decode(variable_get($delta . '_block_polygons', NULL)),
    'routes' => drupal_json_decode(variable_get($delta . '_block_routes', NULL)),
    'edit' => FALSE,
  );

  // Adding map to js.
  drupal_add_js(array('yamaps' => array($id => $map)), 'setting');
  // Load library.
  drupal_add_library('yamaps', 'yamaps.full');

  // Return map container div.
  return array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#attributes' => array(
      'id' => $id,
      'style' => 'width:' . $width . ';height:' . $height,
    ),
    '#value' => '',
  );
}
