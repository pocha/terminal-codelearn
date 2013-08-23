require('./initialize-server');
require('./initialize-client');

describe('Client terminal',function(){
	before(function(done){
		check(function() { return !$("#output").text() },done);
		initializeEnvironment();
	});

	it("should return '"+process.env.USER+"' when I send 'whoami'",function(done){
		$("input[name='command']").val('whoami');
		keyPress(ENTER_KEY);
		async.series([
			function(callback){
				check(checkLength(2),callback);			
			},
			function(callback){				
				result = $('#output').text().split("\n")[1];
				result.should.match(new RegExp(process.env.USER));				
				callback();		
			}
		],
		function(){
			initializeEnvironment();
			done()
		});
	});
	
	it("should stop the command execution when I press kill",function(done){

		async.series([
			function(callback){
				$("input[name='command']").val('sleep 100');
				keyPress(ENTER_KEY);
				check(function() { return !$("#output").text().match(/sleep 100/) }, function() { setTimeout(callback,2000) } );	
			},
			function(callback){			
				exec("ps -ef | grep 'sleep 100' | grep -v grep",function(error,stdout,stderr){
					stdout.should.match(/sleep\s*100/);
					$('#kill').click();
					check(checkLength(2),callback);	
				});			
			},
			function(callback){				
				exec("ps -ef | grep 'sleep 100' | grep -v grep",function(error,stdout,stderr){
					stdout.should.not.match(/sleep\s*100/);
					callback()
				});			
			}
		],
		function(){
			initializeEnvironment();
			done()
		});
		
	});

	it("should close background processes and return a different pid when I press reset",function(done){

		var pid1, pid2,buff = '';

		async.series([
			function(callback){
				$("input[name='command']").val('echo $$');
				keyPress(ENTER_KEY);
				check(checkLength(3),callback);	
			},
			function(callback){
				pid1 = $('#output').text().split('\n')[1];
				console.log("pid found - " + pid1)
				$("input[name='command']").val('sleep 100 & sleep 1000 & sleep 10000 & whoami');
				keyPress(ENTER_KEY);
				setTimeout(callback,2000);
			},
			function(callback){
				exec("ps -ef | grep "+pid1,function(error,stdout,stderr){
					//stdout.split("\n").should.have.lengthOf(3);
					$('#reset').click();
					checkPromptPostReset(callback)
				});			
			},
			function(callback){
				$("input[name='command']").val('echo $$');
				keyPress(ENTER_KEY);
				check(checkLength(2),callback);
			},
			function(callback){
				pid2 = parseInt($('#output').text().split('\n')[1]);
				pid1.should.not.equal(pid2);				
				callback();
			}
		],
		function(){
			initializeEnvironment();
			done()
		});

	});

});

