GLOBAL.Terminal = require('../lib/main');
GLOBAL.Config = require('../lib/config');
GLOBAL.jsdom = require('jsdom');
GLOBAL.async = require('async');
GLOBAL.jQuery = require('jQuery');
GLOBAL.fs = require('fs');
GLOBAL.exec = require('child_process').exec;
GLOBAL.spawn = require('child_process').spawn;
GLOBAL.mailer = require('../lib/mailer');

Terminal.listen();

var htmlFile = fs.readFileSync("./public/client.html").toString();

GLOBAL.SockJS = require('ws');
GLOBAL.SOCKET_URL = "ws://"+Config.host+":"+Config.port+Config.prefix+"/websocket"

GLOBAL.window = jsdom.jsdom(htmlFile).createWindow();

GLOBAL.$ = jQuery.create(window);

require("../public/assets/terminal-client.js");

GLOBAL.check = function (cond,callback){
	if(cond()){
		setTimeout(function(){
			check(cond,callback);
		},20);				
	} else {	
		callback();
	}
};

GLOBAL.buttonDisabled = function(){
	return $('#execute').is(':disabled')
}

GLOBAL.checkLength = function(len){
	return ($('#output').html().split('\n').length < len);
}
