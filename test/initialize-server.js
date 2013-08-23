console.log("initializing server ...")
GLOBAL.Terminal = require('../lib/main');
GLOBAL.Config = require('../lib/config');
GLOBAL.exec = require('child_process').exec;
GLOBAL.spawn = require('child_process').spawn;
GLOBAL.mailer = require('../lib/mailer');

Terminal.listen();
console.log("done")
