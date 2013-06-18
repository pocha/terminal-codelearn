var http = require('http'),
	fs = require('fs'),
	fileStatic = require('node-static');

var fileServer = new fileStatic.Server('./public', { cache: 3600 });

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

module.exports = server;
