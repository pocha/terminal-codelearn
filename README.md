This is a multi user bash Terminal web app done in Node.js using SockJS for communication.

The purpose of this app is not to emulate a true Terminal. It is designed to let website owners give a web based Pseudo Terminal to their users to be able to run linux commands.

> Note : security etc has not been taken care. Google for chroot & /etc/security/limits.conf to be able to jail users & limit their privileges.

#Installation 

Install the modules required to run the application.

	npm install

Start the server.

	npm start

This will start the server at port 1134.

Now go to **http://localhost:1134/client.html**,to see the application running.


#Testing

Install the modules required for testing.

	npm install -dev

Run the Mocha tests
  
	mocha -r should -R spec

You will see the tests passing with green check marks in front of them.
