/*jslint sub: true*/

// Import the Clay package
var Clay = require('pebble-clay');
// Load our Clay configuration file
var clayConfig = require('./config');
// Initialize Clay
var clay = new Clay(clayConfig);

/*json file looks like
{"A":"1", "B":"1", "C":"0", D":"0"}
up to 4 entries, remove any not required ie
{"A":"1"} 
*/

var myAPIKey = localStorage.getItem('apikey');
var mypplurl = localStorage.getItem('ppl');
var latitude = localStorage.getItem('latitude');
var longitude = localStorage.getItem('longitude');
var defaultlocOnly = localStorage.getItem('defaultlocOnly');
var metric = localStorage.getItem('metric');
var ppl_disable = localStorage.getItem('ppldisable');
var refresh = localStorage.getItem('refresh');
var vibration = localStorage.getItem('vibration');
var ppl_dict = localStorage.getItem('ppl_dict');
var weather_dict = localStorage.getItem('weather_dict');
var watch_dict = localStorage.getItem('watch_dict');
var batt = require('./battery.js');

var xhrRequest = function(url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function() {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function clone(obj) {
    if (null === obj || "object" != typeof obj) return obj;
    var copy = obj.constructor();
    for (var attr in obj) {
        if (obj.hasOwnProperty(attr)) copy[attr] = clone(obj[attr]);
    }
    return copy;
}

function convert(val) {
  if (metric) {
    val = (val-32)/1.8;
  }
  return Math.round(val);
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
   // Init the battery service
   
     batt.Battery_Init();
    
  if (!ppl_disable){
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
        dictionary.KEY_PPL_TOTAL = ""+Object.keys(ppljson).length;
        if (JSON.stringify(dictionary)!=ppl_dict) {
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
            localStorage.ppl_dict = JSON.stringify(clone(dictionary));

        }
      }
    );
  }
  if (weather && myAPIKey != null) {
  // Construct URL
  //var url = 'https://api.forecast.io/forecast/' +
  // myAPIKey + '/' pos.coords.latitude + ',' + pos.coords.longitude + '?exclude=minutely,hourly,alerts,flags';
  var url = 'https://api.forecast.io/forecast/' +
    myAPIKey + '/' + latitude + ',' + longitude;
    console.log('url is' + url);

  // Send request to Forecast.io
  xhrRequest(url, 'GET',
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);
      var temperature = null;
      var highTemperature = null;
      var lowTemperature = null;
      var precip = null;
      var conditions = null;
      if(typeof json.currently != "undefined"){
          // Temperature, current, high, and low
          temperature = parseFloat(json.currently.temperature).toFixed(1);
          console.log('Temperature is ' + temperature);        
          conditions = json.currently.icon;
          console.log(' Conditions are ' + conditions);
      }
      if (typeof json.daily != "undefined"){
      if (typeof json.daily.data[0] != "undefined"){
          highTemperature = parseFloat(json.daily.data[0].temperatureMax).toFixed(1);
          console.log(' High Temperature is ' + highTemperature);
          lowTemperature = parseFloat(json.daily.data[0].temperatureMin).toFixed(1);
          console.log(' Low Temperature is ' + lowTemperature);
          precip = Math.round(json.daily.data[0].precipProbability * 100);
          console.log(' Precip Prob is '+ precip);
      }}
      // Conditions
    
      var dictionary = {
        "KEY_TEMP": convert(temperature),
        "KEY_HIGHTEMP": convert(highTemperature),
        "KEY_LOWTEMP": convert(lowTemperature),
        "KEY_COND": conditions
        
      };
      
      if (JSON.stringify(dictionary)!=weather_dict) {
          //console.log(dictionary);
          Pebble.sendAppMessage(dictionary,
            function(e) {
              console.log('Weather info sent to Pebble successfully!');
            },
    
            function(e) {
              console.log('Error sending weather info to Pebble!');
            }
          );
          localStorage.weather_dict = JSON.stringify(clone(dictionary));
      }
      
    }
  );
  }
}


function getWeather() {
console.log(defaultlocOnly + ' is ' + typeof defaultlocOnly);
  if (defaultlocOnly) {
    getPeople(latitude, longitude, true);
    console.log('getting people with default loc.');
  } else {
    console.log('getting position for non-default weather');
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

Pebble.addEventListener('webviewclosed', function(e) {
  var claySettings = clay.getSettings(e.response, false);
  console.log('String:' + JSON.stringify(claySettings));
    
  mypplurl = claySettings.ppl.value;
  myAPIKey = claySettings.apikey.value;
  metric = claySettings.metric.value;
  longitude = claySettings.longitude.value;
  latitude = claySettings.latitude.value;
  defaultlocOnly = claySettings.defaultLocOnly.value;
  refresh = claySettings.refresh.value;
  vibration = claySettings.vibrate.value;
  ppl_disable = claySettings.ppldisable.value;
  localStorage.apikey = myAPIKey;
  localStorage.ppl = mypplurl;
  localStorage.metric = metric;
  localStorage.longitude = longitude;
  localStorage.latitude = latitude;
  localStorage.defaultlocOnly = defaultlocOnly;
  console.log("API: " + myAPIKey);
  send_to_c();
  getWeather();

});

function send_to_c(){
  var dictionary = {
    "KEY_VIBRATE": ""+vibration,
    "KEY_REFRESH": ""+refresh,
    "KEY_PPL": ""+ppl_disable
  };
    if (JSON.stringify(dictionary)!=watch_dict) {

      Pebble.sendAppMessage(dictionary,
                            function(e) {
                              console.log('Config info sent to Pebble successfully!');
                            },
    
                            function(e) {
                              console.log('Error sending config info to Pebble!');
                            }
                           );
        localStorage.watch_dict = JSON.stringify(clone(dictionary));
    }
}
// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    getWeather();

  }
);

