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
	
	def loop_for_lines(lines) #for normal command, 2 lines are one input & 1 output. For @bash.execute("\n"), two lines suffice as well
		count = 0
		loop do
			puts "inside loop"
			if !@check_data.gets.nil?
				count = count + 1
			end
			break if count == lines
		end
	end

	def execute(command)
		puts @bash.inspect
		if !command.nil? and !command.empty? and !/^\s+$/.match(command) #The if ensures that empty command do not get executed as we are anyway sending two enters below
			puts "Executing command"
			#@check_data.read #empty check data
			@bash.execute(command)

			#wait for the input command to appear
			#loop_for_lines(1) - guess its best to wait for a second or so else the output gets garbled
			sleep 1
		end
		#@check_data.read #empty check data
		@bash.execute("\n") #empty command so that post complete we get the PS1 back 
		#loop_for_lines(2)
		sleep 1 #sleep another second. Passing empty input wont show on the output. So cant loop/wait for lines
	end

	def respond(request)
		#we sent two enters at the end of every command. If command is over you get two $PS1 or two $PS2. If the bash is still waiting, then there would be two empty lines. This will break if pressing enter creates similar lines in the output other than $PS1 or $PS2. But in that case as well, the client keeps polling & things will restore to normal when next command gets execute
		puts "output - #{@output}"
		status = (/(.+?)\n\1\n\z/.match(@output) or /.*?>.*?\n.*?>.*?\n\z/.match(@output)) ? "complete" : "waiting"
		data = @read_data.read
		puts "data - #{data}"
		if status == "waiting" 
			request.respond :ok, JSON.generate({:content => data, :status => status})
		else
			#remove extra line from the output because of extra enter - @bash.execute("\n")
			request.respond :ok, JSON.generate({:content => data.gsub(/.*?\n\z/,""), :status => status}) 
		end
	end

	def get_children_process(pid)
		`ps --ppid #{pid} | grep -v PID | awk '{print $1}'`.split("\n")
	end

	def kill_all_children
		if @parent_pid.nil?
			@parent_pid = get_children_process(get_children_process(@bash.pid)[0])[0] 
		end	
		get_children_process(@parent_pid).each do |p|
			system("kill -2 #{p}")
		end
		sleep 1 #let the PS1 appear which was sent prior to the command execution
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
	
	puts "url - #{request.url}"
	
	if type == "execute"
		command = CGI::unescape(command) if command
		puts "command #{command}"
		user.execute(command)
	end

	if type == "kill"
		user.kill_all_children
	end

	if type == "reset"
		user.kill_all_children
		@users["#{user}"] = nil
		request.respond :ok, JSON.generate({:content => "", :status => "completed"}) 
		return
	end

	user.respond(request)
  end

  def handle_websocket(sock)
    sock << "Hello everyone out there in WebSocket land!"
    sock.close
  end
end

MyServer.run
