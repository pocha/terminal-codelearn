require('./initialize-server');
require('./initialize-client');

describe('Connection',function(){
		
	before(function(done){
		check(function() { return !$("#output").text() },done);
	});


	it("should display the connection closed message when I send 'exit'",function(done){
		async.series([
			function(callback){
				$("input[name='command']").val('exit');
				keyPress(ENTER_KEY);
				check(checkMsgInOutput('Connection to server closed'),callback);
			},
			function(callback){
				client1.readyState.should == 0
				$('#reset').click();
				checkPromptPostReset(callback)
			}
		],
		function(){
			initializeEnvironment();
			done();	
		});		
 
	});

	it("should display the could not connect message when I connect to already stopped server",function(done){
		async.series([
			function(callback){
				Terminal.close();
				$('#reset').click();
				check(checkMsgInOutput('Could not connect'),callback);
			},
			function(callback){
				client1.readyState.should == 0
				Terminal.listen(callback);
			}
		],
		function(){
			$('#reset').click();
			checkPromptPostReset(done)
		});

	});
	
	it("should display connection closed message when I manually call the function passed to idle timer",function(done){
		async.series([
			function(callback){
				timerFired();
				check(checkMsgInOutput('Connection to server closed'),callback);
			},
			function(callback){
				client1.readyState.should == 0
				$('#reset').click();
				checkPromptPostReset(callback)
			}
		],
		function(){
			initializeEnvironment();
			done();	
		});

	});

});
