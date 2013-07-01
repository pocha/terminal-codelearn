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

	it("should return a different pid when I press reset",function(done){

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

	it("should be owned by '"+process.env.USER+"' and have the node's script pid as its parent pid",function(done){

		var pid,buff = '';		

		async.series([
			function(callback){
				$("input[name='command']").val('echo $$');
				$('#execute').click();
				check(callback);	
			},
			function(callback){
				pid = parseInt($('#output').html().split('\n')[1]);
				exec("ps -ef | grep "+pid+" | head -1 | awk '{print $1"+'"\t"'+"$3}'", function(error,stdout,stderr){
					buff += stdout;
					callback();		
				});			
			},
			function(callback){
				var result = buff.split("\t");
				result[0].should.match(new RegExp(process.env.USER));
				var ppid = parseInt(result[1]);
				ppid.should.equal(process.pid);
				callback();
			}
		],
		function(){
			$('#output').html('');
			done();	
		});

	});

	it("should have exit in output and go button disabled when I send 'exit'",function(done){
		$("input[name='command']").val('exit');
		$('#execute').click();
		setTimeout(function(){
			var result  = $('#output').html().split("\n");
			result[0].should.match(/exit/);
			result[1].should.match(/exit/);
			$('#execute').is(':disabled').should.be.true;
			$('#output').html('');
			done();			
		},50);	
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

