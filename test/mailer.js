require('./common');

describe('Mailer',function(){
	it("should send a mail if I process a new error",function(done){
		var myerr = new Error("Hi, I am a new Error");		
		async.series([
			function(callback){
				if(fs.existsSync('email.log'))
					fs.unlinkSync('email.log');	
				mailer.processError(myerr,callback);	
			},
			function(callback){
				log = fs.readFileSync('email.log').toString();
				subject  = /Subject:.*\r/.exec(log)[0];
				subject.should.match(new RegExp(myerr.message));
				callback();	
			}
		],
		function(){
			done();	
		});

	});
});
