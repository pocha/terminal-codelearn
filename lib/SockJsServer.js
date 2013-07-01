var sockjs = require('sockjs'),
	pty = require('pty.js'),
	Config = require('./Config.js'),
	exec = require('child_process').exec,
	async = require('async');


var server = sockjs.createServer({
					prefix: Config.prefix,
					log: monitorLog
				});

function getUidGid(username,callback){

	var uid,gid;

	async.parallel([
		function(callback){
			exec("id -u " + username,function(error,stdout,stderr){
				callback(null,stdout);		
			});
		},
		function(callback){
			exec("id -g " + username,function(error,stdout,stderr){
				callback(null,stdout);		
			});	
		}
	],	
	function(err,results){
		uid = parseInt(results[0]);
		gid = parseInt(results[1]);		
		callback(uid,gid);
	});
};

var Terminal = function(conn,userid,groupid){
	var term = pty.spawn('bash', [], {
		name: 'xterm-color',
		cols: 80,
		rows: 30,
		cwd: process.env.HOME,
		uid: userid,
		gid: groupid
	});

	  
	//console.log("Process started: " + term.pid);		

	term.on('data', function(data) {				
		//console.log(data);				
		conn.write(data);
	});

	term.on('exit', function(code,signal){
		//console.log("Process "+term.pid+" exited with code: "+code+" and signal "+signal);
	});

	return term;
};
	
server.on('connection', function(conn) {
	
	var term;

	conn.once('data',function(message) {
		getUidGid(message || process.env.USER ,function(uid,gid){

			term = new Terminal(conn,uid,gid);		
			conn.on('data', function(message){
				term.write(message);	
			});

		});
	})
  
	conn.on('close', function() {
		term.destroy();
	});

});

function monitorLog(severity,message){
	console.log(severity,":",message);
}

module.exports  = server;
