var sockjs = require('sockjs'),
	pty = require('pty.js'),
	Config = require('./config'),
	exec = require('child_process').exec,
	async = require('async'),
	mailer = require('./mailer'),
	db = require('./mongo'),
	crypto = require('crypto'),
	logger = require('./logger');

var startTime = {};
var usernames = {};
var input = {};
var output = {};

var endOfOutput = /(\$|>)\s*$/;

var TIMER = 15*60*1000;

var server = sockjs.createServer({
					prefix: Config.prefix,
					log: monitorLog
				});



var Terminal = function(conn,user){
	
	try{
		var term

		if (user == process.env.USER) {
			term = pty.spawn('bash', [], {
				name: 'xterm-color',
				cols: 80,
				rows: 30
			});
		}
		else {
			term = pty.spawn('su', ['-l',user], {
				name: 'xterm-color',
				cols: 80,
				rows: 30
			});
		}
	
	  
		logger.log('info',"Process started: " + term.pid);		


		term.on('data', function(data) {				
			output[conn.id] += data;

			if(startTime[conn.id]){
				endTime = process.hrtime(startTime[conn.id]);
				logger.log('info',endTime[0] * 1000 + endTime[1]/1000000);
				startTime[conn.id] = undefined;				
			}

			if(endOfOutput.test(data) && input[conn.id]){
				db.insert(usernames[conn.id],input[conn.id],output[conn.id]);
				output[conn.id] = '';
			}			

			logger.log('debug',data);
			conn.write(data);
		});
	
		term.on('exit', function(code,signal){
			logger.log('info',"Process "+term.pid+" exited with code: "+code+" and signal "+signal);
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
	var inactive;
	
	conn.once('data',function(message) {
		options = JSON.parse(message)
		if (options['user']) {
			var hmac = crypto.createHmac('sha1', Config.secret)
			hmac.update(options['user'])
			if (options['signature'] != hmac.digest('hex')) {
				conn.close();
				return;
			}
		}

		//prevent user from executing arbitary bash command
		var username = /[a-z_][a-z0-9_]{0,30}/.exec(options['user']) || process.env.USER;

		//intialize variables
		output[conn.id] = '';
		input[conn.id] = '';
		usernames[conn.id] = username; 

		term = new Terminal(conn,username);
		
		
		inactive = setTimeout(function(){
			if(conn.readyState == 1)
				conn.close();
		},TIMER);
				
		conn.on('data', function(message){
			clearTimeout(inactive);
			
			startTime[conn.id] = process.hrtime();
			input[conn.id] = message;
			term.write(message);
			
			inactive = setTimeout(function(){
				if(conn.readyState == 1)
					conn.close();
			},TIMER);
				
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
	logger.log(severity,message);
}

module.exports  = server;
