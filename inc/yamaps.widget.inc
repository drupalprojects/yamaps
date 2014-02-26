<?php

/**
 * @file
 * Yandex.Maps field widget.
 */

/**
 * Implements hook_field_widget_info().
 */
function yamaps_field_widget_info() {
  return array(
    'yamaps_field' => array(
      'label' => t('Yandex map'),
      'field types' => array('field_yamaps'),
    ),
  );
}

/**
 * Helper function for cleanup values.
 */
function _yamaps_cleanup_values(&$value) {
  $value['coords'] = isset($value['coords']) ? $value['coords'] : NULL;
  $value['type'] = isset($value['type']) ? $value['type'] : 'yandex#map';
  $value['placemarks'] = isset($value['placemarks']) ? $value['placemarks'] : NULL;
  $value['lines'] = isset($value['lines']) ? $value['lines'] : NULL;
  $value['polygons'] = isset($value['polygons']) ? $value['polygons'] : NULL;
  $value['routes'] = isset($value['routes']) ? $value['routes'] : NULL;
}

/**
 * Helper function for cleanup values for json.
 */
function _yamaps_format_values_to_js($clean_values) {
  return array(
    'coords' => drupal_json_decode($clean_values['coords']),
    'type' => $clean_values['type'],
    'placemarks' => drupal_json_decode($clean_values['placemarks']),
    'lines' => drupal_json_decode($clean_values['lines']),
    'polygons' => drupal_json_decode($clean_values['polygons']),
    'routes' => drupal_json_decode($clean_values['routes']),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function yamaps_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  // Unique map id.
  $id = drupal_html_id(implode('-', array('ymap', $element['#field_name'], $element['#delta'], 'edit')));
  $value = isset($items[$delta]) ? $items[$delta] : array();

  $element += array(
    '#type' => $instance['widget']['type'],
    '#default_value' => $value,
  );

  // Set value after "Add more" button clicked
  if (isset($form_state['values'][$field['field_name']][$langcode][$delta])) {
    $value = $form_state['values'][$field['field_name']][$langcode][$delta];
  }

  // Map information.
  _yamaps_cleanup_values($value);
  $default_js = _yamaps_format_values_to_js($value);

  // Issue #1712292 fix.
  // Display default position on edit form
  if (!$element['#default_value']) {
    if (isset($element['#entity']) && $entity = $element['#entity']) {
      $default_value = field_get_default_value($element['#entity_type'], $entity, $field, $instance);
      $default_value = reset($default_value);
      _yamaps_cleanup_values($default_value);
      $default_js = _yamaps_format_values_to_js($default_value);
    }
  }

  // Map initialization parameters.
  $map = array(
    'init' => array(
      'center' => $default_js['coords']['center'],
      'zoom' => $default_js['coords']['zoom'],
      'type' => $default_js['type'],
      'behaviors' => array('scrollZoom', 'dblClickZoom', 'drag'),
    ),
    'controls' => 1,
    'traffic' => 0,
    'placemarks' => $default_js['placemarks'],
    'lines' => $default_js['lines'],
    'polygons' => $default_js['polygons'],
    'routes' => $default_js['routes'],
    'edit' => TRUE,
  );

  // Add information about this map to js.
  drupal_add_js(array('yamaps' => array($id => $map)), 'setting');
  // Load library.
  drupal_add_library('yamaps', 'yamaps.full');

  $element['remove_yamap_field_item'] = array(
    '#name' => $element['#field_name'] . '_' . $element['#delta'] . '_remove_button',
    '#type' => 'markup',
    '#markup' => l(t('Remove'), '', array('attributes' => array('id' => 'remove_yamap_field_item', 'class' => array('button','delta-' . $element['#delta'])))),
  );


  // Map container.
  $element['map']['#markup'] = theme('html_tag', array(
    'element' => array(
      '#tag' => 'div',
      '#attributes' => array(
        'style' => 'width:100%; height:400px;',
        'id' => $id,
      ),
      '#value' => '',
    ),
  ));
  // Hidden elements to saving map information.
  $element['coords'] = array(
    '#type' => 'hidden',
    '#title' => t('Coordinates'),
    '#default_value' => $value['coords'],
    '#required' => $element['#required'],
    '#attributes' => array('class' => array('field-yamaps-coords-' . $id)),
  );
  $element['type'] = array(
    '#type' => 'hidden',
    '#title' => t('Type'),
    '#default_value' => $value['type'],
    '#attributes' => array('class' => array('field-yamaps-type-' . $id)),
  );
  $element['placemarks'] = array(
    '#type' => 'hidden',
    '#title' => t('Placemarks'),
    '#default_value' => $value['placemarks'],
    '#attributes' => array('class' => array('field-yamaps-placemarks-' . $id)),
  );
  $element['lines'] = array(
    '#type' => 'hidden',
    '#title' => t('Lines'),
    '#default_value' => $value['lines'],
    '#attributes' => array('class' => array('field-yamaps-lines-' . $id)),
  );
  $element['polygons'] = array(
    '#type' => 'hidden',
    '#title' => t('Polygons'),
    '#default_value' => $value['polygons'],
    '#attributes' => array('class' => array('field-yamaps-polygons-' . $id)),
  );
  $element['routes'] = array(
    '#type' => 'hidden',
    '#title' => t('Routes'),
    '#default_value' => $value['routes'],
    '#attributes' => array('class' => array('field-yamaps-routes-' . $id)),
  );

  // Map description.
  $element['#description'] = '<div class="yamaps-terms">' . l(
    t('Terms of service «API Yandex.Maps»'),
    'http://legal.yandex.ru/maps_api/',
    array('attributes' => array('target' => '_blank'))
  ) . '</div>';

  return $element;
}
