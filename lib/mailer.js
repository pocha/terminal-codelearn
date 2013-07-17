var nodemailer = require('nodemailer');
var fs = require('fs');
var logger = require('./logger');

var smtpTransport= nodemailer.createTransport("Stub");

//Uncomment the following lines to send error mails via Gmail
// Also delete the above line
/*

var smtpTransport= nodemailer.createTransport("SMTP",{
	service : "Gmail",
	auth: {
		user: "someusername@gmail.com",
		pass: "secretpass"
	}
});

*/

exports.processError = function(err,callback){
	logger.log('error',err.stack);
	sendMail(err.message,err.stack,callback);
}

var sendMail = function(subject,body,callback){
	smtpTransport.sendMail(
	{
		from: "Aviral Agarwal <aviral12028@gmail.com>",
		to:   "aviral12028@gmail.com",
		subject: subject,
		text: body
	},
	function(error, response){
		if(error){
			fs.appendFileSync('email.log',"\n"+ getDateTime() + error);	
		} else {
			fs.appendFileSync('email.log',"\n"+ getDateTime() + response.message);		
		}
		if(callback) callback();
	});
};

function getDateTime() {

    var date = new Date();
    var hour = date.getHours();
    hour = (hour < 10 ? "0" : "") + hour;
    var min  = date.getMinutes();
    min = (min < 10 ? "0" : "") + min;
    var sec  = date.getSeconds();
    sec = (sec < 10 ? "0" : "") + sec;
    var year = date.getFullYear();
    var month = date.getMonth() + 1;
    month = (month < 10 ? "0" : "") + month;
    var day  = date.getDate();
    day = (day < 10 ? "0" : "") + day;

    return day + ":" + month + ":" + year + "  " + hour + ":" + min + ":" + sec;

}

