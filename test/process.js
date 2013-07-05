require('./common');

describe('Connection',function(){
		
	before(function(done){
		check(done);
	});

	it("should be owned by '"+process.env.USER+"'",function(done){

		var buff = '';		

		async.series([
			function(callback){
				$("input[name='command']").val('echo $$');
				$('#execute').click();
				check(callback);	
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

	});

	it("should display the connection closed message as well as close background processes when I send 'exit'",function(done){
		async.series([
			function(callback){
				$("input[name='command']").val('sleep 100 & sleep 1000 & sleep 10000 & whoami');
				$('#execute').click();
				check(callback);
			},
			function(callback){
				$("input[name='command']").val('exit');
				$('#execute').click();
				setTimeout(callback,50);
			},
			function(callback){
				$('#output').html().should.match(/connection to server closed/i);
				$('#reset').click();
				check(callback);
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
				setTimeout(callback,50);	
			},
			function(callback){
				$('#output').html().should.match(/could not connect/i);
				Terminal.listen(callback);
			},
			function(callback){
				$('#reset').click();
				check(callback);	
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
				timerFired()
				setTimeout(callback,50);	
			},
			function(callback){
				$('#output').html().should.match(/connection to server closed/i);
				$('#reset').click();
				check(callback);
			}
		],
		function(){
			$('#output').html('');
			done();	
		});

	});

});
