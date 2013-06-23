function conn(host, fn_onopen, fn_onmessage, fn_onclose){

	socket = new SockJS('http://'+host+'/echo');

	socket.onopen = function() {	
		console.log('open');
		fn_onopen();
		//$('#output').html('');				
		//$('#execute').button({disabled: false});
	};

	socket.onmessage = function(e) {				
		//console.log(e.data);
		fn_onmessage();
		/*$('#output').append(colorReplace(e.data));
		pre.scrollTop(pre.prop('scrollHeight'));

		var end_of_output = /(\$|>)\s*$/;

		if(end_of_output.test(e.data)){		
			$('#execute').button({disabled: false});	
		};*/


	};

	socket.onclose = function() {
		console.log('close');
		fn_onclose();
		//$('#execute').button({disabled: true});
	};		

	this.on_execute = function(command) {
		socket.send(command+'\r');
	}
	this.on_kill = function() {
		socket.send(String.fromCharCode(3));
	}
	this.on_reset = function() {
		socket.close()
		socket = new SockJS('http://'+host+'/echo');
	}

	return socket;

};

