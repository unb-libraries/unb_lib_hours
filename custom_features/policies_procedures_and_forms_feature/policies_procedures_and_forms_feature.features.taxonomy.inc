<?php

/**
 * Implementation of hook_taxonomy_default_vocabularies().
 */
function policies_procedures_and_forms_feature_taxonomy_default_vocabularies() {
  return array(
    'ppf_categories' => array(
      'name' => 'Policies, Procedures and Forms Categories',
      'description' => 'Categories for Policies, Procedures and Forms',
      'help' => '',
      'relations' => '1',
      'hierarchy' => '0',
      'multiple' => '0',
      'required' => '1',
      'tags' => '0',
      'module' => 'features_ppf_categories',
      'weight' => '0',
      'nodes' => array(
        'policies_procedures_forms' => 'policies_procedures_forms',
      ),
    ),
  );
}
