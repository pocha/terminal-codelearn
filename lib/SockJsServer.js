var sockjs = require('sockjs'),
	pty = require('pty.js'),
	Config = require('./Config.js');


var server = sockjs.createServer({
					prefix: Config.prefix,
					log: monitorLog
				});
	
server.on('connection', function(conn) {

	var term = pty.spawn('bash', [], {
		name: 'xterm-color',
		cols: 80,
		rows: 30,
		cwd: process.env.HOME,
		env: process.env
	});
	  
	//console.log("Process started: " + term.pid);		

	term.on('data', function(data) {				
		//console.log(data);				
		conn.write(data);
	});

	term.on('exit', function(code,signal){
		//console.log("Process "+term.pid+" exited with code: "+code+" and signal "+signal);
	});

	conn.on('data',function(message) {
		//console.log('Input: ',message);				
		term.write(message);
  });
  
	conn.on('close', function() {
		term.end();					
	});

});

function monitorLog(severity,message){
	
}

module.exports  = server;
