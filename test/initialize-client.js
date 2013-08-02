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

require("../public/assets/ansi_to_html.js");
require("../public/assets/terminal-client.js");

GLOBAL.check = function (cond,callback){
	//console.log("output data - " + $("#output").text())
	if(cond()){
		setTimeout(function(){
			check(cond,callback);
		},100);				
	} else {
		callback();
	}
};

GLOBAL.ENTER_KEY = 13
GLOBAL.UP_KEY = 38
GLOBAL.DOWN_KEY = 40
GLOBAL.TAB_KEY = 9

GLOBAL.keyPress = function(keycode) {
	var e = jQuery.Event("keydown")
	e.which = keycode
	$("input[name='command']").trigger(e)
}

GLOBAL.initializeEnvironment = function() {
	$("#output").contents().filter(function(){
		return (this.nodeType == 3);
	}).remove();

	$("#output span").remove()
	$("input[name='command']").val("")
}

GLOBAL.checkPromptPostReset = function(done,user) {
	user = user || process.env.USER
	check(function() { return !$("#output").text().match(new RegExp(user)) } ,done)			
}

GLOBAL.checkLength = function(len){
	return function() { return ($('#output').text().split('\n').length < len) };
}
GLOBAL.checkMsgInOutput = function(msg) {
	return function() { return $("#output").text().match(new RegExp(msg)) == null  }
}

console.log("done")
