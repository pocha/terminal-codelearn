var Terminal = require('../lib/Terminal.js');
var Config = require('../lib/Config.js');
var jsdom = require('jsdom');
var async = require('async');
var jQuery = require('jQuery');
var fs = require('fs');
var exec = require('child_process').exec;

Terminal.listen();

var htmlFile = fs.readFileSync("./public/client.html").toString();

	SockJS = require('ws');
	SOCKETURL = "ws://"+Config.host+":"+Config.port+Config.prefix+"/websocket"

window = jsdom.jsdom(htmlFile).createWindow();

$ = jQuery.create(window);

require("../public/assets/terminal-client.js");


describe('Connection',function(){

	before(function(done){
		check(done);
	});

	it("it should return '"+process.env.USER+"' when I send 'whoami'",function(done){
		$("input[name='command']").val('whoami');
		$('#execute').click();
		async.series([
			function(callback){
				check(callback);			
			},
			function(callback){				
				result = $('#output').html().split("\n")[1];
				result.should.match(new RegExp(process.env.USER));				
				callback();		
			}
		],
		function(){
			$('#output').html('');
			done();	
		});
	});
	
	it("it should stop the command execution when I press kill",function(done){
		var buff;

		async.series([
			function(callback){
				$("input[name='command']").val('sleep 100');
				$('#execute').click();
				setTimeout(callback,50);	
			},
			function(callback){
				exec("ps -ef | grep 'sleep 100' | awk '{print $8"+'"\t"'+"$9}'",function(error,stdout,stderr){
					buff += stdout;
					callback();		
				});			
			},
			function(callback){				
				result = buff.split("\n")[0];
				buff = "";				
				result.should.match(/sleep\s*100/);				
				$('#kill').click();
				exec("ps -ef | grep 'sleep 100' | awk '{print $8"+'"\t"'+"$9}'",function(error,stdout,stderr){
					buff += stdout;
					callback();		
				});			
			},
			function(callback){				
				result = buff.split("\n")[0];
				buff = "";				
				result.should.not.match(/sleep\s*100/);
				callback();
			}
		],
		function(){
			$('#output').html('');
			done();	
		});
		
	});

	it("it should return a different pid when I press reset",function(done){

		var pid1, pid2;

		async.series([
			function(callback){
				$("input[name='command']").val('echo $$');
				$('#execute').click();
				check(callback);	
			},
			function(callback){
				pid1 = parseInt($('#output').html().split('\n')[1]);
				$('#reset').click();
				callback();
			},
			function(callback){
				check(callback)			
			},
			function(callback){
				$('#output').html('');
				$("input[name='command']").val('echo $$');
				$('#execute').click();
				check(callback);
			},
			function(callback){
				pid2 = parseInt($('#output').html().split('\n')[1]);
	
				pid1.should.not.equal(pid2);				
				callback();
			}
		],
		function(){
			$('#output').html('');
			done();	
		});
		
	});

	after(function(){
		Terminal.close();	
	});
});

function check(callback){
	if($('#execute').is(':disabled')){
		setTimeout(function(){
			check(callback);
		},500);				
	} else {	
		callback();
	}
};

