/*json file looks like
{"A":"1", "B":"1", "C":"0", D":"0"}
up to 4 entries, remove any not required ie
{"A":"1"} 
*/
var myAPIKey = 'FORCAST IO API';
var mypplurl = 'URL OF JSON';
var latitude = -30.123;
var longitude = 142.111;
var defaultlocOnly = true;
var metric = false;

var xhrRequest = function(url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function() {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function convert(val) {
  var out = val;
  if (metric) {
    out = (val-32)/1.8;
  }
  return out;
}

function locFailure() {
  if (latitude && longitude){
    getPeople(latitude, longitude, true);
  } else {
    getPeople(latitude, longitude, false);
  }
}

function locSuccess(pos) {
  getPeople(pos.coords.latitude, pos.coords.longitude,true);
}

function getPeople(latitude, longitude, weather) {

  console.log('getting people and weather!');
  xhrRequest(mypplurl, 'GET',
    function(responseText) {
      var ppljson = JSON.parse(responseText);
      var dictionary = {};
      for (var i=0; i<Object.keys(ppljson).length; i++) {
        var name = Object.keys(ppljson)[i];
        dictionary["KEY_NAME"+(i+1)] = name;
        var val = ppljson[Object.keys(ppljson)[i]];
        dictionary["KEY_VALUE"+(i+1)] = val;
        console.log(name + ':' + val);
      }
      console.log('got people');

      //console.log(dictionary);
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
  if (weather) {
  // Construct URL
  //var url = 'https://api.forecast.io/forecast/' +
  // myAPIKey + '/' pos.coords.latitude + ',' + pos.coords.longitude + '?exclude=minutely,hourly,alerts,flags';
  var url = 'https://api.forecast.io/forecast/' +
    myAPIKey + '/' + latitude + ',' + longitude;

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
      var precip = json.daily.data[0].precipProbability * 100;
      console.log(' Precip Prob is '+ precip);
      // Conditions
      var conditions = json.currently.icon;
      console.log('Conditions are ' + conditions);
    
      var dictionary = {
        "KEY_TEMP": convert(temperature),
        "KEY_HIGHTEMP": convert(highTemperature),
        "KEY_LOWTEMP": convert(lowTemperature),
        "KEY_COND": conditions
      };
      //console.log(dictionary);
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log('Weather info sent to Pebble successfully!');
        },

        function(e) {
          console.log('Error sending weather info to Pebble!');
        }
      );
    }
  );
  }
}


function getWeather() {
  if (defaultlocOnly) {
    getPeople(latitude, longitude, true);
  } else {
    navigator.geolocation.getCurrentPosition(
      locSuccess,
      locFailure, {
        timeout: 15000,
        maximumAge: 0
      }
    );
  }
  
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

