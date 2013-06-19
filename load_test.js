/* These variables define the test parameters
*  totalUsers: total number of connections to the server
*  totalMsgPerUser: total number messages to send through each connection
*/

var totalUsers  = 10;
var totalMsgPerUser =500;

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

var conn = function(num,callback){
			
	Sockets[num] = new Websocket("ws://localhost:1134/echo/websocket");

	Sockets[num].onopen = function() {	
		console.log("Client ",num+1,"/",totalUsers," connected.")
		connected++;			
		callback();			
	};

	Sockets[num].onmessage = function(e) {				
		var end_of_output = /(\$|>)\s*$/;

		if(end_of_output.test(e.data) && connected == totalUsers){		
		
			if(Times[num] > 0){
	
				if(timeStart[num]){
					timeEnd[num] = process.hrtime(timeStart[num]);
					delay[num] += (timeEnd[num][0] * 1000) + (timeEnd[num][1]/1000000); 
				}

				Sockets[num].send('whoami\r');

				timeStart[num] = process.hrtime();			
				Times[num]--;			
			
			} else
				Sockets[num].close();	

		};
	};

	Sockets[num].onclose = function() {
		closed++;
		console.log("Client ",num+1,"/",totalUsers," disconnected.")		
	};		

	return Sockets[num];
	
};


asyncLoop(totalUsers, function(loop) {
		new conn(loop.iteration(),loop.next)
	},
	function(){
		for(var i=0;i<totalUsers;i++)
			Sockets[i].send('whoami\r');
	}
);


var updateStatus = setInterval(function(){

	if(closed == totalUsers){
		clearInterval(updateStatus);				
		printDelays();
		return;
	}
	
	if(connected == totalUsers){
		console.log("-----------------------------------------------");	
		for(var i=0;i<totalUsers;i++){
			console.log("User",i+1,"has sent",totalMsgPerUser - Times[i],"messages");
		}
		console.log("-----------------------------------------------");	
	}	

	if(closed > 0)
		console.log("Clients disconnected",closed);

},5000);

function printDelays(){
		totalDelay = 0;
		console.log("-----------------------------------------------");

		for(var i=0;i<totalUsers;i++){
			console.log("Avg Delay for User",i+1,":",delay[i]/totalMsgPerUser);
			totalDelay += delay[i]/totalMsgPerUser;
		}
		
		console.log('***********************************************');
		console.log('Total Average Delay:',totalDelay/totalUsers);	
		console.log('***********************************************');
}

function asyncLoop(iterations, func, callback) {
    var index = 0;
    var done = false;
    var loop = {
        next: function() {
            if (done) {
                return;
            }

            if (index < iterations) {
                index++;
                func(loop);

            } else {
                done = true;
                callback();
            }
        },

        iteration: function() {
            return index - 1;
        },

        break: function() {
            done = true;
            callback();
        }
    };
    loop.next();
    return loop;
}
