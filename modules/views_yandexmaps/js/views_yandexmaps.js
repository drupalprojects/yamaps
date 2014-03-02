        // Как только будет загружен API и готов DOM, выполняем инициализацию

//ymaps.ready(init);
alert('myymap');

ymaps.ready(init);
var myMap;

function init(){
  myMap = new ymaps.Map ("myymap", {
    center: [55.76, 37.64],
    zoom: 7
  });
}
/*
function init() {
  var myMap = new ymaps.Map("myymap", {
    center: [53.91, 27.53],
    zoom: 12,
    type: "yandex#map"
  });

  myMap.controls.add(new ymaps.control.TrafficControl({providerKey: 'traffic#archive'}));
  myMap.controls.add(new ymaps.control.ZoomControl());
  myMap.controls.add(new ymaps.control.TypeSelector(['yandex#map', 'yandex#publicMap']));
  myMap.controls.add(new ymaps.control.MapTools());
  var geo_objects = Drupal.settings.geo_objects;

  myCollection = new ymaps.GeoObjectCollection();

  for (var i = 0; i < geo_objects.length; i++) {


    myPlacemark = new ymaps.Placemark([geo_objects[i][0], geo_objects[i][1]], {
      title: geo_objects[i][2],
      url: geo_objects[i][3],
      descr: geo_objects[i][4]
    }, {preset: 'twirl#houseIcon'});
    myCollection.add(myPlacemark);
  }

  // Создаем шаблон для отображения контента балуна
  var myBalloonLayout = ymaps.templateLayoutFactory.createClass(
    '<h6><a href="$[properties.url]">$[properties.title]</a></h6>' +
      '<p>$[properties.descr]</p>' +
      '<p><div class="field field-name-book-flat field-type-ds field-label-hidden"><div class="field-items"><div class="field-item even"><a class="book-flat" rel="nofollow" href="/content/zabronirovat-kvartiru?flat_title=$[properties.title]">Забронировать</a></div></div></div><a href="$[properties.url]">Подробнее</a></p>'

  );


  // Помещаем созданный шаблон в хранилище шаблонов. Теперь наш шаблон доступен по ключу 'my#superlayout'.
  ymaps.layout.storage.add('my#superlayout', myBalloonLayout);

  // Задаем наш шаблон для балунов геобъектов коллекции.
  myCollection.options.set({
    balloonContentBodyLayout: 'my#superlayout',
    // Максимальная ширина балуна в пикселах
    balloonMaxWidth: 300
  });
  myMap.geoObjects.add(myCollection);


}
*/

