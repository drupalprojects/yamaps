<?php

/**
 * @file
 * Yandex Maps Admin UI file.
 */

/**
 * Form builder for admin settings form.
 *
 * @param $form
 * @param $form_state
 *
 * @return array
 */
function yamaps_settings_page($form, &$form_state) {
  // Set amount of blocks admin can create.
  $form['yamaps_blocks_amount'] = array(
    '#title' => t('Set amount of Yandex Maps blocks'),
    '#description' => t('Amount of available Yandex Maps bloks.'),
    '#element_validate' => array('element_validate_integer_positive'),
    '#type' => 'select',
    '#options' => range(1, 20),
    '#default_value' => variable_get('yamaps_blocks_amount', YAMAPS_DEFAULT_BLOCK_AMOUNT),
    '#required' => TRUE,
  );

  $display_options = variable_get('yamaps_block_edit_display_options', array());

  // Container for display options.
  $form['yamaps_block_edit_display_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display options'),
    '#tree' => TRUE,
  );

  // Display options.
  $form['yamaps_block_edit_display_options']['display_type'] = array(
    '#type' => 'radios',
    '#title' => t('How to display map for admins'),
    '#options' => array(
      'map' => t('Map'),
      'map_button' => t('Map opened by button click'),
    ),
    '#default_value' => (isset($display_options['display_type'])) ? $display_options['display_type'] : 'map',
    '#required' => FALSE,
    '#description' => t('Type of map displaying.'),
  );

  // Text on button that opens map by click.
  $form['yamaps_block_edit_display_options']['open_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Button text'),
    '#default_value' => (isset($display_options['open_button_text'])) ? $display_options['open_button_text'] : 'Open map',
    '#required' => FALSE,
    '#description' => t('Text of button that opens map.'),
  );

  return system_settings_form($form);
}
