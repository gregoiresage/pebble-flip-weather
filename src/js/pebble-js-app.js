var locationOptions = {
	"timeout": 30000,
	"maximumAge": 30000
};

function getWeather(location, unit) {
	console.log("getWeather");
	var now = new Date();
	var weatherUrl = 'https://query.yahooapis.com/v1/public/yql?format=json&rnd='+now.getFullYear()+now.getMonth()+now.getDay()+now.getHours()+'&q=';
	if(location !== '') {
		weatherUrl += 'select * from weather.forecast where woeid in (select woeid from geo.placefinder where text="'+location+'" and gflags="R" limit 1) and u="'+unit+'"';
	} else {
		console.log("Could not retrieve weather due to an invalid location.");
		sendError();
		return;
	}

	console.log(weatherUrl);

	var req = new XMLHttpRequest();
	req.open('GET', encodeURI(weatherUrl), true);
	req.onload = function(e) {
		if (req.readyState == 4) {
			if (req.status == 200) {
				var response = JSON.parse(req.responseText);
				console.log(req.responseText);
				if (response) {
					var condition = response.query.results.channel.item.condition;
					console.log(response.query.results.channel.location.city + " --- Temperature " + condition.temp + " " + condition.code);
					MessageQueue.sendAppMessage({
						"weathercode" : parseInt(condition.code),
						"temperature" : parseInt(condition.temp),
						"cityname"	: response.query.results.channel.location.city
					});
				}
			} else {
				sendError();
			}
		}
	};
	req.send(null);
}

function updateWeather() {
	console.log("updateWeather");
	// if (options.gps === 1) {
		navigator.geolocation.getCurrentPosition(locationSuccess,locationError,locationOptions);
	// } else {
	// 	getWeather(options.location, autoconfigSettings.units == 1 ? 'c' : 'f');
	// }
}

function sendError(){
	console.log("Error");
	MessageQueue.sendAppMessage({
		"weathercode":3200,
		"temperature":0
	});
}

function locationSuccess(pos) {
	console.log("locationSuccess");
	getWeather(pos.coords.latitude+','+pos.coords.longitude, autoconfigSettings.units == 1 ? 'c' : 'f');
}

function locationError(err) {
	console.warn('locationError : ' + err.message);
	sendError();
}

function autoconfigReady(e){
	console.log("autoconfigReady");
	updateWeather();
}

function autoconfigShowConfiguration(e){
	console.log("autoconfigShowConfiguration");
}

function autoconfigWebviewclosed(e){
	console.log("autoconfigWebviewclosed");
	updateWeather();
}

Pebble.addEventListener("appmessage", function(e) {
	updateWeather();
});
