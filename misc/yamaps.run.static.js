/**
 * @file
 * Display static map.
 */
(function ($) {
  Drupal.behaviors.yamapsStatic = {
    attach: function(context) {
      function processMapsStatic() {
        if (Drupal.settings.yamapsStatic) {
          for (var mapId in Drupal.settings.yamapsStatic) {
            var options = Drupal.settings.yamapsStatic[mapId];
            if (options.display_options.display_type === 'map_button') {
              $('#' + mapId).hide();
              $('#' + options.display_options.open_button_id).bind({
                click: function () {
                  mapId = $(this).attr('mapId');
                  options = Drupal.settings.yamapsStatic[mapId];
                  $('#' + options.display_options.open_button_id).hide();
                  $('#' + mapId).show();
                }
              });
            }
          }
        }
      }

      processMapsStatic();
    }
  }
})(jQuery);
