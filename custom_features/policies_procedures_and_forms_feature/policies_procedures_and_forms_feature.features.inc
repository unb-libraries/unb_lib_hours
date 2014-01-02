<?php

/**
 * Implementation of hook_node_info().
 */
function policies_procedures_and_forms_feature_node_info() {
  $items = array(
    'policies_procedures_forms' => array(
      'name' => t('Policies, Procedures and Forms'),
      'module' => 'features',
      'description' => t('Creates an entry for Policies, Procedures and Forms.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Comment'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function policies_procedures_and_forms_feature_views_api() {
  return array(
    'api' => '2',
  );
}
