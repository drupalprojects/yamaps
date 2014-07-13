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

        function processMaps() {
          if (Drupal.settings.yamaps) {
            for (var mapId in Drupal.settings.yamaps) {
              var options = Drupal.settings.yamaps[mapId];
              creating_map(mapId, options);
            }
          }
        };

        function processMapsStatic() {
          if (Drupal.settings.yamapsStatic) {
            for (var mapId in Drupal.settings.yamapsStatic) {
              var options = Drupal.settings.yamapsStatic[mapId];
              if (options.display_options.display_type === 'map_button') {
                $('#' + options.display_options.open_button_id).bind({
                  click: function () {
                    if ($('#' + mapId).hasClass('element-invisible')) {
                      $(this).text(options.display_options.close_button_text);
                      $('#' + mapId).removeClass('element-invisible');
                    }
                    else {
                      $(this).text(options.display_options.open_button_text);
                      $('#' + mapId).addClass('element-invisible');
                    }
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

        $('div.open_yamap_button').click(function() {
          // @todo it also can be yamapsStatic, so we cannot be sure now which object to use.
          // @todo first we should define which object contains proper settings.
          // @todo better to add 'class' or another attr to the button: yamaps or yamapsStatic.
          // @todo check and rebuild map static open / close.
          // @todo the problem here is that after adding new item via ajax 'click' event called twice.
          var buttonMapId = $(this).attr('mapid');
          var buttonMapOptions = Drupal.settings.yamaps[buttonMapId];
          if ($('#' + buttonMapId).hasClass('element-invisible')) {
            $(this).text(buttonMapOptions.display_options.close_button_text);
            $('#' + buttonMapId).removeClass('element-invisible');
          }
          else {
            $(this).text(buttonMapOptions.display_options.open_button_text);
            $('#' + buttonMapId).addClass('element-invisible');
          }
        })
      })
    }
  }
})(jQuery);
