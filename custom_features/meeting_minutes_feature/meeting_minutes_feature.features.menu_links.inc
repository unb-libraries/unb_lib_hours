<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function meeting_minutes_feature_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: navigation:meeting_minutes
  $menu_links['navigation:meeting_minutes'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'meeting_minutes',
    'router_path' => 'meeting_minutes',
    'link_title' => 'Meeting Minutes',
    'options' => array(
      'attributes' => array(
        'title' => 'Browse Meeting Minutes by Committee',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Meeting Minutes');


  return $menu_links;
}
