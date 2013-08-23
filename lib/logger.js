var winston = require('winston');

var logger = new (winston.Logger)({
	transports:[
		new (winston.transports.Console)({
			level: 'debug'
		}),
		new (winston.transports.File)({
			name: 'file#all',
			filename: "out.log",
			level:'debug',
			json: false
		}),
		new (winston.transports.File)({
			name: 'file#error',
			filename: "err.log",
			level: 'error',
			json: false
		})
	]
});

module.exports = logger;
