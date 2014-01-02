<?php

function lcd_displays_item_load($nid) {
	// look into announcements first
	$res = db_query("SELECT * FROM {ld_news} WHERE nid=%d", $nid) ;
	if (!$res) {
		$res = db_query("SELECT * FROM {ld_events} WHERE nid=%d", $nid) ;
		if (!$res) {
			return FALSE ;
		}		
	}
	
	return db_fetch_object($res) ;
}

function lcd_displays_services_targets() {
	$targets_voc = variable_get('lcd_displays_target_vocabulary', '') ;
	$tree = taxonomy_get_tree($targets_voc) ;
	$data = array() ;
	foreach ($tree as $voc_element) {
		$data[$voc_element->tid] = check_plain($voc_element->name) ;
	}
	
	drupal_json($data) ;
}


function lcd_displays_services_list($target_id) {
	$cache_id = 'lcd_displays_for_target' . $target_id ;
	$data = cache_get($cache_id) ;
	//isset($data->data) ? drupal_json($data->data) : drupal_json(array()) ;
	isset($data->data) ? print json_encode($data->data) : print json_encode(array()) ;
}