var http = require('http'),
	sockjs = require('sockjs'),
	pty = require('pty.js'),
	fs = require('fs'),
	fileStatic = require('node-static');

var echo = sockjs.createServer();
echo.on('connection', function(conn) {
		
		var term = pty.spawn('bash', [], {
			name: 'xterm-color',
			cols: 80,
			rows: 30,
			cwd: process.env.HOME,
			env: process.env
		});
	    
		console.log("Process started: " + term.pid);		

		term.on('data', function(data) {
  		conn.write(data);
		});
		
		term.on('exit', function(code,signal){
			console.log("Process "+term.pid+" exited with code: "+code+" and signal "+signal);
		});

		conn.on('data',function(message) {
			term.write(message+'\r');
    });
    
		conn.on('close', function() {
			term.write('exit\r');					
		});
});
var fileServer = new fileStatic.Server();

var server = http.createServer(function (request, response) {
	fileServer.serve(request, response, function (err, res) {
		if (err) {
				console.error("> Error serving " + request.url + " - " + err.message);
				response.writeHead(err.status, err.headers);
				response.end();
		} else { 
				console.log("> " + request.url + " - " + res.message);
		}
	});

});

echo.installHandlers(server, {prefix:'/echo'});
server.listen(3000, '0.0.0.0')
