var http = require('http');
var sockjs = require('sockjs');
var pty = require('pty.js');

var echo = sockjs.createServer();
echo.on('connection', function(conn) {
		
		var term = pty.spawn('bash', [], {
			name: 'xterm-color',
			cols: 80,
			rows: 30,
			cwd: process.env.HOME,
			env: process.env
		});
    
		term.on('data', function(data) {
  		conn.write(data);
		});

		conn.on('data',function(message) {
			console.log('Input :' + message);      
			term.write(message+'\r');
    });
    
		conn.on('close', function() {
			term.kill();					
		});
});

var server = http.createServer();
echo.installHandlers(server, {prefix:'/echo'});
server.listen(3000, '0.0.0.0')
