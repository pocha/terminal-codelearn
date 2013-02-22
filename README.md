**Installation**

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
	rails server

Now open *http://localhost:3000/* on your browser & start fiddling.

P.S. - The kill butto is not implemented yet.
