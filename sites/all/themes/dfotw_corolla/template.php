<?php

/**
 * Implements theme_feed_icon().
 */
function dfotw_corolla_feed_icon($variables) {
  return <<<EOT
<a title="Subscribe to Drupal Function of the Week RSS" class="feed-icon" href="http://feeds.feedburner.com/dfotw"><img src="http://feeds.feedburner.com/~fc/dfotw?bg=858A26&amp;fg=222222&amp;anim=0" height="26" width="88" style="border:0" alt="" /></a>
EOT;
}