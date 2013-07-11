console.log("initializing client")

GLOBAL.jsdom = require('jsdom');
GLOBAL.async = require('async');
GLOBAL.jQuery = require('jQuery');
GLOBAL.fs = require('fs');

var htmlFile = fs.readFileSync("./public/client.html").toString();

GLOBAL.SockJS = require('ws');
GLOBAL.SOCKET_URL = "ws://"+Config.host+":"+Config.port+Config.prefix+"/websocket"
GLOBAL.USERNAME = (typeof username != "undefined" && username) || ""
console.log("here")
GLOBAL.SIGNATURE = (typeof signature != "undefined" && signature) || ""
GLOBAL.client1 //to be used to test if websocket is closed in mocha tests

GLOBAL.window = jsdom.jsdom(htmlFile).createWindow();

GLOBAL.$ = jQuery.create(window);

require("../public/assets/terminal-client.js");

GLOBAL.check = function (cond,callback){
	console.log("output data - " + $("#output").html())
	if(cond()){
		setTimeout(function(){
			check(cond,callback);
		},100);				
	} else {
		callback();
	}
};

GLOBAL.buttonDisabled = function(){
	return $('#execute').is(':disabled')
}

GLOBAL.checkLength = function(len){
	return function() { return ($('#output').html().split('\n').length < len) };
}
GLOBAL.checkMsgInOutput = function(msg) {
	return function() { return $("#output").html().match(new RegExp(msg)) == null  }
}

console.log("done")
