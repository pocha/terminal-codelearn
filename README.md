#Installation 

Backup your .bashrc & copy the .bashrc from here to your linux user home directory

	cp ~/.bashrc ~/.bashrc-bkup
	cp .bashrc ~/.bashrc

.bashrc is used to calculate $PS1 . Colored $PS1 might create problems. 

	cd demo-app
	bundle install
	cd ..
	cd session/sample
	sudo su
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

P.S. - The kill button is not implemented yet.
