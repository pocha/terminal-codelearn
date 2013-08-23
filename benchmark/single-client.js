/* These variables define the test parameters
*
*  totalMsgPerUser: total number of messages to send to the server
*  URL : url of the server to make the connection
*
*/

var totalMsgs =  500;
var URL = "ws://localhost:1134/_sockjs/websocket";

/*****************************************************************/
var fs = require('fs');

if(fs.existsSync('output-single.dat'))
	fs.unlinkSync('output-single.dat');


var Async = require('async');
var Websocket = require('ws');

var delay = new Array();

var Socket;
var msgsToBeSent  = totalMsgs;
var timeStart;
var timeEnd;

var end_of_output = /(\$|>)\s*$/;

	var conn = function(){
			
		Socket = new Websocket(URL);

		Socket.onopen = function() {	
			Socket.send(JSON.stringify({user: '', signature:''}));
			console.log("Client connected.")
		};

		Socket.onmessage = function(e) {			
			if(end_of_output.test(e.data)){		
				if(timeStart){
					timeEnd= process.hrtime(timeStart);
					delay[msgsToBeSent] = (timeEnd[0] * 1000) + (timeEnd[1]/1000000); 
				}		
				if(msgsToBeSent > 0){
					Socket.send('whoami\r');
					timeStart = process.hrtime();			
					msgsToBeSent--;			
				}else{
					Socket.close();	
				}											
			}
		};
		Socket.onclose = function() {
			console.log("Client disconnected.")
			clearInterval(updateStatus);				
			printDelays();
		};		

		return Socket;
	
	};

	new conn();

	var updateStatus = setInterval(function(){
			console.log("-----------------------------------------------");	
				console.log("Messages sent",totalMsgs - msgsToBeSent);
			console.log("-----------------------------------------------");		
	},5000);

	function printDelays(){
		
		var totalDelay = 0;
		
		buff =""

		for(var i=0;i<totalMsgs;i++)
			buff+= (i+1) + "	" + delay[i] + "\n";	
		
		fs.writeFileSync("output-single.dat",buff);


		console.log("-----------------------------------------------");

		for(var i=0;i<totalMsgs;i++){
			totalDelay += delay[i];
		}
	
		console.log('***********************************************');
		console.log('Total Average Delay:',totalDelay/totalMsgs);	
		console.log('***********************************************');
	}

