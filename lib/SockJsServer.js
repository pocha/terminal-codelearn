var sockjs = require('sockjs'),
	pty = require('pty.js'),
	Config = require('./Config.js'),
	fs = require('fs');


var delay = {};
var startTime = {};
var msgs = {};
var avg_delay = 0;
var index = 0;

var end_of_output = /(\$|>)\s*$/;


var server = sockjs.createServer({
					prefix: Config.prefix,
					log: monitorLog
				});
	
server.on('connection', function(conn) {

	delay[conn.id] = 0;
	msgs[conn.id] = 0;

	var term = pty.spawn('bash', [], {
		name: 'xterm-color',
		cols: 80,
		rows: 30,
		cwd: process.env.HOME,
		env: process.env
	});

	term.on('data', function(data) {				
		if(end_of_output.test(data) && startTime[conn.id]){				
			var timeEnd = process.hrtime(startTime[conn.id])		
			delay[conn.id] += timeEnd[0] * 1000 + timeEnd[1]/1000000;			
			msgs[conn.id]++;		
		}		
		conn.write(data);
	});

	term.on('exit', function(code,signal){
		//console.log("Process "+term.pid+" exited with code: "+code+" and signal "+signal);
	});

	conn.on('data',function(message) {
		startTime[conn.id] = process.hrtime();
		term.write(message);
  });
  
	conn.on('close', function() {		
		avg_delay += delay[conn.id]/msgs[conn.id];
		index++;
		console.log("Avg Delay till client",index,"is",avg_delay/index);
		term.end();					
	});

});

function monitorLog(severity,message){
	console.log(severity,":",message);
}

module.exports  = server;
