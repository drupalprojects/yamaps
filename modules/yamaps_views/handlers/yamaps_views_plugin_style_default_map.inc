<?php

/**
 * @file
 * Class yamaps_views_plugin_style_default_map.
 */
class yamaps_views_plugin_style_default_map extends views_plugin_style {
  const PLACEMARK_TITLE = 'iconContent';
  const PLACEMARK_BALLON_HEADER = 'balloonContentHeader';
  const PLACEMARK_BALLON_BODY = 'balloonContentBody';
  const PLACEMARK_DEFAULT_FIELD = '<default>';
  const PLACEMARK_NONE_FIELD = '<none>';


  /**
   * Default settings for map.
   *
   * @return array
   *   Default values.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['placemarks'] = array('default' => '');
    $options['lines'] = array('default' => '');
    $options['polygons'] = array('default' => '');
    $options['routes'] = array('default' => '');
    $options['zoom'] = array('default' => '6');
    $options['coords'] = array('default' => '');
    $options['yamaps_center_options'] = array(
      'default' => array(
        'map_center_type' => 'geolocation',
        'map_center_geolocation' => '',
      ),
    );
    $options['yandex_map_field'] = array('default' => '');
    $options['placemark_title'] = array('default' => $this::PLACEMARK_DEFAULT_FIELD);
    $options['baloon_title'] = array('default' => $this::PLACEMARK_DEFAULT_FIELD);
    $options['baloon_body'] = array('default' => $this::PLACEMARK_DEFAULT_FIELD);
    $options['type'] = array('default' => 'yandex#map');
    $options['map_center'] = array('default' => '');
    $options['map_grouping_cat'] = array('default' => 'standart');

    return $options;
  }

  /**
   * Views Yandex Maps settings form.
   *
   * @return array
   *   Form array.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $fields = $this->getFields();
    $yandex_fields = $this->getYandexMapsFields();

    $form['yandex_map_field'] = array(
      '#title' => t('Yandex Map Field'),
      '#description' => t('Choose Yandex Maps field. Add if views fields this field for the first.'),
      '#type' => 'select',
      '#options' => $yandex_fields,
      '#required' => TRUE,
      '#default_value' => $this->options['yandex_map_field'],
    );
    $form['yamaps_center_options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Map center'),
    );
    $form['yamaps_center_options']['map_center_type'] = array(
      '#type' => 'radios',
      '#title' => t('Choose map center type'),
      '#options' => array(
        'geolocation' => t('Geolocation.'),
        'mini_map' => t('Choose on map.'),
      ),
      '#default_value' => $this->options['yamaps_center_options']['map_center_type'],
      '#required' => FALSE,
      '#description' => t('Type of map displaying.'),
    );

    $form['yamaps_center_options']['map_center_geolocation'] = array(
      '#title' => t('Map center geolocation'),
      '#description' => t('Please enter place on whitch map will be centered.'),
      '#type' => 'textfield',
      '#default_value' => $this->options['yamaps_center_options']['map_center_geolocation'],
      '#size' => 40,
      '#states' => array(
        'visible' => array(
          ':input[name="style_options[yamaps_center_options][map_center_type]"]' => array('value' => 'geolocation'),
        ),
      ),
    );
    $form['yamaps_center_options']['zoom'] = array(
      '#title' => t('Zoom'),
      '#type' => 'select',
      '#description' => t('Zoom of map'),
      '#options' => range(1, 15),
      '#states' => array(
        'visible' => array(
          ':input[name="style_options[yamaps_center_options][map_center_type]"]' => array('value' => 'geolocation'),
        ),
      ),
      '#default_value' => $this->options['yamaps_center_options']['zoom'],
    );
    $prepared_params = _yamaps_format_values_to_js($this->options);
    // Map initialization parameters.
    $map = array(
      'init' => array(
        'type' => $this->options['type'] ? $this->options['type'] : 'yandex#map',
        'behaviors' => array('scrollZoom', 'dblClickZoom', 'drag'),
      ),
      'display_options' => array(
        'display_type' => 'map',
      ),
      'controls' => 1,
      'traffic' => 0,
      'placemarks' => $prepared_params['placemarks'],
      'lines' => $prepared_params['lines'],
      'polygons' => $prepared_params['polygons'],
      'routes' => $prepared_params['routes'],
      'edit' => TRUE,
    );
    if ($this->options['coords']) {
      $coords = json_decode($this->options['coords']);
      $map['init']['center'] = $coords->center;
      $map['init']['zoom'] = $coords->zoom;
    }
    $id = drupal_html_id(implode('-', array(
      $this->plugin_name,
      $this->view->name,
      $this->view->current_display,
      'style_options_form',
    )));
    // Map container.
    $form['yamaps_center_options']['map'] = array(
      '#type' => 'html_tag',
      '#tag' => 'div',
      '#value' => '',
      '#description' => t('Map view will be used when "Choose on map." radio is active'),
      '#attributes' => array(
        'style' => ' width: ' . $this->options['width'] . '; height:' . $this->options['height'],
        'id' => $id,
        'class' => array(
          'yamaps-map-container',
        ),
      ),
      '#states' => array(
        'visible' => array(
          ':input[name="style_options[yamaps_center_options][map_center_type]"]' => array('value' => 'mini_map'),
        ),
      ),
    );
    $form['coords'] = array(
      '#type' => 'textfield',
      '#title' => t('Coordinates'),
      '#default_value' => $this->options['coords'],
      '#attributes' => array(
        'class' => array('field-yamaps-coords-' . $id),
        'style' => 'width: 100%;',
      ),
      '#states' => array(
        'visible' => array(
          ':input[name="style_options[yamaps_center_options][map_center_type]"]' => array('value' => 'mini_map'),
        ),
      ),
      '#description' => t('Search for object on the map to fill this field or leave it blank (if field is not required).'),
    );

    // Hidden elements to save map information.
    $form['type'] = array(
      '#type' => 'hidden',
      '#title' => t('Type'),
      '#default_value' => $this->options['type'],
      '#attributes' => array('class' => array('field-yamaps-type-' . $id)),
    );

    // Hidden elements to saving map information.
    $form['placemarks'] = array(
      '#type' => 'hidden',
      '#default_value' => $this->options['placemarks'],
      '#attributes' => array('class' => array('field-yamaps-placemarks-' . $id)),
    );

    $form['lines'] = array(
      '#type' => 'hidden',
      '#default_value' => $this->options['lines'],
      '#attributes' => array('class' => array('field-yamaps-lines-' . $id)),
    );

    $form['polygons'] = array(
      '#type' => 'hidden',
      '#default_value' => $this->options['polygons'],
      '#attributes' => array('class' => array('field-yamaps-polygons-' . $id)),
    );

    $form['routes'] = array(
      '#type' => 'hidden',
      '#default_value' => $this->options['routes'],
      '#attributes' => array('class' => array('field-yamaps-routes-' . $id)),
    );

    $form['#attached']['js'][] = array(
      'data' => array('yamaps' => array($id => $map)),
      'type' => 'setting',
    );

    // Load library.
    $form['#attached']['library'][] = array('yamaps', 'yamaps.full');

    /*
    $form['yamaps_display_options']['modal_type'] = array(
      '#type' => 'radios',
      '#title' => t('How display map for admins'),
      '#options' => $modal_type,
      '#default_value' => $this->options['yamaps_display_options']['display_type'],

      '#description' => t('Type of map displaying.'),
      '#states' => array(
        'visible' => array(
          ':input[name="style_options[yamaps_display_options][display_type]"]' => array('value' => 'map_modal'),
        ),
      ),
    );*/
    $form['placemark_title'] = array(
      '#title' => t('Placemark title'),
      '#type' => 'select',
      '#options' => $fields,
      '#default_value' => $this->options['placemark_title'],
    );
    $form['baloon_title'] = array(
      '#title' => t('Baloon title'),
      '#type' => 'select',
      '#options' => $fields,
      '#default_value' => $this->options['baloon_title'],
    );

    $form['baloon_body'] = array(
      '#title' => t('Balloon body Field'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $fields,
      '#default_value' => $this->options['baloon_body'],
    );
  }

  /**
   * Preparing body.
   *
   * @return string
   *  Prepared body.
   */
  function prepareBody($body_array) {
    $output = '<div class="balloon-inner">';
    foreach ($body_array as $key => $val) {
      $output .= '<span class="' . $key . '">' . $val . '</span>';
    }
    $output .= '</div>';
    return $output;
  }

  /**
   * Return TRUE if this style also uses fields.
   *
   * @return bool
   *   Boolean value.
   */
  function uses_fields() {
    $fields = parent::uses_fields();
    $yandexmap_field = '';
    if (isset($this->view->field)) {
      foreach ($this->view->field as $field_name => $field_handler) {
        if (isset($field_handler->field_info) && $field_handler->field_info['type'] == 'field_yamaps') {
          $yandexmap_field = $field_name;
          break;
        }
      }
    }
    // Excluded field.
    if (isset($this->view->field[$yandexmap_field])) {
      $this->view->field[$yandexmap_field]->options['exclude'] = TRUE;
    }
    return $fields;
  }

  /**
   * Validation.
   *
   * @return array
   *   Errors list.
   */
  function validate() {
    $errors = parent::validate();

    // Checking of existanse of pager settings.
    if ($this->display->handler->use_pager()) {
      $errors[] = t('The Yandex Maps style cannot be used with a pager. Disable the "Use pager" option for this display.');
    }

    // Checking for existans of yandex map field.
    if (empty($this->options['yandex_map_field']) || $this->options['yandex_map_field'] == '<none>') {
      $errors[] = t('The Yandex Maps style cannot be used with a pager. Please add "Yandex Maps" field and chose it in views settings form.');
    }

    return $errors;
  }

  /**
   * Preparing data for render.
   */
  function pre_render($result) {

  }

  /**
   * Rendering data.
   *
   * @return
   *   Rendered string.
   */
  function render() {
    $objects = array();
    $prepared_lines = array();
    $prepared_polygons = array();
    $prepared_route = array();

    if (isset($this->view->live_preview) && $this->view->live_preview) {
      return t("Selected view format doesn't support live preview.");
    }

    $this->render_fields($this->view->result);

    $yandexmap_field_name = 'field_' . $this->options['yandex_map_field'];
    // Excluded field.
    $this->view->field[$yandexmap_field_name]->options['exclude'] = TRUE;
    $yandexmap_field_settings = $this->view->field[$this->options['yandex_map_field']]->options['settings'];
    $yandexmap_field_type = $this->view->field[$this->options['yandex_map_field']]->options['type'];

    foreach ($this->view->result as $row_index => $row) {
      // $rendered_row = $this->row_plugin->render($row);
      // Fix yandex cart.
      foreach ($row->$yandexmap_field_name as $yandexmap_field_coords) {
        // Preparing placemarks.
        $decoded_placemarks = drupal_json_decode($yandexmap_field_coords['raw']['placemarks']);

        if (is_array($decoded_placemarks)) {
          foreach ($decoded_placemarks as $placemark) {
            // $marker_title = '';
            if (isset($this->options['placemark_title']) && $this->options['placemark_title'] !== $this::PLACEMARK_DEFAULT_FIELD) {
              // Prepare placemark title.
              $marker_title = $this->preparePlacemarkTitle($row_index);
              $placemark['params'][$this::PLACEMARK_TITLE] = $marker_title;
            }
            // Prepare Baloon title.
            if ($this->options['baloon_title'] && $this->options['baloon_title'] !== $this::PLACEMARK_DEFAULT_FIELD) {
              $placemark['params'][$this::PLACEMARK_BALLON_HEADER] = $this->rendered_fields[$row_index][$this->options['baloon_title']];
            }
            // Prepare Baloon body.
            if (is_array($this->options['baloon_body'])) {
              foreach ($this->options['baloon_body'] as $bkey => $bval) {
                //$this->rendered_fields[$row_index][$this->options['placemark_title']];
                $ballon_body[] = $this->get_field($row_index, $bval);
              }
              $placemark['params'][$this::PLACEMARK_BALLON_BODY] = $this->prepareBody($ballon_body);
            }

            $this->view->row_index = $row_index;
            unset($ballon_body);
            $objects[] = $placemark;
          }
        }

        // Preparing lines.
        $decoded_lines = drupal_json_decode($yandexmap_field_coords['raw']['lines']);
        if (is_array($decoded_lines)) {
          foreach ($decoded_lines as $lines) {
            $prepared_lines[] = $lines;
          }
        }

        // Preparing polygons.
        $decoded_polygons = drupal_json_decode($yandexmap_field_coords['raw']['polygons']);
        if (is_array($decoded_polygons)) {
          foreach ($decoded_polygons as $polygons) {
            $prepared_polygons[] = $polygons;
          }
        }
        // Preparing routes.
        $decoded_routes = drupal_json_decode($yandexmap_field_coords['raw']['routes']);
        if (is_array($decoded_routes)) {
          foreach ($decoded_routes as $route) {
            $prepared_route[] = $route;
          }
        }
      }
    }

    unset($this->view->row_index);

    $array_of_unique_params = array(
      $this->plugin_name,
      $this->view->name,
      $this->view->current_display,
    );

    if (isset($this->view->dom_id)) {
      $array_of_unique_params[] = $this->view->dom_id;
    }
    // Unique map id.
    $id = drupal_html_id(implode('-', $array_of_unique_params));
    $open_button_id = drupal_html_id(implode('-', array($id, 'open_button')));

    $prepared_params = _yamaps_format_values_to_js($this->options);

    // Merging placemark.
    if (is_array($prepared_params['lines'])) {
      foreach ($prepared_params['placemarks'] as $p) {
        $objects[] = $p;
      }
    }

    // Merging lines.
    if (is_array($prepared_params['lines'])) {
      foreach ($prepared_params['lines'] as $lines) {
        $prepared_lines[] = $lines;
      }
    }

    // Merging polygons.
    if (is_array($prepared_params['polygons'])) {
      foreach ($prepared_params['polygons'] as $polygon) {
        $prepared_polygons[] = $polygon;
      }
    }
    // Merging routes.
    if (is_array($prepared_params['routes'])) {
      foreach ($prepared_params['routes'] as $route) {
        $prepared_route[] = $route;
      }
    }

    switch ($this->options['yamaps_center_options']['map_center_type']) {
      case 'geolocation':
        if (($this->options['yamaps_center_options']['map_center_type'] == 'geolocation')) {
          $parameters = yamaps_geocoding($this->options['yamaps_center_options']['map_center_geolocation']);
        }
        if (isset($parameters) && $parameters !== FALSE) {
          $map['init']['center'] = $parameters['map_center'];
          $map['init']['zoom'] = ++$this->options['yamaps_center_options']['zoom'];
          // $map['init']['bounds'] = $parameters['bounds'];
        }
        /**
         * if (!empty($this->options['map_center'])) {
         * $map['init']['center_geocode'] = $this->options['map_center'];
         *}
         */
        break;
      case 'mini_map':
        if ($this->options['coords']) {
          $map['init']['center'] = $prepared_params['coords']['center'];
          $map['init']['zoom'] = $prepared_params['coords']['zoom'];
        }
        break;
    }

    $views_output = array();

    // Adding map to js.
    $views_output['#attached']['js'][] = array(
      'data' => array('yamaps' => array($id => $map)),
      'type' => 'setting',
    );

    // Load library.
    $views_output['#attached']['library'][] = array('yamaps', 'yamaps.full');

    if (isset($yandexmap_field_settings['yamaps_display_options']['display_type']) && $yandexmap_field_settings['yamaps_display_options']['display_type'] == 'map_button') {
      $views_output[]['open_button_text'] = array(
        '#type' => 'html_tag',
        '#tag' => 'div',
        '#value' => t((isset($settings['yamaps_display_options']['open_button_text'])) ?
          $settings['yamaps_display_options']['open_button_text'] : 'Open button'),
        '#attributes' => array(
          'id' => $open_button_id,
          'class' => 'open_yamap_button',
          'mapId' => $id,
        ),
      );
    }
    $views_output[] = array(
      '#type' => 'html_tag',
      '#tag' => 'div',
      '#attributes' => array(
        'style' => ' width: ' . $yandexmap_field_settings['width'] . '; height:' . $yandexmap_field_settings['height'],
        'id' => $id,
        'class' => array(
          'yamaps-map-container',
        ),
      ),
      '#value' => '',
    );

    if (isset($this->options['yamaps_display_options']['display_type']) && $this->options['yamaps_display_options']['display_type'] == 'map_button') {
      $views_output['open_map_button'] = array(
        '#type' => 'html_tag',
        '#tag' => 'div',
        '#value' => t((isset($this->options['yamaps_display_options']['open_button_text'])) ? $this->options['yamaps_display_options']['open_button_text'] : 'Open map'),
        '#attributes' => array(
          'id' => $open_button_id,
          'class' => 'open_yamap_button',
          'mapId' => $id,
        ),
      );
    }

    return $views_output;
  }

  /**
   * Getting fields.
   *
   * @return array
   *   Fields list.
   */
  function getFields() {
    $field_names = array(
      $this::PLACEMARK_NONE_FIELD => t('<none>'),
      $this::PLACEMARK_DEFAULT_FIELD => t('Default baloon value'),
    );
    $fields = $this->display->handler->get_handlers('field');
    foreach ($fields as $id => $handler) {
      if (isset($handler->human_name)) {
        $field_names[$id] = $handler->human_name;
      }
      else {
        $field_names[$id] = $handler->definition['title'];
      }
    }
    return $field_names;
  }

  function preparePlacemark() {

  }

  /**
   * Clearing
   *
   * @param $row_index
   * @return string
   */
  function preparePlacemarkTitle($row_index) {
    $title = $this->get_field($row_index, $this->options['placemark_title']);
    return check_plain(strip_tags($title));
  }

  /**
   * Getting yandex maps fields.
   *
   * @return array
   *   List of yandex maps fields.
   */
  function getYandexMapsFields() {
    $field_names = array($this::PLACEMARK_NONE_FIELD => t('<none>'));
    $fields = $this->display->handler->get_handlers('field');
    foreach ($fields as $id => $handler) {
      if (isset($handler->field_info) && $handler->field_info['module'] == 'yamaps') {
        $field_names[$id] = $handler->definition['title'];
      }
    }
    return $field_names;
  }
}
