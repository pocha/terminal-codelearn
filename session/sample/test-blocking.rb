require "../lib/session.rb"
require 'stringio'

@output = ""
user = "pocha"

@bash = Session::Bash.new
	@read_data = StringIO::new(@output)
		@bash.outproc = lambda { |out| @output << out }
		@bash.errproc = lambda {|err| @output << err }
		@bash._initialize
@bash.execute "cd ~/rails/qa"
sleep 1
@bash.execute "rails server"
while 1
	if line = @read_data.read and !line.empty?
		puts line
	end
end
