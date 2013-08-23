var state = false;

var filter = new Filter();

var history = new Array()
var ptr = 0

var Client = function(){

	var socket = new SockJS(SOCKET_URL)

	socket.onopen = function(){
		initializeOutput()
		socket.send(JSON.stringify({user: USERNAME, signature: SIGNATURE})); //Send a username here to make the pty spawn as that user
		state = true;
	}

	socket.onmessage = function(e){
		//console.log(e.data)
		data = e.data.replace("<","&lt;").replace(">","&gt;")
		//console.log(data)
		appendOutput(filter.toHtml(data));
	}

	socket.onclose = socket.onerror = function(){
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
	
	socket.sendData = function(character) {
		console.log(character)
		socket.send($("input[name='command']").val() + character);
	}

	socket.reset = function() {
		socket.close();
		initializeOutput();
		client1 = new Client();
		ptr = history.length
	};

	return socket;	
}

client1 = new Client();
		

//Buttons

	$("#kill").click(function(){
		client1.kill();
	});

	$("#reset").click(function(){
		client1.reset();
	});
	
	

function sendData(character) {
	client1.sendData(character);
	$("input[name='command']").val("")
}


timerFired = function (){
	client1.close();
}

function appendOutput(data){
	//$('#output').append(data);
	$("#output input").before(data)
	$('#output').scrollTop($('#output').prop('scrollHeight'));
}

function initializeOutput() {
	$("input[name='command']").val("")
	appendOutput("\nConnecting...");
	$("#output").html('<input type="text" name="command" class="input" autocomplete="off">')

	$("input[name='command']").keydown(function(event) {
		switch(event.which) {
			case 13: //enter key
				event.preventDefault();
				history[ptr] = $("input[name='command']").val()
				if (++ptr > history.length) ptr = history.length
				//console.log(history + " " + ptr)
				sendData('\r')
			break;
			case 38: //up key
				event.preventDefault();
				if (--ptr < -1) ptr = -1
				$("input[name='command']").val(history[ptr])
				//console.log(history + " " + ptr)
				//sendData('\x1b[A')

			break;
			case 40: //down key
				event.preventDefault();
				if (++ptr > history.length) ptr = history.length
				$("input[name='command']").val(history[ptr])
				//console.log(history + " " + ptr)
				//sendData('\x1b[B')
			break;
			case 9: //tab key
				event.preventDefault();
				sendData('\t')
			break;
			case 67: //checking control C
				if (event.ctrlKey) {
					event.preventDefault();
					client1.kill()
				}
			break;
		}
	})
}
