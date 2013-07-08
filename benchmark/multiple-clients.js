/* These variables define the test parameters
*
*  totalUsersArray: total number of connections to the server
*                   for which the script is run each time 
*  totalMsgPerUser: total number of messages to send through each connection
*  URL : url of the server to make the connection
*
*/

var totalUsersArray  = [10,40,70]//,100,130,160,190,220,250];
var totalMsgPerUser =  5;
var URL = "ws://localhost:1134/_sockjs/websocket";

/*******************************/
var fs = require('fs');

if(fs.existsSync('output-multiple.dat'))
	fs.unlinkSync('output-multiple.dat');

var Async = require('async');
var Websocket = require('ws');


asyncLoop(totalUsersArray.length, function(mainLoop) {

	var totalUsers = totalUsersArray[mainLoop.iteration()];

	var delay = new Array();
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
			Sockets[num].send('');
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
			if(closed == totalUsers){
				clearInterval(updateStatus);	
				printDelays();
			}
		};		

		return Sockets[num];
	
	};


	for(var i=0;i<totalUsers;i++){
		new conn(i)
	}


	function queue(func,num){
		callInParallel[num] = func;
		parallelIndex++;
		if(parallelIndex == (totalUsers - closed)){
			p();	
		}	
	}
	
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

	
		if(connected == totalUsers){
			console.log("-----------------------------------------------");	
			for(var i=0;i<totalUsers;i++){
				console.log("User",i+1,"has sent",totalMsgPerUser - msgsToBeSent[i],"messages");
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
	
			buff =""
			buff+= totalUsersArray[mainLoop.iteration()] + "	" + totalDelay/totalUsers + "\n";	

			fs.appendFileSync("output-multiple.dat",buff);

			mainLoop.next();
	}
},function(){});

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
