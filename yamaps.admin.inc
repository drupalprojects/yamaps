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
    '#title' => t('Map display style in admin UI for blocks'),
    '#options' => array(
      'map' => t('Map'),
      'map_button' => t('Map opened by button click'),
    ),
    '#default_value' => (isset($display_options['display_type'])) ? $display_options['display_type'] : 'map',
    '#required' => FALSE,
    '#description' => t('Configure how to display "Yandex Map" field while creating new / updating blocks.
    This setting applies to admin UI only. To display map opening in block by button click for end users select
    appropriate option in "Map display style for end users" section of the appropriate block.'),
  );

  // Text on button that opens map by click.
  $form['yamaps_block_edit_display_options']['open_button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Button text'),
    '#default_value' => (isset($display_options['open_button_text'])) ? $display_options['open_button_text'] : 'Open map',
    '#required' => FALSE,
    '#description' => t('Specify text to display on the button opening map in case of "Map opened by button click" option.
    This setting applies to admin UI only. To configure text on the button opening map for end users select appropriate
    option in "Manage Display" section of the appropriate block.'),
  );

  return system_settings_form($form);
}
