var use_celsius = true;

var CLEAR_DAY = 0;
var CLEAR_NIGHT = 1;
var WINDY = 2;
var COLD = 3;
var PARTLY_CLOUDY_DAY = 4;
var PARTLY_CLOUDY_NIGHT = 5;
var HAZE = 6;
var CLOUD = 7;
var RAIN = 8;
var SNOW = 9;
var HAIL = 10;
var CLOUDY = 11;
var STORM = 12;
var NA = 13;

var imageId = {
  0 : STORM, //tornado
  1 : STORM, //tropical storm
  2 : STORM, //hurricane
  3 : STORM, //severe thunderstorms
  4 : STORM, //thunderstorms
  5 : HAIL, //mixed rain and snow
  6 : HAIL, //mixed rain and sleet
  7 : HAIL, //mixed snow and sleet
  8 : HAIL, //freezing drizzle
  9 : RAIN, //drizzle
  10 : HAIL, //freezing rain
  11 : RAIN, //showers
  12 : RAIN, //showers
  13 : SNOW, //snow flurries
  14 : SNOW, //light snow showers
  15 : SNOW, //blowing snow
  16 : SNOW, //snow
  17 : HAIL, //hail
  18 : HAIL, //sleet
  19 : HAZE, //dust
  20 : HAZE, //foggy
  21 : HAZE, //haze
  22 : HAZE, //smoky
  23 : WINDY, //blustery
  24 : WINDY, //windy
  25 : COLD, //cold
  26 : CLOUDY, //cloudy
  27 : CLOUDY, //mostly cloudy (night)
  28 : CLOUDY, //mostly cloudy (day)
  29 : PARTLY_CLOUDY_NIGHT, //partly cloudy (night)
  30 : PARTLY_CLOUDY_DAY, //partly cloudy (day)
  31 : CLEAR_NIGHT, //clear (night)
  32 : CLEAR_DAY, //sunny
  33 : CLEAR_NIGHT, //fair (night)
  34 : CLEAR_DAY, //fair (day)
  35 : HAIL, //mixed rain and hail
  36 : CLEAR_DAY, //hot
  37 : STORM, //isolated thunderstorms
  38 : STORM, //scattered thunderstorms
  39 : STORM, //scattered thunderstorms
  40 : STORM, //scattered showers
  41 : SNOW, //heavy snow
  42 : SNOW, //scattered snow showers
  43 : SNOW, //heavy snow
  44 : CLOUD, //partly cloudy
  45 : STORM, //thundershowers
  46 : SNOW, //snow showers
  47 : STORM, //isolated thundershowers
  3200 : NA, //not available
};

function getWeatherFromLocation(location_name) {
    var response;
    var woeid = -1;
    var query = encodeURI("select woeid from geo.places(1) where text=\"" + location_name + "\"");
    var url = "http://query.yahooapis.com/v1/public/yql?q=" + query + "&format=json";

    console.log(url);
    
    var req = new XMLHttpRequest();
    req.open('GET', url, true);
    req.onload = function(e) {
        if (req.readyState == 4) {
            if (req.status == 200) {
                console.log(req.responseText);
                response = JSON.parse(req.responseText);
                if (response) {
                    woeid = response.query.results.place.woeid;
                    console.log("Got WOEID of " + woeid);
                    getWeatherFromWoeid(woeid);
                }
            }
            else {
                console.log("Error in getting weather from location: " + e);
            }
        }
    };
    req.send(null);
}

function getWeatherFromWoeid(woeid) {
    var query = encodeURI("select item.condition from weather.forecast where woeid = " + woeid + " and u = " + (false ? "\"c\"" : "\"f\""));
    var url = "http://query.yahooapis.com/v1/public/yql?q=" + query + "&format=json";
    
    console.log(url);

    var response;
    var req = new XMLHttpRequest();
    req.open('GET', url, true);
    req.onload = function(e) {
        if (req.readyState == 4) {
            if (req.status == 200) {
                console.log(req.responseText);
                response = JSON.parse(req.responseText);
                if(response){
                    var condition = response.query.results.channel.item.condition;
                    var icon = imageId[parseInt(condition.code)];

                    console.log("temp " + condition.temp);
                    console.log("icon " + condition.code);
                    console.log("icon " + icon);
                    console.log("condition " + condition.text);

                    var fixedTemp = parseInt(condition.temp);
                    if(use_celsius){
                        console.log("User is using the best temperature scale");
                        fixedTemp -= 32;
                        fixedTemp = Math.ceil(fixedTemp*(5/9));
                    }
                    var message = {
                        "icon" : icon,
                        "temperature" : fixedTemp
                    };
                    console.log("Sending message through WOEID " + JSON.stringify(message));
                    Pebble.sendAppMessage(message);
                }
            } else {
                console.log("Error from getting through WOEID: " + e);
            }
        }
    };
    req.send(null);
}
Pebble.addEventListener("ready", function(e){
  console.log("Ready for interaction!");  
});

Pebble.addEventListener("appmessage", function(e){
    console.log("Got message from app: " + e.payload.getWeather);  
    
    getWeatherFromLocation(e.payload.getWeather);
    /* var object = {
        testkey: 123
    };
    Pebble.sendAppMessage(object);
    */ 
});