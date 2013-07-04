var Terminal = require('../lib/main');
var Config = require('../lib/config');
var jsdom = require('jsdom');
var async = require('async');
var jQuery = require('jQuery');
var fs = require('fs');
var exec = require('child_process').exec;
var spawn = require('child_process').spawn;

Terminal.listen();

var htmlFile = fs.readFileSync("./public/client.html").toString();

GLOBAL.SockJS = require('ws');
GLOBAL.SOCKET_URL = "ws://"+Config.host+":"+Config.port+Config.prefix+"/websocket"

GLOBAL.window = jsdom.jsdom(htmlFile).createWindow();

GLOBAL.$ = jQuery.create(window);

require("../public/assets/terminal-client.js");


describe('Connection',function(){

	before(function(done){
		check(done);
	});

	it("should return '"+process.env.USER+"' when I send 'whoami'",function(done){
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
	
	it("should stop the command execution when I press kill",function(done){
		var buff = '';

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
				var result = buff.split("\n")[0];
				buff = "";				
				result.should.match(/sleep\s*100/);				
				$('#kill').click();
				exec("ps -ef | grep 'sleep 100' | awk '{print $8"+'"\t"'+"$9}'",function(error,stdout,stderr){
					buff += stdout;
					callback();		
				});			
			},
			function(callback){				
				var result = buff.split("\n")[0];
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

	it("should close background processes and return a different pid when I press reset",function(done){

		var pid1, pid2,buff = '';

		async.series([
			function(callback){
				$("input[name='command']").val('echo $$');
				$('#execute').click();
				check(callback);	
			},
			function(callback){
				pid1 = parseInt($('#output').html().split('\n')[1]);
				$("input[name='command']").val('sleep 100 & sleep 1000 & sleep 10000 & whoami');
				$('#execute').click();
				check(callback);
			},
			function(callback){
				$('#reset').click();
				check(callback)			
			},
			function(callback){
				exec("ps -ef | grep "+pid1,function(error,stdout,stderr){
					buff += stdout;
					callback();		
				});			
			},
			function(callback){
				buff.split("\n").should.have.lengthOf(3);
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

	it("should be owned by '"+process.env.USER+"'",function(done){

		var buff = '';		

		async.series([
			function(callback){
				$("input[name='command']").val('echo $$');
				$('#execute').click();
				check(callback);	
			},
			function(callback){
				pid = parseInt($('#output').html().split('\n')[1]);
				exec("ps -ef | grep "+pid+" | head -1 | awk '{print $1}'", function(error,stdout,stderr){
					buff += stdout;
					callback();		
				});			
			},
			function(callback){
				var result = buff.split("\t");
				result[0].should.match(new RegExp(process.env.USER));
				callback();
			}
		],
		function(){
			$('#output').html('');
			done();	
		});

	});

	it("should display the connection closed message as well as close background processes when I send 'exit'",function(done){
		async.series([
			function(callback){
				$("input[name='command']").val('sleep 100 & sleep 1000 & sleep 10000 & whoami');
				$('#execute').click();
				check(callback);
			},
			function(callback){
				$("input[name='command']").val('exit');
				$('#execute').click();
				setTimeout(callback,50);
			},
			function(callback){
				$('#output').html().should.match(/connection to server closed/i);
				$('#reset').click();
				check(callback);
			}
		],
		function(){
			$('#output').html('');
			done();	
		});		
 
	});

	it("should display the could not connect message when I connect to already stopped server",function(done){
		async.series([
			function(callback){
				Terminal.close();
				$('#reset').click();
				setTimeout(callback,50);	
			},
			function(callback){
				$('#output').html().should.match(/could not connect/i);
				Terminal.listen(callback);
			},
			function(callback){
				$('#reset').click();
				check(callback);	
			},
		],
		function(){
			$('#output').html('');
			done();	
		});

	});
	
	it("should display connection closed message when I manually call the function passed to idle timer",function(done){
		async.series([
			function(callback){
				timerFired()
				setTimeout(callback,50);	
			},
			function(callback){
				$('#output').html().should.match(/connection to server closed/i);
				$('#reset').click();
				check(callback);
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
		},20);				
	} else {	
		callback();
	}
};

