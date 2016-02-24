<?php

/**
 * @file
 * Cookie Consent administration pages.
 */

/**
 * Menu callback. Return the Cookie Consent configuration form
 */
function eu_cookie_consent_settings_form($form) {

  $library_path = eu_cookie_consent_get_library_path();
  if (!$library_path) {
    drupal_set_message(t('The library could not be detected. You need to download the !library and extract the contents of the github repo at %path directory',
      array('!library' => l(t('EU Cookie Consent Library'), JQUERY_EU_COOKIE_CONSENT_WEBSITE_URL), '%path' => 'sites/all/libraries/eu-cookie-consent')
    ), 'error');
  }
  $available_themes = array(
    'none'      => 'None',
    'light'      => 'Light',
    'dark'       => 'Dark',
  );

  $settings = eu_cookie_consent_get_default_values();
  $form['eu_cookie_consent'] = array(
    '#type' => 'fieldset',
    '#title' => t('EU Cookie Settings'),
    '#tree' => TRUE, // All the options in this fieldset will be grouped in 1 single variable.
  );
  $form['eu_cookie_consent']['theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#options' => $available_themes,
    '#default_value' => $settings['theme'],
    '#description' => t('The theme of the cookie message. Choose <em>None</em>em>, if you are adding your own custom CSS'),
  );
  $form['eu_cookie_consent']['hide_on_any_click'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide on any click'),
    '#default_value' => (boolean) $settings['hide_on_any_click'],
    '#description' => t('Hide the cookie message on any click on the website.'),
  );
  $form['eu_cookie_consent']['hide_on_scroll'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide on scroll'),
    '#default_value' => (boolean) $settings['hide_on_scroll'],
    '#description' => t('Hide the cookie message when user starts scrolling.'),
  );
  $form['eu_cookie_consent']['scroll_delay'] = array(
    '#type' => 'textfield',
    '#size' => 9,
    '#maxlength' => 6,
    '#title' => t('Scroll delay'),
    '#default_value' => (int) $settings['scroll_delay'],
    '#description' => t('Initial delay (in milliseconds) until the scroll starts to be tracked. It prevents that accidental (or browser-initiated) scrolling hides the message without the user noticing. <em>Default value: 2000 (2 seconds)</em>'),
  );
  $form['eu_cookie_consent']['expiry_days'] = array(
    '#type' => 'textfield',
    '#size' => 9,
    '#maxlength' => 6,
    '#title' => t('Expiry days'),
    '#default_value' => (int) $settings['expiry_days'],
    '#description' => t('Number of days until the consent expires and EU Cookie message shows again. <em>Default: 365</em>'),
  );
  $form['eu_cookie_consent']['message'] = array(
    '#type' => 'textfield',
    '#title' => t('Message'),
    '#default_value' => $settings['message'],
    '#description' => t('Message displayed. You can use the special tokens {{ link_1 }} and {{ link_2 }}. It would be replaced with the Links below. HTML allowed: a,br,em,strong,cite,blockquote,code,ul,ol,li,dl,dt,dd'),
  );
  $form['eu_cookie_consent']['accept_btn_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Accept button text'),
    '#default_value' => $settings['accept_btn_text'],
  );
  $form['eu_cookie_consent']['link_1'] = array(
    '#type' => 'textfield',
    '#title' => t('Link #1'),
    '#default_value' => $settings['link_1'],
    '#description' => t('Link 1, to be displayed inside of the message, where users can learn more about your Cookie policy, for example. The exact string <em>{{ link_1 }}</em> needs to be in your message, for the replacement to happen.'),
  );
  $form['eu_cookie_consent']['link_1_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Link #1 Text'),
    '#default_value' => $settings['link_1_text'],
  );
  $form['eu_cookie_consent']['link_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Link #2'),
    '#default_value' => $settings['link_2'],
    '#description' => t('Link 2, to be displayed inside of the message. The exact string <em>{{ link_2 }}</em> needs to be in your message, for the replacement to happen.'),
  );
  $form['eu_cookie_consent']['link_2_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Link #2 Text'),
    '#default_value' => $settings['link_2_text'],
  );

  return system_settings_form($form);
}


/*
 * Validate and sanitize input values from the settings form
 */
function eu_cookie_consent_settings_form_validate($form, &$form_state) {
  $settings = &$form_state['values']['eu_cookie_consent'];

  if ($settings['link_1'] && !$settings['link_1_text']) {
    form_set_error('eu_cookie_consent][link_1_text', t('Error. There is no value for <em>Link #1 Text</em>. Please fill it, or remove Link #1'));
  }
  if ($settings['link_2'] && !$settings['link_2_text']) {
    form_set_error('eu_cookie_consent][link_2_text', t('Error. There is no value for <em>Link #2 Text</em>. Please fill it, or remove Link #2'));
  }

  $settings['link_2_text'] = check_plain(trim($settings['link_2_text']));

  if (!is_numeric($settings['expiry_days'])) {
    form_set_error('eu_cookie_consent][expiry_days', t('Expiry days must be a number. 365 is the default value, for example.'));
  } else {
    $settings['expiry_days'] = (int) $settings['expiry_days'];
  }
  if (!is_numeric($settings['scroll_delay'])) {
    form_set_error('eu_cookie_consent][scroll_delay', t('Scroll delay must be a number. 2000 is the default value, for example.'));
  } else {
    $settings['scroll_delay'] = (int) $settings['scroll_delay'];
  }

  $settings['link_1'] = check_url($settings['link_1']);
  $settings['link_2'] = check_url($settings['link_2']);
  // filter_xss default tags, plus '<br>'
  $allowed_tags = array('a', 'br', 'em', 'strong', 'cite', 'blockquote', 'code', 'ul', 'ol', 'li', 'dl', 'dt', 'dd');
  $settings['message'] = filter_xss($settings['message'], $allowed_tags);
}
