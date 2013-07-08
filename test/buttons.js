require('./common');

describe('Client terminal',function(){
	before(function(done){
		check(buttonDisabled,done);
	});

	it("should return '"+process.env.USER+"' when I send 'whoami'",function(done){
		$("input[name='command']").val('whoami');
		$('#execute').click();
		async.series([
			function(callback){
				check(buttonDisabled,callback);			
			},
			function(callback){				
				result = $('#output').html().split("\n")[1];
				result.should.match(new RegExp(process.env.USER));				
				callback();		
			}
		],
		function(){
			$('#output').html('');
			done();	
		});
	});
	
	it("should stop the command execution when I press kill",function(done){
		var buff = '';

		async.series([
			function(callback){
				$("input[name='command']").val('sleep 100');
				$('#execute').click();
				setTimeout(callback,50);	
			},
			function(callback){
				exec("ps -ef | grep 'sleep 100' | awk '{print $8"+'"\t"'+"$9}'",function(error,stdout,stderr){
					buff += stdout;
					callback();		
				});			
			},
			function(callback){				
				var result = buff.split("\n")[0];
				buff = "";				
				result.should.match(/sleep\s*100/);				
				$('#kill').click();
				exec("ps -ef | grep 'sleep 100' | awk '{print $8"+'"\t"'+"$9}'",function(error,stdout,stderr){
					buff += stdout;
					callback();		
				});			
			},
			function(callback){				
				var result = buff.split("\n")[0];
				buff = "";				
				result.should.not.match(/sleep\s*100/);
				callback();
			}
		],
		function(){
			$('#output').html('');
			done();	
		});
		
	});

	it("should close background processes and return a different pid when I press reset",function(done){

		var pid1, pid2,buff = '';

		async.series([
			function(callback){
				$("input[name='command']").val('echo $$');
				$('#execute').click();
				check(buttonDisabled,callback);	
			},
			function(callback){
				pid1 = parseInt($('#output').html().split('\n')[1]);
				$("input[name='command']").val('sleep 100 & sleep 1000 & sleep 10000 & whoami');
				$('#execute').click();
				check(buttonDisabled,callback);
			},
			function(callback){
				$('#reset').click();
				check(buttonDisabled,callback)			
			},
			function(callback){
				exec("ps -ef | grep "+pid1,function(error,stdout,stderr){
					buff += stdout;
					callback();		
				});			
			},
			function(callback){
				buff.split("\n").should.have.lengthOf(3);
				$('#output').html('');
				$("input[name='command']").val('echo $$');
				$('#execute').click();
				check(buttonDisabled,callback);
			},
			function(callback){
				pid2 = parseInt($('#output').html().split('\n')[1]);
	
				pid1.should.not.equal(pid2);				
				callback();
			}
		],
		function(){
			$('#output').html('');
			done();	
		});
		
	});

});

