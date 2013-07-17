var SockJsServer = require('./sockjs_app'),
	Config = require('./config'),
	http = require('http'),
	fs = require('fs'),
	fileStatic = require('node-static'),
	mailer = require('./mailer'),
	logger = require('./logger');

var fileServer = new fileStatic.Server('./public', { cache: 3600 });


process.on('uncaughtException',function(err){
	mailer.processError(err);
});



var server = http.createServer(function (request, response) {
	fileServer.serve(request, response, function (err, res) {
		if (err) {
				logger.log('error',"> Error serving " + request.url + " - " + err.message);
				response.writeHead(err.status, err.headers);
				response.end();
		} else { 
				logger.log('degub',"> " + request.url + " - " + res.message);
		}
	});

});

SockJsServer.installHandlers(server);

exports.listen = function(callback){
	server.listen(Config.port,Config.host,callback);
}

exports.close = function(){
	server.close();
}
