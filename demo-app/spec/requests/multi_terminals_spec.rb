#We are just testing the backend calls. No JS testing is done. 

require 'spec_helper'

describe "User created: " do
	
	before :all do
		#Start server
		#system("su -l root -c 'cd ../session/sample; eye l test.eye; eye start test; cd -; sleep 2'")
	end
	after :all do
		#stop server
		#system("cd ../session/sample; eye stop test; cd -")
	end

	it "command execution in terminals should work" do
		post '/terminals/0/execute', :command => 'a="terminal-1"'
		post '/terminals/1/execute', :command => 'a="terminal-2"'
		
		post '/terminals/0/execute', :command => 'echo $a'
		puts response.body
		response.body.match(/terminal-1/).should_not be_nil

		post '/terminals/1/execute', :command => 'echo $a'
		puts response.body
		response.body.match(/terminal-2/).should_not be_nil
	end
	
end
