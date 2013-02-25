require 'reel'
require '../lib/session'
require 'stringio'
require 'json'
require 'cgi'

class Terminal_User
	
	def initialize(user)
		@bash = Session::Bash.new
		@output = ""
		@read_data = StringIO::new(@output)
		@check_data = StringIO::new(@output)
		@bash.outproc = lambda {|out| @output << out }
		@bash.errproc = lambda {|err| @output << err }
		@bash._initialize
		@bash.execute "su -l #{user} -c 'bash -i'"
		sleep 1	
=begin
		@bash.execute(" ")
		loop do
			@ps1 = @check_data.gets
			break if !@ps1.nil? #hold till output got some data
		end
		puts "PS1 - #{@ps1}"
		@ps1.gsub!("$","\\$")
=end
	end


	def execute(command)
		if !command.nil? and !command.empty? and !/^\s+$/.match(command) #The if ensures that empty command do not get executed as we are anyway sending two enters below
			@check_data.read #empty check data
			puts "Executing command"
			puts @bash.inspect
			@bash.execute(command)

			#the code below makes sure that the output is captured in a sequential manner
			loop do
				break if @check_data.read #hold till output got some data
			end
		end
		@bash.execute("\n") #empty command so that post complete we get the PS1 back 
		sleep 1 #cant do loop here as the main command might be blocking the output
	end

	def respond(request)
		#we sent two enters at the end of every command. If command is over you get two $PS1 or two $PS2. If the bash is still waiting, then there would be two empty lines. This will break if pressing enter creates similar lines in the output other than $PS1 or $PS2. But in that case as well, the client keeps polling & things will restore to normal when next command gets executed
		status = (/(.+?)\n\1\n$/.match(@output) or /.*?>\n.*?>\n/.match(@output)) ? "complete" : "waiting"
		data = @read_data.read
		if status == "waiting" 
			request.respond :ok, JSON.generate({:content => data, :status => status})
		else
			#remove extra line from the output because of extra enter - @bash.execute("\n")
			request.respond :ok, JSON.generate({:content => data.gsub(/.*?\n$/,""), :status => status}) 
		end
	end
	
end

class MyServer < Reel::Server
  def initialize(host = "127.0.0.1", port = 3001)
    super(host, port, &method(:on_connection))
  	@users = Hash.new
  end
  
  def on_connection(connection)
    while request = connection.request
      case request
      when Reel::Request
        handle_request(request)
      when Reel::WebSocket
        handle_websocket(request)
      end
    end
  end

  def handle_request(request)
	nothing,user,type,command = request.url.split("/")
	if @users["#{user}"].nil?
		@users["#{user}"] = Terminal_User.new(user)
	end
	user = @users["#{user}"]

	case type
	when "execute"
		command = CGI::unescape(command) if command
		puts "command #{command}"
		user.execute(command)
	end
	user.respond(request)
  end

  def handle_websocket(sock)
    sock << "Hello everyone out there in WebSocket land!"
    sock.close
  end
end

MyServer.run
