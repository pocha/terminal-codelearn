require('./initialize-server')
require('./initialize-client')

describe("Autocomplete tests", function() {

	before(function(done) {
		$('#reset').click();
		checkPromptPostReset(done)
	})

	it("should show previous command on up key",function(done) {
		async.series([
			function(callback) {
				$("input[name='command']").val('whoami');
				keyPress(ENTER_KEY);
				check(checkLength(2),callback);	
			},
			function(callback){
				keyPress(UP_KEY)
				$("input[name='command']").val().should.match(/whoami/);
				callback();
			}
		],
		function(){
			initializeEnvironment();
			done();	
		});

	})

	it("should show next command on down key",function(done) {
		async.series([
			function(callback){
				$("input[name='command']").val('pwd');
				keyPress(ENTER_KEY);
				check(checkLength(2),callback);	
			},
			function(callback){
				//$("input[name='command']").trigger("downKey");
				keyPress(UP_KEY)
				keyPress(UP_KEY)
				keyPress(DOWN_KEY)
				$("input[name='command']").val().should.match(/pwd/);
				callback();
			}
		],
		function(){
			initializeEnvironment();
			done();	
		});
	})

	it("autocomplete if there is only one option",function(done) {
		async.series([
			function(callback){
				$("input[name='command']").val('mkdir test-app');
				keyPress(ENTER_KEY);
				check(checkLength(2),callback);	
			},
			function(callback){
				var val = $("#output").text()
				$("input[name='command']").val('cd test-');
				//$("input[name='command']").trigger("tabKey");
				keyPress(TAB_KEY)
				check(function(){ return $("#output").text() == val },callback);	
			},
			function(callback){
				$("#output").text().should.match(/cd test-app/)
				$("input[name='command']").val().should.equal("");
				callback();
			}
			],
			function(){
				initializeEnvironment()
				done();	
			});
	});

	it("show options in output if more than one options",function(done) {
		async.series([
			function(callback){
				var val = $("#output").text()
				$("input[name='command']").val('cd t');
				keyPress(TAB_KEY)
				check(function(){ return $("#output").text() == val },callback);	
			},
			function(callback){
				$("#output").text().should.match(/cd t\u0007est/);
				$("input[name='command']").val().should.equal("");
				var val = $("#output").text()
				keyPress(TAB_KEY)
				keyPress(TAB_KEY)
				check(checkLength(3),callback);	
			},
			function(callback) {
				$("#output").text().should.match(/test\/\s+test\-app\//)
				callback()
			}
		],
		function(){
			initializeEnvironment()
			done();	
		});
	});

	after(function(done){
		$("input[name='command']").val('rm -rf test-app');
		keyPress(ENTER_KEY);
		check(checkLength(2),done);	
	})

})
