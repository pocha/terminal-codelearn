This is a multi user bash Terminal web app done in Node.js using SockJS for communication.

The purpose of this app is not to emulate a true Terminal. It is designed to let website owners give a web based Pseudo Terminal to their users to be able to run linux commands.

![Application Screenshot](http://www.codelearn.org/blog/wp-content/uploads/2013/06/terminal_screenshot.png)

> Note : security etc has not been taken care. Google for chroot & /etc/security/limits.conf to be able to jail users & limit their privileges.

#Installation 

Install the modules required to run the application.

	npm install --production

Start the server.

	npm start

This will start the server at port 1134.

Now go to **http://localhost:1134/client.html** to see the application running.


#Testing

Install the modules required for testing.

	npm install --dev

Run the Mocha tests
  
	mocha -r should -R spec

You will see the tests passing with green check marks in front of them.

#Benchmarking Results

The scripts in Benchmark directory were used to generate the graphs.The server and the client specifications are given below

###1.Server#
   + CPU : Intel Core i7 920 @ 2.67GHz
   + CPU cores : 4
   + Memory : 11.72 GB

###2.Client#
   + CPU : Intel Xeon E5645 @ 2.40GHz
   + CPU cores : 1
   + Memory : 595 MB
 
Server to Client ping delay = 98.9 ms


##Single Client#
The graph below shows the client side delay measured when a single client is connected to the server.

![Single Client Graph](https://raw.github.com/pocha/terminal-codelearn/master/graphs/single-client.png)
   
The blue lines give the minimum and the maximum of the dataset while the green one shows the average of the whole data

##Multiple Clients#
The graph below shows the average time taken by the server to respond to a request once it has recieved a message from the client when particular number of clients are connected and sending messages concurrently.

![Multiple Clients Graph](https://raw.github.com/pocha/terminal-codelearn/master/graphs/multiple-clients.png)

This graph measures the time taken upto 506 clients only. More than 506 clients are not able to connect because 'pty' modules gives error that 'forkpty(3) failed'.

