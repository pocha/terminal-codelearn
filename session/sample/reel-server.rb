require 'reel'
require '../lib/session'
require 'stringio'
require 'json'
require 'cgi'

ANSI_COLOR_CODE = {
	0 => 'black',
	1 => 'red',
	2 => 'green',
	3 => 'yellow',
	4 => 'blue',
	5 => 'purple',
	6 => 'cyan',
	7 => 'white'
}



class TerminalUser
	
	def initialize(user)
		@bash = Session::Bash::new({:prog => "su -l #{user} -c 'bash -i'"})
		@output = ""
		@read_data = StringIO::new(@output)
		@check_data = StringIO::new(@output)
		@bash.outproc = lambda {|out| 
			@output << out
			@status = ( /(\$|>)\s*\z/.match(out) ) ? "complete" : "waiting"
		}
		#commenting out as no STDERR in ruby pty
		#@bash.errproc = lambda {|err| @output << err }
		@bash._initialize
		@status = "waiting"
		sleep 1	
	end
	

	def execute(command)
		puts @bash.inspect
		puts "Executing command"
		@bash.execute(command)

		sleep 1
	end

	def respond(request)
		puts "output - #{@output}"
		data = @read_data.read
		puts "data - #{data}"
		@output.slice!(0,@read_data.pos)
		@read_data.rewind
		request.respond :ok, {"Content-type" => "text/html; charset=utf-8"},  JSON.generate({:content => sanitize_ansi_data(data), :status => @status})
	end

	def sanitize_ansi_data(data) 
		data.gsub!(/\033\[1m/,"<b>")
		data.gsub!(/\033\[0m/,"</b></span>")
		
		data.gsub!(/\033\[[\d\;]{2,}m.*?<\/b><\/span>/){ |data|
			span = "<span style='"
			content = ""
			/\033\[([\d\;]{2,})m(.*?)<\/b><\/span>/.match(data) {|m|
				content = m[2]
				m[1].split(";").each do |code|
					#puts code
					if match = /(\d)(\d)/.match(code) 
						case match[1]
						when "3"
							span += "color: #{ANSI_COLOR_CODE[match[2].to_i]}; "
						when "4"
							span += "background-color: #{ANSI_COLOR_CODE[match[2].to_i]}; "
						else
							#do nothing
						end
					else
						span += "font-weight:bold; "
					end
				end
			}
			span += "'>"
			"#{span}#{content}</b></span>"
		}
		data
	end


	def get_children_process(pid)
		`ps --ppid #{pid} | grep -v PID | awk '{print $1}'`.split("\n")
	end

def kill_all_children(interrupt)
		if @parent_pid.nil?
			@parent_pid = get_children_process(get_children_process(@bash.pid)[0])[0] 
		end	
		get_children_process(@parent_pid).each do |p|
			system("kill #{interrupt} #{p}")
			sleep 1
		end
		sleep 1 #let the PS1 appear which was sent prior to the command execution
	end

	def kill_all
		kill_all_children(-9)
		@bash.close if @bash.ready?
		sleep 1
=begin
		intermediate_parent = get_children_process(@bash.pid)[0]
		system("kill -9 #{@parent_pid}")
		sleep 1
		system("kill -9 #{intermediate_parent}")
		sleep 1
		system("kill -9 #{@bash.pid}")
		sleep 1
=end
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
	begin	
		puts "url - #{request.url}"
		nothing,user,type,command = request.url.split("/")
		
		if @users["#{user}"].nil?
			puts "user not found. Creating"
			@users["#{user}"] = TerminalUser.new(user)
		end
		terminal_user = @users["#{user}"]
		puts terminal_user.inspect
		
		
		if type == "execute"
			command = CGI::unescape(command) if command
			puts "command #{command}"
			Thread::new(terminal_user, command) do |terminal_user, command|
				terminal_user.execute(command)
			end
		end

		if type == "kill"
			terminal_user.kill_all_children(-2)
		end

		if type == "reset"
			handle_error(terminal_user, user,request)			
		end

		terminal_user.respond(request)
	rescue
		#there might be a problem with broken pipe as the user might have typed 'exit'. Just send error & expect for a new request
		handle_error(terminal_user, user,request)
	end
  end

  def handle_error(terminal_user, user,request)
		terminal_user.kill_all
		
		@users["#{user}"] = nil
		#hopefully garbage collector kicks in here & picks up the object that is made nil
		@users["#{user}"] = TerminalUser.new(user)
		
		request.respond :ok, JSON.generate({:status => "error", :content => ""})
		return
  end

  def handle_websocket(sock)
    sock << "Hello everyone out there in WebSocket land!"
    sock.close
  end
end

MyServer.run
