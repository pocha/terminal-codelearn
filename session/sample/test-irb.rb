require "../lib/session.rb"
require 'stringio'

@output = ""
user = "pocha"

@bash = Session::Bash.new
	@read_data = StringIO::new(@output)
		@bash.outproc = lambda { |out| @output << out }
		@bash.errproc = lambda {|err| @output << err }
		@bash._initialize
@bash.execute "bash -i"
sleep 2
@bash.execute "irb"
sleep 2
@bash.execute "2+2"
sleep 2
@bash.execute "puts 'hello'"
sleep 2
@bash.execute "exit"
sleep 2
puts @read_data.read
