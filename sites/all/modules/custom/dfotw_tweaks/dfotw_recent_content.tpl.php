<div id="dfotw_recent_content">
  <?php if (!empty($functions)): ?>
  <ul>
    <?php foreach($functions as $nid => $func): ?>
      <li><?php echo l($func, 'node/' . $nid); ?></li>
    <?php endforeach; ?>
  </ul>
  <?php else: ?>
    <p class="empty"><?php echo t('These is no recent content'); ?></p>
  <?php endif; ?>
</div>
