$('#execute').attr("disabled",true);

var Client = function(){
	socket = new SockJS(SOCKETURL)

	socket.onopen = function(){
		$('#output').html('');
	}

	socket.onmessage = function(e){
		$('#output').append(colorReplace(e.data));
		$('#output').scrollTop($('#output').prop('scrollHeight'));

		var end_of_output = /(\$|>)\s*$/;

		if(end_of_output.test(e.data)){		
			$('#execute').attr("disabled",false);	
		};
	}

	socket.onclose = function(){
		$('#execute').attr("disabled",true);
	}


	socket.execute = function(command) {
		socket.send(command+'\r');
	}

	socket.kill = function() {
		socket.send(String.fromCharCode(3));
	}	

	socket.reset = function() {
		socket.close()
		socket = new Client();
	};

	return socket;	
}

socket = new Client();

$("#myForm").submit(function(){
	if($('#execute').is(':disabled') == false){
		$('#execute').click();
	};
});		

//Buttons
	$("#execute").click(function(){
		command = $("input[name='command']").val();			
		socket.execute(command);
		$("input[name='command']").val('');
		$('#execute').attr("disabled",true);
	});

	$("#kill").click(function(){
		socket.kill();
		$('#execute').attr("disabled",true);
	});

	$("#reset").click(function(){
		socket.reset();
		$('#execute').attr("disabled",true);
	});


function colorReplace(input) {

	var span = "<span style='";
		var color = "color: ";
		var bold = " font-weight: bold;";
		var underline = " text-decoration: underline;";
		var span_fin = "'>";			
		var span_end = "</span>";
	var reset = /\033\[0+m/;

	input = input.replace(reset,"<span>");

		var replaceColors = {
			"31" : "red;",
			"01;31" : "red;" + bold,

			"32" : "green;",
			"01;32" : "green;" + bold,

			"33" : "yellow;",
			"01;33" : "yellow;" + bold,

			"34" : "blue;",
			"01;34" : "blue;" + bold,

			"35" : "purple;",
			"01;35" : "purple;" + bold,

			"36" : "cyan;",
			"01;36" : "cyan;" +bold,
			"04;36" : "cyan;" +underline,

			"37" : "white;",
			"01;37" : "white;" +bold,

			"01;30" : bold,
			"01" : bold

		};


		for( k in replaceColors )
		{
			var re = new RegExp( "\\033\\[" + k + "m", "g" );
			var re_str  = span_end  + span + color + replaceColors[ k ] + span_fin;
			input = input.replace( re,re_str);
		};

		input = input.replace(/\033\[0+m/g,span_end);			

		return input;
}



