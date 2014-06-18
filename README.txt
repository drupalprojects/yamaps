
-- SUMMARY --

Module for Drupal 7.
Uses http://api.yandex.ru/maps/ (API 2.x)

For a full description of the module, visit the project page:
  https://drupal.org/project/yamaps

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/yamaps

-- OPTIONS --

* Change the type, size and center of the map.
* Add placemarks, select label color, change texts, add labels using the search string
* Draw polylines, chose colors, transparency, line width, text
* Draw polygons, chose colors and line width, fill color, transparency, texts
* Add a route
* Displays traffic jams

-- REQUIREMENTS --

Requires core 'block' and 'field' modules.

-- INSTALLATION AND UNINSTALLATION --

Install and uninstall as usual.
See http://drupal.org/documentation/install/modules-themes/modules-7 for further information.

-- CONFIGURATION --

Provides configurable "Yandex Maps" field, which can be added to any type of Drupal content.

Provides configurable amount of "Yandex Maps" blocks to display maps in any regions of the website.
To add "Yandex Maps" block perform following steps:

* navigate to /admin/config/services/yamaps and set required amount of "Yandex Maps" blocks
* navigate to /admin/structure/block, scroll down to find "Disabled" section
  and find block called "Yandex Map #{NUMBER}"
* configure the block and pull it into the required region.
