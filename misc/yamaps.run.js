/**
 * @file
 * Runs map.
 */

(function($) {
  Drupal.behaviors.yamapsRun = {
    attach: function (context, settings) {
      ymaps.ready(function () {
        function creating_map(mapId, options) {
          $('#' + mapId).once('yamaps', function () {
            // If zoom and center are not set - set it from user's location.

            if (!options.init.center || !options.init.zoom) {
              var location = ymaps.geolocation;
              // Set map center.
              if (!options.init.center) {
                // Set location, defined by ip, if they not defined.
                options.init.center = [location.latitude, location.longitude];
              }
              if (!options.init.zoom) {
                options.init.zoom = location.zoom ? location.zoom : 10;
              }
            }
            // Create new map.
            var map = new $.yaMaps.YamapsMap(mapId, options);
            if (options.controls) {
              // Enable controls.
              map.enableControls();
            }
            if (options.traffic) {
              // Enable traffic.
              map.enableTraffic();
            }
            // Enable plugins.
            map.enableTools();
          });
        }

        var processMaps = function () {
          if (Drupal.settings.yamaps) {
            for (var mapId in Drupal.settings.yamaps) {
              var options = Drupal.settings.yamaps[mapId];
              if (options.display_options.display_type === 'map_button') {
                $('#' + mapId).hide();
                $('#' + options.display_options.remove_button_id).hide();
                $('#' + options.display_options.open_button_id).bind({
                  click: function () {
                    mapId = $(this).attr('mapId');
                    options = Drupal.settings.yamaps[mapId];
                    creating_map(mapId, options);
                    $('#' + options.display_options.open_button_id).hide();
                    $('#' + mapId).show();
                    $('#' + options.display_options.remove_button_id).show();
                  }
                });
              }
              else {
                creating_map(mapId, options);
              }
            }
          }
        };

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

        // Initialize layouts.
        $.yaMaps.initLayouts();
        processMaps();
        processMapsStatic();

        Drupal.behaviors.yamapsInitBehaviors = {
          attach: processMaps
        };
      });
	  }
  }
})(jQuery);
