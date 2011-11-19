// jQuery.ready() used for speed instead of Drupal.behaviors
jQuery(document).ready(function($){
  $('#block-system-main-menu').find('ul.menu').find('li').each(function(){
    var $t = $(this);
    if ($t.find('a.active').length) {
      $t.addClass('active-trail');
    }
  });
});;
