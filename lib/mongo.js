var mongoose = require('mongoose'),
	mailer = require('./mailer');

var db = mongoose.connect('mongodb://localhost/test');

var logSchema = new mongoose.Schema({
	user: String,
	createdAt : { type: Date, default: Date.now },
	input: String,
	output: String
});

var Log = mongoose.model('Command',logSchema);

exports.insert = function(username,input,output){

	var log = new Log({user: username, input: input, output: output});

	log.save(function(err){
		if(err) mailer.processError(err);
	});

} 
