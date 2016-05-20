var myAPIKey = 'CHANGE_THIS_BEFORE_USE';
var sarah;
var melissa;
var alec;
var zack;
var batt;
var xhrRequest = function(url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function() {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locFailure() {
  console.log('getting people!');
  var url = 'URL_OF_JSON_STRING';
  xhrRequest(url, 'GET',
    function(responseText) {
      var ppljson = JSON.parse(responseText);
      sarah = ppljson.sarah;
      melissa = ppljson.melissa;
      alec = ppljson.alec;
      zack = ppljson.zack;
      batt = ppljson.batt;
      console.log('got people');
      console.log(sarah);
      console.log(melissa);
      console.log(alec);
      console.log(zack);
      console.log(batt);
      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_ZACK": zack,
        "KEY_SARAH": sarah,
        "KEY_ALEC": alec,
        "KEY_MELISSA": melissa,
        "KEY_BATT": batt
      };

      console.log(dictionary);
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log('People info sent to Pebble successfully!');
        },

        function(e) {
          console.log('Error sending people info to Pebble!');
        }
      );
    }
  );
}

function getPeople(pos) {

  console.log('getting people and weather!');
  var url = 'URL_OF_JSON_STRING';
  xhrRequest(url, 'GET',
    function(responseText) {
      var ppljson = JSON.parse(responseText);
      sarah = ppljson.sarah;
      melissa = ppljson.melissa;
      alec = ppljson.alec;
      zack = ppljson.zack;
      batt = ppljson.batt;
      console.log('got people');
      console.log(sarah);
      console.log(melissa);
      console.log(alec);
      console.log(zack);
      console.log(batt);

    }
  );

  // Construct URL
  //var url = 'https://api.forecast.io/forecast/' +
  // myAPIKey + '/' pos.coords.latitude + ',' + pos.coords.longitude + '?exclude=minutely,hourly,alerts,flags';
  var url = 'https://api.forecast.io/forecast/' +
    myAPIKey + '/' + pos.coords.latitude + ',' + pos.coords.longitude;

  // Send request to Forecast.io
  xhrRequest(url, 'GET',
    function(responseText) {
      // responseText contains a JSON object with weather info
      console.log(responseText);
      var json = JSON.parse(responseText);

      // Temperature, current, high, and low
      var temperature = json.currently.temperature;
      console.log('Temperature is ' + temperature);
      var highTemperature = json.daily.data[0].temperatureMax;
      console.log(' High Temperature is ' + highTemperature);
      var lowTemperature = json.daily.data[0].temperatureMin;
      console.log(' High Temperature is ' + lowTemperature);
      // Conditions
      var conditions = json.currently.icon;
      console.log('Conditions are ' + conditions);

      var dictionary = {
        "KEY_ZACK": zack,
        "KEY_SARAH": sarah,
        "KEY_ALEC": alec,
        "KEY_MELISSA": melissa,
        "KEY_BATT": batt,
        "KEY_TEMP": temperature,
        "KEY_HIGHTEMP": highTemperature,
        "KEY_LOWTEMP": lowTemperature,
        "KEY_COND": conditions
      };
      console.log(dictionary);
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log('People info sent to Pebble successfully!');
        },

        function(e) {
          console.log('Error sending people info to Pebble!');
        }
      );
    }
  );
}

//function locationError(err) {
//  console.log("Error requesting location!");
//}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    getPeople,
    locFailure, {
      timeout: 15000,
      maximumAge: 0
    }
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready',
  function(e) {
    console.log("PebbleKit JS ready!");

    // Get the initial weather
    getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    getWeather();

  }
);
