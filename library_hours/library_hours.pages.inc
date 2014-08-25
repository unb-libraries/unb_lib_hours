<?php

function library_hours_daterange($node) {
	$output = t('Use this page to add/edit a date range for <em>!title</em>.', array('!title' => $node->title));
	$output .= drupal_render( drupal_get_form('library_hours_daterange_form', $node) );
	return $output; 
}

function library_hours_holidays($node) {
	$output = t('Use this page to add/edit a holidays dates for <em>!title</em>.', array('!title' => $node->title));
	
	# date list
	$holidays = array();
	$result = db_query("SELECT hid, nid, label, date FROM {holidays} WHERE nid = :nid ORDER BY UNIX_TIMESTAMP(date) ASC", array(':nid'=>$node->nid));
	while ($holiday = db_fetch_object($result)) {
		$row = array();
		$row[] = check_plain($holiday->label);
		$row[] = check_plain(date('F d', strtotime($holiday->date)));
		$row[] = l(t('Delete'), "node/{$node->nid}/holidays/delete/{$holiday->hid}");
		
		$rows[] = $row;
	}
	
	if (empty($rows)) {
		$rows[] = array(array('data' => t('Empty List.'), 'colspan' => 3));
	}
	
	$header = array(t('Label'), t('Date'), array('data' => t('Operations')));
	$output .= theme('table', $header, $rows);
	
	$output .= drupal_get_form('library_hours_holidays_form', $node);
	return $output;
}

function library_hours_holidays_form(&$form_state, $node, $edit = array()) {
	$form['holiday'] = array(
		'#tree' => TRUE,
		'#type' => 'fieldset',
		'#access' => user_access('access content'),
		'#title' => t('Enter a Holiday'),
		'#collapsible' => TRUE,
	);
	
	$form['holiday']['label'] = array(
		'#type' => 'textfield',
		'#title' => t('Label'),
		'#description' => t('Enter holiday name'),
		'#default_value' => isset($edit['label']) ? $edit['label'] : '', 
	);
	
	$form['holiday']['date'] = array(
		'#title' => t('Date'),
		'#type' => 'date',
		'#description' => t('Set holiday date'),
	);
	
	$form['holiday']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save'),
		'#name' => 'holiday_save',
	);
	
	return $form;
}

function library_hours_holidays_form_validate($form, &$form_state) {
	$label = trim($form_state['values']['holiday']['label']);
	if (empty($label)) {
		form_set_error('label', t('Label field is required.'));
	}
}

function library_hours_holidays_form_submit($form, &$form_state) {
	$node = $form['#parameters'][2]; 
	
	$holiday = new stdClass();
	$holiday->nid = $node->nid;
	$holiday->label = $form_state['values']['holiday']['label'];
	$holiday->date = date('Y-m-d H:i:s', mktime(0,0,0,
		$form_state['values']['holiday']['date']['month'],
		$form_state['values']['holiday']['date']['day'],
		$form_state['values']['holiday']['date']['year']
	)); 
	
	drupal_write_record('holidays', $holiday);
	watchdog('library_hours', 'Added new holiday date for node with id %id', array('%id' => $node->nid));
	drupal_set_message('New holiday date added successfully');
}

function library_hours_daterange_form($form, &$form_state, $node, $edit = null) {
	if (!isset($form_state['timeframe_count']) && !empty($edit)) {
		$form_state['timeframe_count'] = count($edit['time_frame']['rows']);
	}
	
	$form['#cache'] = TRUE;
	
	if (!empty($edit)) {
		$form['#action'] = "/node/{$node->nid}/daterange/edit/{$edit['did']}";
	}
	else {
		$form['#action'] = "/node/{$node->nid}/daterange/add";		
	}
	
	if (!empty($edit)) {
		$form['did'] = array(
			'#type' => 'hidden',
			'#value' => $edit['did'], 
		);
		
		$breadcrumb = drupal_get_breadcrumb();
		$breadcrumb[] = "<a href=\"/library_hours\">Library Hours</a>";
		$breadcrumb[] = "<a href=\"/node/{$node->nid}\">" . check_plain($node->title) . '</a>';
		drupal_set_breadcrumb($breadcrumb);
	}
	
	$form['label'] = array(
		'#type' => 'textfield',
		'#title' => t('Label'),
		'#default_value' => isset($edit['label']) ? $edit['label'] : '', 
	);
	
	$form['hide_label'] = array(
		'#title' => 'Hide label',
		'#type' => 'checkbox',
		'#default_value' => isset($edit['hide_label']) ? $edit['hide_label'] : 0,
		'#description' => t("Check this if you don't want the label to be displayed")
	);
	
	$start_date_value;
	if (isset($edit['start_date'])) {
		$start_date_value = $edit['start_date'];
	}
	else {
		$start_date_value = array(
			'month' => format_date(time(), 'custom', 'n'),
			'day' => format_date(time(), 'custom', 'j'),
			'year' => format_date(time(), 'custom', 'Y'),
		);
	}
	$form['start_date'] = array(
		'#title' => t('Start Date'),
		'#type' => 'date',
		'#description' => t('Set Start Date'),
		'#default_value' => $start_date_value
	);
	
	$end_date_value;
	if (isset($edit['end_date'])) {
		$end_date_value = $edit['end_date'];
	}
	else {
		$end_date_value = array(
			'month' => format_date(time(), 'custom', 'n'),
			'day' => format_date(time(), 'custom', 'j'),
			'year' => format_date(time(), 'custom', 'Y'),
		);
	}
	$form['end_date'] = array(
		'#title' => t('End Date'),
		'#type' => 'date',
		'#description' => t('Set End Date'),
		'#default_value' => $end_date_value
	);
	
	$form['time_frame'] = array(
		'#tree' => TRUE,
		'#type' => 'fieldset',
		'#access' => user_access('access content'),
		'#title' => t('Time Frame'),
		'#collapsible' => TRUE,
	);
	
	$form['time_frame']['rows'] = array(
		'#prefix' => '<div id="timeframe-elements">',
		'#suffix' => '</div>',
	);
	
	if (!isset($form_state['timeframe_count'])) {
		$form_state['timeframe_count'] = 2;
	}
	else {
		$form_state['timeframe_count']++;
	}
	
	for ($i=1; $i<=$form_state['timeframe_count']; $i++) {
		$default = isset($form_state['post']['time_frame']['rows'][$i]) 
			? $form_state['post']['time_frame']['rows'][$i]["timeframe_element_{$i}"]
			: null;
		if (($default == null) && !empty($edit)) {
			$default = isset($edit['time_frame']['rows'][$i]) 
				? $edit['time_frame']['rows'][$i]["timeframe_element_{$i}"]
				: null;
		}
		
		$form['time_frame']['rows'][$i] = _build_time_frame_element($i, $default);
	} 
	
	$form['time_frame']['add_more'] = array (
		'#type' => 'submit',
		'#value' => t('Add another element'),
		'#name' => 'timeframe_more',
		'#submit' => array('time_frame_more_submit'),
		'#ahah' => array(
			'path' => 'library_hours/timeframe/js',
			'wrapper' => 'timeframe-elements',
			'method' => 'replace',
			'effect' => 'fade'
		)
	);
	
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save'),
		'#name' => 'timeframe_save',
	);
	
	return $form;
}

function _build_time_frame_element($id, $default = null) {
	
	if ($default == null) {
		$default = array(
			"start_day_{$id}" => 'Mon',
			"end_day_{$id}" => 'NaN',
			"start_time_{$id}" => '',
			"end_time_{$id}" => '',
			"alt_text_{$id}" => '',
			"options_{$id}" => array()
		);
	}
	
	$element["timeframe_element_{$id}"] = array(
		'#tree' => TRUE,
		'#prefix' => '<div class="timeframe-element">',
		'#suffix' => '</div>'
	);
	
	$element["timeframe_element_{$id}"]["start_day_{$id}"] = array(
		'#type' => 'select',
		'#title' => t('Start Day'),
		//'#required' => TRUE,
		'#default_value' => $default["start_day_{$id}"],
		'#options' => array(
			'Mon' => t('Monday'),
			'Tue' => t('Tuesday'),
			'Wed' => t('Wednesday'),
			'Thu' => t('Thursday'),
			'Fri' => t('Friday'),
			'Sat' => t('Saturday'),
			'Sun' => t('Sunday')
		) 
	);

	$element["timeframe_element_{$id}"]["end_day_{$id}"] = array(
		'#type' => 'select',
		'#title' => t('Select End Day'),
		'#default_value' => $default["end_day_{$id}"],
		'#options' => array(
			'NaN' => t('Same Day'),
			'Mon' => t('Monday'),
			'Tue' => t('Tuesday'),
			'Wed' => t('Wednesday'),
			'Thu' => t('Thursday'),
			'Fri' => t('Friday'),
			'Sat' => t('Saturday'),
			'Sun' => t('Sunday')
		) 
	);
	
	$element["timeframe_element_{$id}"]["alt_text_{$id}"] = array(
		'#type' => 'textfield',
		'#title' => 'Alternate Text',
		'#default_value' => $default["alt_text_{$id}"],
		'#description' => 'This will replace selected days. For instance Good Friday.'
	);
	
	$element["timeframe_element_{$id}"]["start_time_{$id}"] = array (
		'#type' => 'textfield',
		'#title' => 'Start Time',
		'#size' => 10,
		'#default_value' => $default["start_time_{$id}"],
		'#description' => 'Format: 8:00 AM'
	);
	
	$element["timeframe_element_{$id}"]["end_time_{$id}"] = array (
		'#type' => 'textfield',
		'#title' => 'End Time',
		'#size' => 10,
		'#default_value' => $default["end_time_{$id}"],
		'#description' => 'Format: 8:00 AM'
	);
	
	// check boxes
	$checkboxes_options = array(
		'is_closed' => t('Library is closed'),
		'delete' => t('Delete entry') 
	);
	$element["timeframe_element_{$id}"]["options_{$id}"] = array (
		'#title' => t('Additional Options'),
		'#type' => 'checkboxes',
		'#description' => t('Please note if any of these actions apply this time frame'),
		'#options' => $checkboxes_options,
		'#default_value' => isset($default["options_{$id}"]) ? $default["options_{$id}"] : array()
	);
	
	return $element;
}

function time_frame_more_submit($form, $form_state) {
	
}

function library_hours_timeframe_empty ($key, $timeframe) {
	return ($timeframe["start_day_{$key}"] == 'Mon') 
		&& ($timeframe["end_day_{$key}"] == 'NaN')
		&& empty($timeframe["start_time_{$key}"])
		&& empty($timeframe["end_time_{$key}"])
		&& !$timeframe["options_{$key}"]["is_closed"]; 
}

function library_hours_daterange_form_validate($form, &$form_state) {
	// we want to ignore AHAH validation calls
	// so that validation msgs are not duplicated
	if ($form_state['clicked_button']['#name'] == 'timeframe_more'){
		return;
	}
	
	$label = trim($form_state['values']['label']);
	if (empty($label)) {
		form_set_error('label', t('Label field is required.'));
	}

	$patterns = '/^\d{1,2}:\d{2}\s?[a|p]m/i';
	foreach($form_state['values']['time_frame']['rows'] as $key => $tf_element) {
		if (!library_hours_timeframe_empty($key, $tf_element["timeframe_element_{$key}"])
			&& !$tf_element["timeframe_element_{$key}"]["options_{$key}"]["is_closed"]) {
				
			$time_start = trim($tf_element["timeframe_element_{$key}"]["start_time_{$key}"]);
			$time_end = trim($tf_element["timeframe_element_{$key}"]["end_time_{$key}"]);
			if (empty($time_start)) {
				form_set_error("time_frame][rows][{$key}][timeframe_element_{$key}][start_time_{$key}", 
					t('Start Time is a required field'));
			}
			else if (!preg_match($patterns, $time_start)) {
				form_set_error("time_frame][rows][{$key}][timeframe_element_{$key}][start_time_{$key}", 
					t('!time is not a valid start time', array('!time' => $time_start)));
			}
			
			if (empty($time_end)){
				form_set_error("time_frame][rows][{$key}][timeframe_element_{$key}][end_time_{$key}", 
					t('End Time is a required field'));
			} 
			else if (!preg_match($patterns, $time_end)) {
				form_set_error("time_frame][rows][{$key}][timeframe_element_{$key}][end_time_{$key}", 
					t('!time is not a valid end time', array('!time' => $time_end)));
			} 
		}
	}
}

function library_hours_daterange_form_submit($form, &$form_state) {
	$node = $form_state['build_info']['args'][0]; 
	
	$timestamp = null;
	// we create date range first
	$dr_rec = new stdClass();
	
	if (isset($form_state['values']['did'])) {
		$dr_rec->did = $form_state['values']['did'];
		// reset time frames for this date range
		db_query('DELETE FROM {timeframes} WHERE did = :did', array(':did'=>$form_state['values']['did']));
	}
	
	$dr_rec->nid = $node->nid;
	$dr_rec->label = $form_state['values']['label'];
	$timestamp = mktime(0,0,0,$form_state['values']['start_date']['month'],$form_state['values']['start_date']['day'],$form_state['values']['start_date']['year']);
	$dr_rec->startDate = date('Y-m-d H:i:s', $timestamp);
	$timestamp = mktime(0,0,0,$form_state['values']['end_date']['month'],$form_state['values']['end_date']['day'],$form_state['values']['end_date']['year']);
	$dr_rec->endDate = date('Y-m-d H:i:s', $timestamp);
	$dr_rec->hideLabel = $form_state['values']['hide_label'];
	
	if (isset($form_state['values']['did'])) {
		drupal_write_record('dateranges', $dr_rec, 'did');
	}
	else {
		drupal_write_record('dateranges', $dr_rec);
	}
	
	if (isset($form_state['values']['did'])) {
		watchdog('library_hours', 'Date Range %did updated', array('%did' => $form_state['values']['did']));
	}
	else {
		watchdog('library_hours', 'Added new date range to node with id %id', array('%id' => $node->nid));
	}
	 
	foreach($form_state['values']['time_frame']['rows'] as $key => $timeframe) {
		if (!library_hours_timeframe_empty($key, $timeframe["timeframe_element_{$key}"])
			&& !$timeframe["timeframe_element_{$key}"]["options_{$key}"]["delete"]) {
			$tf_rec = new stdClass();
			$tf_rec->did = $dr_rec->did;
			$tf_rec->startDay = $timeframe["timeframe_element_{$key}"]["start_day_{$key}"];
			$tf_rec->endDay = $timeframe["timeframe_element_{$key}"]["end_day_{$key}"];
			$tf_rec->startTime = date('Y-m-d H:i:s', !empty($timeframe["timeframe_element_{$key}"]["start_time_{$key}"]) ? strtotime($timeframe["timeframe_element_{$key}"]["start_time_{$key}"]) : 0);
			$tf_rec->endTime = date('Y-m-d H:i:s',  !empty($timeframe["timeframe_element_{$key}"]["end_time_{$key}"]) ? strtotime($timeframe["timeframe_element_{$key}"]["end_time_{$key}"]) : 0);
			$tf_rec->altText = trim($timeframe["timeframe_element_{$key}"]["alt_text_{$key}"]);
			$tf_rec->isClosed = (($timeframe["timeframe_element_{$key}"]["options_{$key}"]["is_closed"] === "is_closed") ? 1 : 0);
			drupal_write_record('timeframes', $tf_rec);
			watchdog('library_hours', 'Added new time frame to date range with id %id', array('%id' => $dr_rec->did));
		}
	}
	
	$form_state['redirect'] = "/node/{$node->nid}";
}

function timeframe_more_js () {

	$form_state = array('storage' => NULL, 'submitted' => FALSE);
	$form_build_id = $_POST['form_build_id'];
	
	// Get the form from the cache.
	$form = form_get_cache($form_build_id, $form_state);
	$args = $form['#parameters'];
	$form_id = array_shift($args);
	
	// We will run some of the submit handlers so we need to disable redirecting.
	$form['#redirect'] = FALSE;
	
	// We need to process the form, prepare for that by setting a few internals variables.
  	$form['#post'] = $_POST;
  	$form['#programmed'] = FALSE;
  	$form_state['post'] = $_POST;
  	$form_state['timeframe_count'] = count($form_state['post']['time_frame']['rows']);
  	
  	// Build, validate and if possible, submit the form.
	drupal_process_form($form_id, $form, $form_state);
	
	// This call recreates the form relying solely on the form_state that the
	// drupal_process_form set up.
	$form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
	
	$timeframe_form = $form['time_frame']['rows'];
	$output = drupal_render($timeframe_form);
	
	drupal_json(array('status' => TRUE, 'data' => $output));
}

function library_hours_daterange_edit($node, $daterange) {
	return drupal_get_form ('library_hours_daterange_form', $node, $daterange);
}

function library_hours_holiday_confirm_delete(&$form_state, $node, $holiday) {
	$form['nid'] = array('#type' => 'value', '#value' => $node->nid);
	$form['hid'] = array('#type' => 'value', '#value' => $holiday->hid);
	$form['label'] = array('#type' => 'value', '#value' => $holiday->label);
	return confirm_form (
		$form,
		t('Are you sure you want to delete the entry for %title?', array('%title' => $holiday->label)),
		"node/{$node->nid}/holidays",
		t('This action cannot be undone.'),
		t('Delete'),
		t('Cancel')
	);
}

function library_hours_daterange_confirm_delete(&$form_state, $node, $daterange) {
	$form['nid'] = array('#type' => 'value', '#value' => $node->nid);
	$form['did'] = array('#type' => 'value', '#value' => $daterange['did']);
	$form['label'] = array('#type' => 'value', '#value' => $daterange['label']);
	return confirm_form (
		$form,
		t('Are you sure you want to delete the entry for %title?', array('%title' => $daterange['label'])),
		"node/{$node->nid}",
		t('Deleting a date range will delete all the data associated to it. This action cannot be undone.'),
		t('Delete'),
		t('Cancel')
	);
}

function library_hours_holiday_confirm_delete_submit($form, &$form_state) {
	$hid = (int) $form_state['values']['hid'];
	db_query('DELETE FROM {holidays} WHERE hid = :hid', array(':hid'=>$hid));
	drupal_set_message(t('Deleted Holiday %label', array('%label' => $form_state['values']['label'])));
	watchdog('library_hours', 'Deleted Holiday %label', array('%label' => $form_state['values']['label']), WATCHDOG_NOTICE);
	$form_state['redirect'] = "node/{$form_state['values']['nid']}/holidays";
}

function library_hours_daterange_confirm_delete_submit($form, &$form_state) {
	$did = (int) $form_state['values']['did'];
	db_query('DELETE FROM {timeframes} WHERE did = :did', array(':did'=>$did));
	db_query('DELETE FROM {dateranges} WHERE did = :did', array(':did'=>$did));
	drupal_set_message(t('Deleted Date Range %label', array('%label' => $form_state['values']['label'])));
	watchdog('library_hours', 'Deleted Date Range %label', array('%label' => $form_state['values']['label']), WATCHDOG_NOTICE);
	$form_state['redirect'] = "node/{$form_state['values']['nid']}";
}
