var SockJsServer = require('./SockJsServer'),
	HttpServer = require('./HttpServer'),
	Config = require('./Config');

exports.listen = function(callback){

	SockJsServer.installHandlers(HttpServer);
	HttpServer.listen(Config.port,Config.host,callback);
	
}

exports.close = function(){
	HttpServer.close();
}