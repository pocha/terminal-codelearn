var log_exec_output = function (error, stdout, stderr) {
	console.log("stdout: "  + stdout)
	console.log("stderr: "  + stderr)
	console.log("error: "  + error)
}

require('./initialize-server')
require('./initialize-client')

var crypto = require('crypto')

var user = "user_1"
var incorrect_user = "user_2"



describe("Testing with right username & signature", function() {

	before(function(done) {
		exec("useradd --home /home/" + user + " --create-home --shell /bin/bash " + user, function(){
			done();
		});
		//console.log("here");
		//exec("echo -e 'password\npassword' | passwd user_1", log_exec_output(error,stdout,stderr))
	});

	before(function(done) {
		USERNAME = user
		var hmac = crypto.createHmac('sha1', Config.secret) //change secret to the key inside sockjs_app.js
		hmac.update(USERNAME)
		SIGNATURE = hmac.digest('hex')

		$('#reset').click();

		check(buttonDisabled,done)
	})

	it("should login as user",function(done) {
		async.series([
			function(callback){
				$("input[name='command']").val('whoami');
				$('#execute').click();
				check(buttonDisabled,callback);	
			},
			function(callback){
				var result = $("#output").html().toString().split("\n");
				result[1].should.match(new RegExp(USERNAME));
				callback();
			}
			],
			function(){
				$('#output').html('');
				done();	
			});

	})

	it("should have right home directory",function(done) {
		async.series([
			function(callback){
				$("input[name='command']").val('pwd');
				$('#execute').click();
				check(buttonDisabled,callback);	
			},
			function(callback){
				var result = $("#output").html().toString().split("\n");
				result[1].should.match(new RegExp('/home/' + USERNAME));
				callback();
			}
			],
			function(){
				$('#output').html('');
				done();	
			});
	})

	it("typing exit should close websocket/sockJS connection",function(done) {
		async.series([
			function(callback){
				$("input[name='command']").val('exit');
				$('#execute').click();
				check(checkLength(4),callback);	
			},
			function(callback){
				client1.readyState.should.equal(3);
				callback();
			}
			],
			function(){
				$('#output').html('');
				done();	
			});
	});

	after(function() {
		//delete user
		exec("userdel -r " + user, log_exec_output);
	})
})

describe("Testing with correct user & incorrect signature", function() {
	before(function(done){
		USERNAME = user;
		SIGNATURE = "somearbitsignature";
		$('#reset').click();
		check(checkMsgInOutput('Connection to server closed'),done);
	})

	it("sockJS client should be inactive to start with", function(done) {
		client1.readyState.should.equal(3);
		$('#output').html('');
		done()
	})
})


describe("Testing with incorrect user & correct signature", function() {
	before(function(done) {
		USERNAME = incorrect_user
		var hmac = crypto.createHmac('sha1', Config.secret) //change secret to the key inside sockjs_app.js
		hmac.update(USERNAME)
		SIGNATURE = hmac.digest('hex')

		$('#reset').click();
		check(checkLength(3),done);
	})

	it("sockJS client should be inactive. User should see 'Unknown Id' message",function(done) {
		$('#output').html().should.match(new RegExp('Unknown id: '+incorrect_user)); 
		client1.readyState.should.equal(3);
		done()
	})

	after(function() {
		//reset state
		USERNAME = '';
		SIGNATURE = '';
		$('#reset').click();
	});

});


