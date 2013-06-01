##Installation

1. Install the Tornado web server www.tornadoweb.org
2. Git clone from https://github.com/mrjoes/sockjs-tornado
3. Install SockJS-Tornado 
		
		cd sockjs-tornado
		python setup.py build
		[sudo] python setup.py install


##Test Setup

On server start the Tornado sever 

		python run.py

> You can change the port by changing the number of  *app.listen(9999)* to whichever port you want

On client side edit client.html to point to the server address when initializing SockJS object

Run client.html in the browser

The time taken is shown in the console
