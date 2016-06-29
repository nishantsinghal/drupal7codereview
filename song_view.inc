<?php
/**
 * @file
 * The Song view inc file.
 * Developer-Nishant
 * Contains all function related to song custom view.
 */

/**
 * Function for get filter on songs page.
 *
 * @param form $form
 * Take form field in considration.
 *
 */ 
function song_filter_form($form, &$form_state) {
   $form['movie'] = array(
     '#type' => 'select',
     '#title' => t('Choose Category'), 
     '#options' => getMovie(),
     );
    $form['submit_button'] = array(
     '#type' => 'submit',
     '#value' => t('Apply'), 
    );
    return $form;
}

/**
 * Function for submit filtervalues.
 *
 * @param form $form
 * Take form field in considration.
 *
 */ 
function song_filter_form_submit($form, &$form_state) {
  $movieid = $form_state['values']['movie'];
  drupal_goto("examples/songs", array(
    'query' => array(
        'movie' => $movieid,
      ),
    ));
}

/**
 * Function for get movie filter on songs.
 */ 
function getMovie() {
  $query = new EntityFieldQuery();
    $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'movies');
    $result = $query->execute();
    $nodes = array();
    if (isset($result['node'])) {
      $nids = array_keys($result['node']);
      $nodes = node_load_multiple($nids);
    }
    $list = array();
    $list["a"] = "None";
    //dpm($nodes);
    foreach ($nodes as $node) {
      $list[$node->nid] = $node->title;
    }
    //dpm($list);
    return $list;
}

/**
 * Function for show songs on basis of filters.
 *
 * @param $form
 * Take form name as a parameter.
 *
 */ 
function song_view($form) {
  $query = new EntityFieldQuery();
  $query
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'songs');

  $parameter = drupal_get_query_parameters();
    //dpm($parameter);
  if (isset($parameter['movie']) && $parameter['movie'] != "a") {
    $query->fieldCondition('field_movie', 'target_id', $parameter['movie']);
  }

  $result = $query->execute();
  $nodes = array();
  if (isset($result['node'])) {
    $nids = array_keys($result['node']);
    $nodes = node_load_multiple($nids);
  }
  //print_r($nodes);
  $list = array();
  foreach ($nodes as $node) {
    // $options = array('absolute' => TRUE);
    // $url = url('node/' . $node->nid, $options);
    // $list[] = '<a href=' . $url . '>' . $node->title . '</a>';
    $path = 'node/' . $node->nid;
    $path_alias = drupal_get_path_alias($path);
    $song_link = l(t($node->title), $path_alias);
    $list[] = $song_link ;
  }

  $theme_args = array('items' => $list, 'type' => 'ul');
  $content = theme('item_list', $theme_args);
  $view  = render(drupal_get_form($form));
  $view .= $content;
  return $view;
}