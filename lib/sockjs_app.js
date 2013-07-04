var sockjs = require('sockjs'),
	pty = require('pty.js'),
	Config = require('./config'),
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
	
	try{

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

	} catch (err) {
		//TODO Send error that pty spawn failed
		console.log(err);
		conn.close();
	}
};
	
server.on('connection', function(conn) {
	
	var term;

	conn.once('data',function(message) {
		message = /[a-z_][a-z0-9_]{0,30}/.exec(message);
		getUidGid(message || process.env.USER ,function(uid,gid){
			term = new Terminal(conn,uid,gid);		
			conn.on('data', function(message){
				if(message == 'exit\r') conn.close();
				else term.write(message);	
			});
		});
	})
  
	conn.on('close', function() {
		if(term){
			//Kills only the immediate children
			exec("pkill -TERM -P " + term.pid,function(error,stdout,stderr){
				term.destroy();		
			});
		}
	});

});

function monitorLog(severity,message){
	console.log(severity,":",message);
}

module.exports  = server;
