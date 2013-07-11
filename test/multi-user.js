var log_exec_output = function (error, stdout, stderr) {
	console.log("stdout: "  + stdout)
	console.log("stderr: "  + stderr)
	console.log("error: "  + error)
}

require('./initialize-server')

var crypto = require('crypto')

var user = "user_1"
var incorrect_user = "user_2"

before(function(done) {
	exec("useradd --home /home/" + user + " --create-home --shell /bin/bash " + user, log_exec_output )
	console.log("here")
	//exec("echo -e 'password\npassword' | passwd user_1", log_exec_output(error,stdout,stderr))
	done()
})

describe("Testing with right username & signature", function() {

	before(function(done) {
		GLOBAL.username = user
		var hmac = crypto.createHmac('sha1', Config.secret) //change secret to the key inside sockjs_app.js
		hmac.update(username)
		GLOBAL.signature = hmac.digest('hex')

		require ("./initialize-client")

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
				result[1].should == username;
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
				result[1].should == '/home/' + username;
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
				check(checkLength(2),callback);	
			},
			function(callback){
				client1.readyState.should == 0
			callback();
			}
			],
			function(){
				$('#output').html('');
				done();	
			});
	})
})

describe("Testing with correct user & incorrect signature", function() {
	before(function(done){
		GLOBAL.username = user
		GLOBAL.signature = "somearbitsignature"
		require ("./initialize-client")
		check(checkLength(1),done)
	})

	it("sockJS client should be inactive to start with", function(done) {
		client1.readyState.should == 0
		$('#output').html('');
		done()
	})
})


describe("Testing with incorrect user & correct signature", function() {
	before(function(done) {
		GLOBAL.username = incorrect_user
		var hmac = crypto.createHmac('sha1', Config.secret) //change secret to the key inside sockjs_app.js
		hmac.update(username)
		GLOBAL.signature = hmac.digest('hex')

		require ("./initialize-client")
		setTimeout(done,2000)
	})

	it("sockJS client should be inactive. User does not see any output",function(done) {
		client1.readyState.should == 0
		$("#output").html().should == ""
		done()
	})
})

after(function(done) {
	exec("userdel " + user, log_exec_output)
	done()
})
