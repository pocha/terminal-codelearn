var sockjs = require('sockjs'),
	pty = require('pty.js'),
	Config = require('./config'),
	exec = require('child_process').exec,
	async = require('async'),
	mailer = require('./mailer');

var startTime = {};

var delay = {};
var ustartTime = {};
var msgs = {};
var avg_delay = 0;
var index = 0;

var end_of_output = /(\$|>)\s*$/;

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

	delay[conn.id] = 0;
	msgs[conn.id] = 0;	

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
			if(startTime[conn.id]){
				var endTime = process.hrtime(startTime[conn.id]);
				//console.log(endTime[0] * 1000 + endTime[1]/1000000);
				startTime[conn.id] = undefined;				
			}
			if(end_of_output.test(data) && ustartTime[conn.id]){				
				var timeEnd = process.hrtime(ustartTime[conn.id])		
				delay[conn.id] += timeEnd[0] * 1000 + timeEnd[1]/1000000;			
				msgs[conn.id]++;		
			}			
			//console.log(data);
			conn.write(data);
		});
	
		term.on('exit', function(code,signal){
			//console.log("Process "+term.pid+" exited with code: "+code+" and signal "+signal);
			if(conn.readyState == 1)
				//user typed exit	
				conn.close();
		});

		return term;

	} catch (err) {
		mailer.processError(err);
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
				ustartTime[conn.id] = process.hrtime();
				startTime[conn.id] = process.hrtime();
				term.write(message);	
			});
		});
	})
  
	conn.on('close', function() {

		avg_delay += delay[conn.id]/msgs[conn.id];
		index++;
		console.log("Avg Delay till client",index,"is",avg_delay/index);

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
