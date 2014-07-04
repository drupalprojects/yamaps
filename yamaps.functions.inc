<?php

/**
 * @file
 * Yandex Maps functions used across all components of the module.
 */

/**
 * Form for admin pages.
 *
 * Adds to any 'edit' form (block, field etc) standard "Yandex Maps" controls
 * and hidden fields.
 * The basic idea here is to provide method to add "Yandex Maps" widget
 * to any admin form.
 * @TODO: for now function is used for blocks only, support for other entities
 * will be added in the future.
 *
 * @param array $element
 *   Field form element.
 * @param array $form_state
 *   Field form state.
 * @param array $complete_form
 *   Complete form array.
 *
 * @return mixed
 *   Form array.
 */
function yamaps_admin_form_process($element, $form_state, $complete_form) {
  // Issue #1712292 fix.
  // TODO: Need to be deleted when Fields API fixed.
  if (isset($element['#entity'])) {
    $entity = $element['#entity'];
    if (isset($entity->vid) && !$element['#value']) {
      $entity_type = $element['#entity_type'];
      $field_name = $element['#field_name'];
      $instance = field_info_instance($entity_type, $field_name, $entity->type);
      $field = field_info_field($field_name);
      $default_value = field_get_default_value($entity_type, $entity, $field, $instance);
      $element['#default_value'] = $element['#value'] = reset($default_value);
    }
  }

  // Map information.
  $coords = isset($element['#value']['coords']) ? $element['#value']['coords'] : NULL;
  $coords_array = drupal_json_decode($coords);
  $type = isset($element['#value']['type']) ? $element['#value']['type'] : 'yandex#map';
  $placemarks = isset($element['#value']['placemarks']) ? $element['#value']['placemarks'] : NULL;
  $placemarks_array = drupal_json_decode($placemarks);
  $lines = isset($element['#value']['lines']) ? $element['#value']['lines'] : NULL;
  $lines_array = drupal_json_decode($lines);
  $polygons = isset($element['#value']['polygons']) ? $element['#value']['polygons'] : NULL;
  $polygons_array = drupal_json_decode($polygons);
  $routes = isset($element['#value']['routes']) ? $element['#value']['routes'] : NULL;
  $routes_array = drupal_json_decode($routes);

  // Unique map id.
  $id = drupal_html_id(implode('-', array(
    'ymap',
    $element['#delta'],
    'edit',
  )));

  // Unique map button id.
  $open_button_id = drupal_html_id(implode('-', array($id, 'open_button')));
  $remove_button_id = drupal_html_id(implode('-', array($id, 'remove_button')));
  $display_options = variable_get('yamaps_block_edit_display_options', array());
  $behaviors = array('scrollZoom', 'dblClickZoom', 'drag');

  if (isset($display_options['display_type']) && ($display_options['display_type'] == 'map_button')) {
    $element['open_map_button'] = array(
      '#type' => 'html_tag',
      '#tag' => 'div',
      '#value' => t($display_options['open_button_text']),
      '#attributes' => array(
        'id' => $open_button_id,
        'class' => 'open_yamap_button',
        'mapId' => $id,
      ),
    );
  }

  // Map initialization parameters.
  $map = array(
    'init' => array(
      'center' => $coords_array['center'],
      'zoom' => $coords_array['zoom'],
      'type' => $type,
      'behaviors' => $behaviors,
    ),
    'display_options' => array(
      'display_type' => isset($display_options['display_type']) ? $display_options['display_type'] : 'map',
      'open_button_text' => isset($display_options['open_button_text']) ? t($display_options['open_button_text']) : t('Open map'),
      'open_button_id' => $open_button_id,
      'remove_button_id' => $remove_button_id,
    ),
    'controls' => 1,
    'traffic' => 0,
    'placemarks' => $placemarks_array,
    'lines' => $lines_array,
    'polygons' => $polygons_array,
    'routes' => $routes_array,
    'edit' => TRUE,
  );

  // Map container.
  $element['map'] = array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#value' => '',
    '#attributes' => array(
      'id' => $id,
      'class' => array(
        'yamaps-map-container',
      ),
    ),
  );

  $element['coords'] = array(
    '#type' => 'textfield',
    '#title' => t('Coordinates'),
    '#default_value' => $coords,
    '#attributes' => array(
      'class' => array('field-yamaps-coords-' . $id),
      'style' => 'width: 100%;',
    ),
    '#description' => t('Search for object on the map to fill this field.'),
    '#required' => TRUE,
  );

  // Hidden elements to save map information.
  $element['type'] = array(
    '#type' => 'hidden',
    '#title' => t('Type'),
    '#default_value' => $type,
    '#attributes' => array('class' => array('field-yamaps-type-' . $id)),
  );

  $element['placemarks'] = array(
    '#type' => 'hidden',
    '#title' => t('Placemarks'),
    '#default_value' => $placemarks,
    '#attributes' => array('class' => array('field-yamaps-placemarks-' . $id)),
  );

  $element['lines'] = array(
    '#type' => 'hidden',
    '#title' => t('Lines'),
    '#default_value' => $lines,
    '#attributes' => array('class' => array('field-yamaps-lines-' . $id)),
  );

  $element['polygons'] = array(
    '#type' => 'hidden',
    '#title' => t('Polygons'),
    '#default_value' => $polygons,
    '#attributes' => array('class' => array('field-yamaps-polygons-' . $id)),
  );

  $element['routes'] = array(
    '#type' => 'hidden',
    '#title' => t('Routes'),
    '#default_value' => $routes,
    '#attributes' => array('class' => array('field-yamaps-routes-' . $id)),
  );

  // Map description.
  $element['#description'] = array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#value' => l(
      t('Terms of service «API Yandex.Maps»'),
      YAMAPS_LEGAL_AGREEMENT_URL,
      array('attributes' => array('target' => '_blank'))
    ),
    '#attributes' => array(
      'class' => 'yamaps-terms',
    ),
  );

  // Add information about this map to js.
  $element['#attached']['js'][] = array(
    'data' => array('yamaps' => array($id => $map)),
    'type' => 'setting',
  );
  // Load library.
  $element['#attached']['library'][] = array('yamaps', 'yamaps.full');

  return $element;
}

/**
 * Validate pixels or percentage value.
 *
 * @param array $element
 *   Field form element.
 * @param array $form_state
 *   Field form state.
 */
function yamaps_field_validate_pixels_percentage($element, &$form_state) {
  $value = $element['#value'];
  if (!preg_match('/^[1-9]{1}[0-9]*(px|%)$/', $value)) {
    form_error($element, t('%name must be a positive integer or percentage value.', array('%name' => $element['#title'])));
  }
}

/**
 * Get geo data for string.
 *
 * @param string $geolocation_string
 *   Name of geographical object.
 *
 * @return array|bool
 *   Geocoding array.
 */
function yamaps_geocoding($geolocation_string) {
  if (!$geolocation_string) {
    return FALSE;
  }

  // Preparing geocoding string.
  $query = array(
    'format' => 'json',
    'geocode' => $geolocation_string,
    'results' => 1,
  );

  $geolocation_request = file_get_contents(url(YAMAPS_GEOCODER_URL, array(
    'query' => $query,
    'absolute' => TRUE,
  )));

  $geolocation_request = json_decode($geolocation_request);

  if ($geolocation_request->response->GeoObjectCollection->metaDataProperty->GeocoderResponseMetaData->found > 0) {
    $map_center = $geolocation_request->response->GeoObjectCollection->featureMember[0]->GeoObject->Point->pos;
    $bounded_by = array();
    $bounded_by[] = array_map('yamaps_var_to_float', array_reverse(explode(' ', $geolocation_request->response->GeoObjectCollection->featureMember[0]->GeoObject->boundedBy->Envelope->lowerCorner)));
    $bounded_by[] = array_map('yamaps_var_to_float', array_reverse(explode(' ', $geolocation_request->response->GeoObjectCollection->featureMember[0]->GeoObject->boundedBy->Envelope->upperCorner)));

    return array(
      'map_center' => array_map('yamaps_var_to_float', array_reverse(explode(' ', $map_center))),
      'bounds' => $bounded_by,
    );
  }
  else {
    return FALSE;
  }
}

/**
 * Set variable to float.
 *
 * @param string $val
 *   Variable that need to be set to float.
 *
 * @return float
 *   Float value.
 */
function yamaps_var_to_float($val) {
  return (float) $val;
}

/**
 * Wraps each element in array with specified tag and implodes array.
 *
 * @param array $arr
 *   Item that need to be wrapped.
 * @param string $wrap_start
 *   Wrap start string.
 * @param string $wrap_end
 *   Wrap end string.
 *
 * @return string
 *   String with wrapped elements.
 */
function yamaps_wrap_items($arr, $wrap_start, $wrap_end) {
  $items = array();

  foreach ($arr as $item) {
    $items[] = $wrap_start . $item . $wrap_end;
  }

  return implode('', $items);
}

/**
 * Returns behaviors list.
 *
 * @return array
 *   Types of behaviours.
 */
function yamaps_get_behaviors_list() {
  return array(
    'clickZoom' => t('Click Zoom'),
    'scrollZoom' => t('Scroll Zoom'),
    'dblClickZoom' => t('Double click zoom'),
    'drag' => t('Click and drag'),
    'multiTouch' => t('Multi Touch support'),
    'ruler' => t('Ruler'),
    'rightMouseButtonMagnifier' => t('Right mouse button magnifier'),
  );
}

/**
 * Returns Map type list.
 *
 * @return array
 *   Types of maps.
 */
function yamaps_get_map_types() {
  return array(
    'yandex#map' => t('Schema'),
    'yandex#satellite' => t('Satellite'),
    'yandex#hybrid' => t('Hybrid'),
  );
}
