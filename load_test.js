/* These variables define the test parameters
*  totalUsers: total number of connections to the server
*  totalMsgPerUser: total number messages to send through each connection
*/

var totalUsersArray  = [10,40,70,100,130,160,190,220,250];
var totalMsgPerUser =  5;
var URL = "ws://www.codelearn.org:1134/echo/websocket";

/*******************************/

var Async = require('async');
var Websocket = require('ws');
var fs = require('fs');
var mainTime = new Array();
	var delay = new Array();

asyncLoop(totalUsersArray.length, function(mainLoop) {

	var totalUsers = totalUsersArray[mainLoop.iteration()];

	var Sockets  = new Array();
	var msgsToBeSent  = new Array();
	var timeStart = new Array();
	var timeEnd = new Array();

	var callInParallel = new Array();	

	for(var i=0;i<totalUsers;i++){

		msgsToBeSent[i] = totalMsgPerUser;
		delay[i] = 0;
	}
	
	var parallelIndex = 0;	
	var closed = 0;
	var connected = 0;
	var closedSent = 0;
	var done = true;
	var end_of_output = /(\$|>)\s*$/;

	var conn = function(num){
			
		Sockets[num] = new Websocket(URL);

		Sockets[num].onopen = function() {	
			console.log("Client ",num+1,"/",totalUsers," connected.")
			connected++;
		};

		Sockets[num].onmessage = function(e) {			
			if(end_of_output.test(e.data)){		
				if(timeStart[num]){
					timeEnd[num] = process.hrtime(timeStart[num]);
					delay[num] += (timeEnd[num][0] * 1000) + (timeEnd[num][1]/1000000); 
				}		
				if(msgsToBeSent[num] > 0){
					var f  = function(callback){
						Sockets[num].send('whoami\r');
						timeStart[num] = process.hrtime();			
						msgsToBeSent[num]--;			
						callback();					
					};
					queue(f,num);
				}else{
						closedSent++;
						Sockets[num].close();	
				}											
			}
		};
		Sockets[num].onclose = function() {
			closed++; 			
			console.log("Client ",num+1,"/",totalUsers," disconnected.")
			if(closed == totalUsers)
				printDelays();
		};		

		return Sockets[num];
	
	};


	for(var i=0;i<totalUsers;i++){
		new conn(i)
	}


	function queue(func,num){
		//console.log("Users logged",parallelIndex)	
/*		if(!done){
			console.log("still executing previous queue, deffering request");
			setTimeout(function(){queue(func,num)},100);
		} else {
			if(!callInParallel[num]){
*/				//console.log("Logging request for client",num+1); 				
		callInParallel[num] = func;
		parallelIndex++;
		if(parallelIndex == (totalUsers - closed)){
					//if(done){		
						//console.log("EXECUTING QUEUE");				
			p();	
					//}
		}	
/*			}else {
				console.log("user", num+1 ,"already logged, deffering request");					
				setTimeout(function(){queue(func,num)},100);
			}
		}				
*/	}
	
	function p(){
		if(done){		
			done = false;		
			Async.parallel(callInParallel,function(){
					done = true;			
			});
			callInParallel = new Array();
			parallelIndex = 0;
		} else
			setTimeout(p,10);
	}


	var updateStatus = setInterval(function(){

		/*if(closed == totalUsers){
			clearInterval(updateStatus);				
			printDelays();
			return;
		}*/
	
		if(connected == totalUsers){
			console.log("-----------------------------------------------");	
			for(var i=0;i<totalUsers;i++){
				console.log("User",i+1,"has sent",totalMsgPerUser - msgsToBeSent[i],"messages");
			}
			console.log("-----------------------------------------------");	
		}	

		if(closed > 0)
			console.log("Clients disconnected",closed,"but close sent to",closedSent);

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
	
			mainTime[mainLoop.iteration()] = totalDelay/totalUsers;
	
			mainLoop.next();
	}
},
	function(){
		buff =""
		for(var i=0;i<totalUsersArray.length;i++)
			buff+= totalUsersArray[i] + "	" + mainTime[i] + "\n";	
		fs.writeFileSync("output.dat",buff);
	}
);
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
