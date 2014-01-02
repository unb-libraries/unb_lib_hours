<?php

/**
 * Implementation of hook_node_info().
 */
function meeting_minutes_feature_node_info() {
  $items = array(
    'meeting_minutes' => array(
      'name' => t('Meeting Minutes'),
      'module' => 'features',
      'description' => t('Meeting Minutes posted by committees '),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function meeting_minutes_feature_views_api() {
  return array(
    'api' => '2',
  );
}
