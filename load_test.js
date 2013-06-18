/* These variables define the test parameters
*  totalUsers: total number of connections to the server
*  totalMsgPerUser: total number messages to send through each connection
*/

var totalUsers  = 20;
var totalMsgPerUser = 30;

/*******************************/

var Websocket = require('ws');

var Sockets  = new Array();
var Times  = new Array();
var timeStart = new Array();
var timeEnd = new Array();
var delay = new Array();

for(var i=0;i<totalUsers;i++){

	Times[i] = totalMsgPerUser;
	delay[i] = 0;
}

var closed = 0;
var connected = 0;

var conn = function(num){
			
	Sockets[num] = new Websocket("ws://www.codelearn.org:1134/echo/websocket");

	Sockets[num].onopen = function() {	
		//socket.send('whoami\r');
		console.log("Client ",num+1,"/",totalUsers," connected.")		
		timeStart[num] = process.hrtime();			
	};

	Sockets[num].onmessage = function(e) {				
		var end_of_output = /(\$|>)\s*$/;

		if(end_of_output.test(e.data)){		
			if(Times[num] > 0){
				//console.log("Client ",num+1," is sending message ",totalMsgPerUser - Times[num] + 1)		
				timeEnd[num] = process.hrtime(timeStart[num]);
				delay[num] += (timeEnd[num][0] * 1000) + (timeEnd[num][1]/1000000); 
				//console.log("Client ",num+1,"delay till now is ",delay[num],"ms for ",totalMsgPerUser - Times[num] + 1," messages")						
				Sockets[num].send('whoami\r');
				timeStart[num] = process.hrtime();			
				Times[num]--;			
			}
			else
				Sockets[num].close();	
		};

	};

	Sockets[num].onclose = function() {
		closed++;
		console.log("Client ",num+1,"/",totalUsers," disconnected.")		
	};		

	return Sockets[num];
	
};

for(var i=0;i<totalUsers;i++){
	new conn(i);
}

var updateStatus = setInterval(function(){	
	console.log("-----------------------------------------------");	
	for(var i=0;i<totalUsers;i++){
		console.log("User",i+1,"has sent",totalMsgPerUser - Times[i],"messages");
	}
	console.log("-----------------------------------------------");	
	if(closed > 0)
		console.log("Clients disconnected",closed);
	
},5000);

var id = setInterval(function(){
	if(closed == totalUsers){
		clearInterval(id);
		clearInterval(updateStatus);		
		totalDelay = 0;		
		for(var i=0;i<totalUsers;i++){
			console.log("Avg Delay for User",i+1,"is",delay[i]/totalMsgPerUser);
			totalDelay += delay[i]/totalMsgPerUser;
		}
		
		console.log('***********************************************');
		console.log('Total Average Delay:',totalDelay/totalUsers);	
		
	}
},1000);
