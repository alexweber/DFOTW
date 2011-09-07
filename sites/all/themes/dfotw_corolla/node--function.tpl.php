<?php
//include_once drupal_get_path('module', 'devel') . '/krumo/class.krumo.php';
//krumo($node);

$func = new stdClass();

$func->intro = $node->field_function_intro['und'][0]['safe_value'];
$func->desc = $node->field_function_desc['und'][0]['safe_value'];
$func->body = $node->body['und'][0]['safe_value'];

if ($page) {
  $func->examples = array();
  foreach ($node->field_function_example['und'] as $ex) {
    $func->examples[] = $ex['value']; // permite tags HTML
  }

  $func->versions = array();
  foreach ($node->field_function_version['und'] as $ver) {
//    $term_view = taxonomy_term_view($ver['tid']);
//    $func->versions[] = l($term_view['#term']->name, 'taxonomy/term/' . $term_view['#term']->tid, array('target' => '_blank', 'rel' => 'nofollow'));
    $func->versions[] = l($ver['taxonomy_term']->name, 'taxonomy/term/' . $ver['tid'], array());
  }
  $func->versions_string = implode(', ', $func->versions);

  $func->docs = array();
  foreach ($node->field_function_docs['und'] as $doc) {
    $func->docs[] = l($doc['title'], $doc['url'], array('attributes' => $doc['attributes']));
  }
  $func->docs_string = implode(' | ', $func->docs);

  $func->tags = array();
  foreach ($node->field_function_tags['und'] as $tag) {
    $term_view = taxonomy_term_view($tag['taxonomy_term']);
    $func->tags[] = l($term_view['#term']->name, 'taxonomy/term/' . $term_view['#term']->tid);
  }
  $func->tags_string = implode(', ', $func->tags);

  $func->author = l($node->field_function_author['und'][0]['title'], $node->field_function_author['und'][0]['url'], array('attributes' => $node->field_function_author['und'][0]['attributes']));
}
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <div class="node-header">
    <?php print render($title_prefix); ?>
    <?php if (!$page): ?>
      <h2 class="node-title" <?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>
  </div> <!-- /.node-header -->

  <div class="content clearfix" <?php print $content_attributes; ?>>

    <?php if ($page): ?>
      <p><strong>Versions:</strong> <?php echo $func->versions_string; ?></p>

      <h3><?php echo $func->intro; ?></h3>
      <blockquote><?php echo $func->desc; ?></blockquote>

      <?php echo $func->body; ?>

      <?php if (!empty($func->examples)): ?>
        <h2>Usage</h2>
        <?php foreach ($func->examples as $example): ?>
          <?php echo $example; ?>
        <?php endforeach; ?>
      <?php endif; ?>

      <div class="function-links-container">
        <h2>Links</h2>

        <div class="function-links-subcontainer">
          <?php if (!empty($func->docs)): ?>
            <strong>Documentation:</strong>
            <?php echo $func->docs_string; ?>
          <?php endif; ?>
        </div><!-- /.function-links-subcontainer -->

        <div class="function-links-subcontainer">
          <?php if (!empty($func->tags)): ?>
            <strong>Tags:</strong>
            <?php echo $func->tags_string; ?>
          <?php endif; ?>
        </div><!-- /.function-links-subcontainer -->

        <div class="function-links-subcontainer">
          <?php if (!empty($func->author)): ?>
            <strong>Submitted by:</strong>
            <?php echo $func->author; ?>
          <?php endif; ?>
        </div><!-- /.function-links-subcontainer -->

      </div><!-- /.function-links-container -->
    <?php else: ?>
      <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
      ?>
    <?php endif; ?>

  </div><!-- /.content.clearfix -->

  <div class="links-wrapper">
    <?php print render($content['links']); ?>
  </div>
  <?php print render($content['comments']); ?>

</div><!-- /.node -->