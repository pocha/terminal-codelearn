require "../lib/session.rb"
require 'stringio'

@output = ""
user = "pocha"

@bash = Session::Bash.new
	@read_data = StringIO::new(@output)
		@bash.outproc = lambda { |out| @output << out }
		@bash.errproc = lambda {|err| @output << err }
		@bash._initialize
		#commented out else it will fork & cant be killed with CTRL-C
		#@bash.execute "su -l #{user} -c 'bash -i'"
		#sleep 1	

t = Thread::new do
while 1
	if line = @read_data.read and !line.empty?
		puts line
	end
end
end
		
@bash.execute "cd ~/rails/qa"
sleep 1
@bash.execute "rails server"
sleep 15
puts ">>>sending pwd & nothing appears. This has become black hole"
@bash.execute "pwd"
@bash.execute "pwd"
@bash.execute "pwd"
@bash.execute "pwd"
@bash.execute "pwd"
@bash.execute "pwd"

t.join
