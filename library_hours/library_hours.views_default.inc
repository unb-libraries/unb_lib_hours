<?php

function library_hours_views_default_views () {
	#
	# banner notices list
	#
	$view = new view;
	$view->name = 'banner_notice_list';
	$view->description = 'Display List of banner notices';
	$view->tag = 'custom';
	$view->view_php = '';
	$view->base_table = 'node';
	$view->is_cacheable = FALSE;
	$view->api_version = 2;
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	$handler = $view->new_display('default', 'Defaults', 'default');
	$handler->override_option('fields', array(
	  'title' => array(
	    'label' => '',
	    'alter' => array(
	      'alter_text' => 0,
	      'text' => '',
	      'make_link' => 0,
	      'path' => '',
	      'link_class' => '',
	      'alt' => '',
	      'prefix' => '',
	      'suffix' => '',
	      'target' => '',
	      'help' => '',
	      'trim' => 0,
	      'max_length' => '',
	      'word_boundary' => 1,
	      'ellipsis' => 1,
	      'html' => 0,
	      'strip_tags' => 0,
	    ),
	    'empty' => '',
	    'hide_empty' => 0,
	    'empty_zero' => 0,
	    'link_to_node' => 1,
	    'exclude' => 0,
	    'id' => 'title',
	    'table' => 'node',
	    'field' => 'title',
	    'relationship' => 'none',
	  ),
	));
	$handler->override_option('sorts', array(
	  'changed' => array(
	    'order' => 'DESC',
	    'granularity' => 'second',
	    'id' => 'changed',
	    'table' => 'node',
	    'field' => 'changed',
	    'relationship' => 'none',
	  ),
	));
	$handler->override_option('filters', array(
	  'type' => array(
	    'operator' => 'in',
	    'value' => array(
	      'banner_notice' => 'banner_notice',
	    ),
	    'group' => '0',
	    'exposed' => FALSE,
	    'expose' => array(
	      'operator' => FALSE,
	      'label' => '',
	    ),
	    'id' => 'type',
	    'table' => 'node',
	    'field' => 'type',
	    'relationship' => 'none',
	  ),
	));
	$handler->override_option('access', array(
	  'type' => 'perm',
	  'perm' => 'administer banner notice',
	));
	$handler->override_option('cache', array(
	  'type' => 'none',
	));
	$handler->override_option('title', 'All banner notices');
	$handler = $view->new_display('page', 'Page', 'page_1');
	$handler->override_option('path', 'banner_notices');
	$handler->override_option('menu', array(
	  'type' => 'normal',
	  'title' => 'Banner Notices',
	  'description' => 'List all banner notices',
	  'weight' => '0',
	  'name' => 'navigation',
	));
	$handler->override_option('tab_options', array(
	  'type' => 'none',
	  'title' => '',
	  'description' => '',
	  'weight' => 0,
	  'name' => 'navigation',
	));
	
	$views[$view->name] = $view;
	
	return $views;
}
