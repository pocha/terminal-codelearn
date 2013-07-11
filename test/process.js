require('./initialize-server');
require('./initialize-client');

describe('Connection',function(){
		
	before(function(done){
		check(buttonDisabled,done);
	});

	/* Test not needed for now 
	it("should be owned by '"+process.env.USER+"'",function(done){

		var buff = '';		

		async.series([
			function(callback){
				$("input[name='command']").val('echo $$');
				$('#execute').click();
				check(checkMsgInOutput('echo \$\$\n\d+\n'),callback);	
			},
			function(callback){
				pid = parseInt($('#output').html().split('\n')[1]);
				exec("ps -ef | grep "+pid+" | head -1 | awk '{print $1}'", function(error,stdout,stderr){
					buff += stdout;
					callback();		
				});			
			},
			function(callback){
				var result = buff.split("\t");
				result[0].should.match(new RegExp(process.env.USER));
				callback();
			}
		],
		function(){
			$('#output').html('');
			done();	
		});

	}); */

	it("should display the connection closed message when I send 'exit'",function(done){
		async.series([
			function(callback){
				$("input[name='command']").val('exit');
				$('#execute').click();
				check(checkMsgInOutput('Connection to server closed'),callback);
			},
			function(callback){
				client1.readyState.should == 0
				$('#reset').click();
				check(buttonDisabled,callback);
			}
		],
		function(){
			$('#output').html('');
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
			},
			function(callback){
				$('#reset').click();
				check(buttonDisabled,callback);	
			},
		],
		function(){
			$('#output').html('');
			done();	
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
				check(buttonDisabled,callback);
			}
		],
		function(){
			$('#output').html('');
			done();	
		});

	});

});
