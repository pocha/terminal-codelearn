This is a multi user bash Terminal web app done in Ruby using Reel server. This will only run on a linux platform. 

The server code is inside session. demo-app is the example front-end done in Rails that provides the Terminal webpage & also acts as proxy for the commands between the web page & the Terminal server. Typically you need the setup to keep all the user level information at the backend. 

The purpose of this app is not to emulate a true Terminal. It is designed to let website owners give a web based Pseudo Terminal to their users to be able to run linux commands.

> Note : security etc has not been taken care. Google for chroot & /etc/security/limits.conf to be able to jail users & limit their privileges. 

#Installation 

Backup your .bashrc & copy the .bashrc from here to your linux user home directory

	cp ~/.bashrc ~/.bashrc-bkup
	cp .bashrc ~/.bashrc

.bashrc is used to calculate $PS1 . Colored $PS1 might create problems. 

	cd demo-app
	bundle install
	cd ..
	sudo su 	#the app needs to run as super user to be able to switch the user as 'su'
	gem install reel
	gem install json
	cd session/sample
	ruby reel-server.rb

This will run the Terminal server at port 3001.

Inside *app/controller/terminals_controller.rb*, change "pocha" to the linux username of your PC

	def current_user
		"pocha"
	end

Open another terminal & run rails server
	
	cd demo-app
	thin start

> Used thin instead of Webrick as Webrick did not support parallel requests

Now open *http://localhost:3000/* on your browser & start fiddling.
