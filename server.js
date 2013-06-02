var http = require('http');
var sockjs = require('sockjs');

var echo = sockjs.createServer();
echo.on('connection', function(conn) {
    conn.on('data',function(message) {
      conn.write('Hi from server');
    });
    conn.on('close', function() {});
});

var server = http.createServer();
echo.installHandlers(server, {prefix:'/echo'});
server.listen(3000, '0.0.0.0')
