<div id="dfotw_browse_version">
  <?php if (!empty($versions)): ?>
  <ul>
    <?php foreach($versions as $tid => $title): ?>
      <li><?php echo l($title, 'taxonomy/term/' . $tid); ?></li>
    <?php endforeach; ?>
  </ul>
  <?php else: ?>
    <p class="empty"><?php echo t('These are no versions'); ?></p>
  <?php endif; ?>
</div>
