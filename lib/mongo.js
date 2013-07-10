var mongo = require('mongodb'),
	mailer = require('./mailer');

var Server = mongo.Server,
    Db = mongo.Db,
    BSON = mongo.BSONPure;

var server  = new Server('localhost',27017, {auto_reconnect: true});
var db = new Db('test',server,{safe: false}); 


db.open(function(err,db){
	if(err) mailer.processError(err);
});


exports.insert = function(username,input,output){

	var log = {user: username, createdAt: new Date(), input: input, output: output};

	db.collection('commands', function(err,collection){
		collection.insert(log,function(err,result){
			if (err) mailer.processError(err);
		});
	});
} 
