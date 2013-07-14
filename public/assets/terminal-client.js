$('#execute').attr("disabled",true);

var state = false;

var filter = new Filter();

var Client = function(){

	var socket = new SockJS(SOCKET_URL)

	socket.onopen = function(){
		socket.send(JSON.stringify({user: USERNAME, signature: SIGNATURE})); //Send a username here to make the pty spawn as that user
		$('#output').html('');
		state = true;
	}

	socket.onmessage = function(e){
		appendOutput(filter.toHtml(e.data));
		
		//var end_of_output = /(\$|>)\s*$/;

		//if(end_of_output.test(e.data)){		
			$('#execute').attr("disabled",false);	
		//};
	}

	socket.onclose = socket.onerror = function(){
		$('#execute').attr("disabled",true);
		if(state)		
			appendOutput("\nConnection to server closed.... Please click 'Reset' to reconnect");
		else
			appendOutput("\nCould not connect.... Please click 'Reset' to retry");
		state = false;	
	}


	socket.execute = function(command) {
		socket.send(command+'\r');
	}

	socket.kill = function() {
		socket.send(String.fromCharCode(3));
	}	

	socket.reset = function() {
		socket.close();
		$('#output').html('');
		appendOutput("\nConnecting...");
		client1 = new Client();
	};

	return socket;	
}

client1 = new Client();

$("#myForm").submit(function(){
	if($('#execute').is(':disabled') == false){
		$('#execute').click();
	};
});		

//Buttons
	$("#execute").click(function(){
		command = $("input[name='command']").val();			
		client1.execute(command);
		$("input[name='command']").val('');
		$('#execute').attr("disabled",true);
	});

	$("#kill").click(function(){
		client1.kill();
		$('#execute').attr("disabled",true);
	});

	$("#reset").click(function(){
		client1.reset();
		$('#execute').attr("disabled",true);
	});

timerFired = function (){
	client1.close();
}

function appendOutput(data){
	$('#output').append(data);
	$('#output').scrollTop($('#output').prop('scrollHeight'));
}