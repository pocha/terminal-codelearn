var sockjs = require('sockjs'),
	pty = require('pty.js'),
	Config = require('./config'),
	exec = require('child_process').exec,
	async = require('async'),
	mailer = require('./mailer'),
	db = require('./mongo');

var startTime = {};
var usernames = {};
var input = {};
var output = {};

var endOfOutput = /(\$|>)\s*$/;

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
	
	  
		console.log("Process started: " + term.pid);		

		term.on('data', function(data) {				
			output[conn.id] += data;

			if(startTime[conn.id]){
				endTime = process.hrtime(startTime[conn.id]);
				console.log(endTime[0] * 1000 + endTime[1]/1000000);
				startTime[conn.id] = undefined;				
			}

			if(endOfOutput.test(data) && input[conn.id]){
				db.insert(usernames[conn.id],input[conn.id],output[conn.id]);
				output[conn.id] = '';
			}			

			console.log(data);
			conn.write(data);
		});
	
		term.on('exit', function(code,signal){
			console.log("Process "+term.pid+" exited with code: "+code+" and signal "+signal);
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
		//prevent user from executing arbitary bash command
		message = /[a-z_][a-z0-9_]{0,30}/.exec(message);
		var username = message || process.env.USER;

		//intialize variables
		output[conn.id] = '';
		input[conn.id] = '';
		usernames[conn.id] = username; 

		getUidGid(username,function(uid,gid){
			term = new Terminal(conn,uid,gid);		
			conn.on('data', function(message){
				startTime[conn.id] = process.hrtime();
				input[conn.id] = message;
				term.write(message);	
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

		//Free up memory
		delete output[conn.id];
		delete input[conn.id];
		delete usernames[conn.id];
		delete startTime[conn.id];
	});

});

function monitorLog(severity,message){
	console.log(severity,":",message);
}

module.exports  = server;
