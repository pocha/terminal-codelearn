var Terminal = require('../lib/Terminal.js');
var Config = require('../lib/Config.js');
var exec = require('child_process').exec;
var async = require('async');
var WebSocket = require('ws');


var buff = ""
var buff1 = ""

var  Client = function(done){

	socket =  new WebSocket("ws://"+Config.host+":"+Config.port+Config.prefix+"/websocket");
			
	socket.on('open',function(){
		buff="";			
		done();	
	});

	socket.on('message',function(data,flags){
		buff += data;
	});
	
	return socket;
}

describe('Connection',function(){

	var c1,c2;

	before(function(done){
		Terminal.listen(done);
	});

	before(function(done){
		c1=new Client(done);
	});

	it("it should return '"+process.env.USER+"' when I send 'whoami'",function(done){
		c1.send('whoami\r');
		async.series([
			function(callback){
				check(callback);			
			},
			function(callback){				
				result = buff.split("\n")[2];
				result.should.match(new RegExp(process.env.USER));
				buff ="";				
				callback();			
			}
		],
		function(){
			done();	
		});
	});

	it("it should stop the command execution when I send unicode for Ctrl+C ",function(done){
		async.series([
			function(callback){
				c1.send('sleep 100\r');
				setTimeout(callback,50);	
			},
			function(callback){
				exec("ps -ef | grep 'sleep 100' | awk '{print $8"+'"\t"'+"$9}'",function(error,stdout,stderr){
					buff1 += stdout;
					callback();		
				});			
			},
			function(callback){				
				result = buff1.split("\n")[0];
				buff1 = "";				
				result.should.match(/sleep\s*100/);				
				c1.send(String.fromCharCode(3));
				exec("ps -ef | grep 'sleep 100' | awk '{print $8"+'"\t"'+"$9}'",function(error,stdout,stderr){
					buff1 += stdout;
					callback();		
				});			
			},
			function(callback){				
				result = buff1.split("\n")[0];
				buff1 = "";				
				result.should.not.match(/sleep\s*100/);
				callback();
			}
		],
		function(){
			done();	
		});
		
	});
	
	it("it should return a different pid on a new connection ",function(done){

		var pid1, pid2;

		async.series([
			function(callback){
				buff=""
				c1.send('echo $$\r');
				check(callback);	
			},
			function(callback){
				pid1 = parseInt(buff.split('\n')[1]);
				c1.close();
				c2 = new Client(callback);
		
			},
			function(callback){
				check(callback)			
			},
			function(callback){				
				buff="";
				c2.send('echo $$\r');
				check(callback);	
			},
			function(callback){
				pid2 = parseInt(buff.split('\n')[1]);
				
				pid1.should.not.equal(pid2);
				
				callback();
			}
		],
		function(){
			done();	
		});
		
	});

	after(function(){
		Terminal.close();	
	});

});


function check(callback){
	
	var end_of_output = /(\$|>)\s*$/;	
	
	//console.log(buff);
	state = end_of_output.test(buff);

	if(!state){
		setTimeout(function(){
			check(callback);
		},25);
	} else {	
		callback();
	}
};
