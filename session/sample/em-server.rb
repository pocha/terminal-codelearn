require 'faye'
require 'eventmachine'
require '../lib/session'
require 'stringio'
require 'json'
require 'cgi'
require 'moped'
require 'active_support/core_ext/hash/slice'
require 'rubygems' # or use Bundler.setup

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

#### Configurable data points#####

DEFAULT_HOST = '127.0.0.1'
MONGODB_PORT = '27017'
SERVER_PORT = 1201


$mongodb_session = Moped::Session.new([ DEFAULT_HOST+":"+MONGODB_PORT ])
$mongodb_session.use "terminal_commands"

#making function global as it is needed by both TerminalUser & MyServer
def get_children_process(pid)
	`ps --ppid #{pid} | grep -v PID | awk '{print $1}'`.split("\n")
	#Could not find a Ruby way to do this
end

class TerminalUser
	
	def initialize(user)
		@bash = Session::Bash::new({:prog => "su #{user}"})
		@output = ""
		@read_data = StringIO::new(@output)
		@check_data = StringIO::new(@output)
		@bash.outproc = lambda {|out| 
			@output << out
			@status = ( /(\$|>)\s*\z/.match(out) ) ? "complete" : "waiting"
		}
		@status = "waiting"
		@bash._initialize
	end
	
	def block_for_output_to_come
		count = 0
		@check_data.read
		loop_count = 0
		until @status == "complete" or loop_count > 4
			sleep 1
			loop_count = loop_count + 1
			#puts "inside loop"
		end
	end

	def execute(command)
		@status = "waiting" #changing state so that execution hangs till output appear
		#puts @bash.inspect
		#puts "Executing command - #{command}"
		@bash.execute(command)
		#sleep 1
		#block_for_output_to_come
	end

	def respond(request)
		block_for_output_to_come
		#puts "output - #{@output}"
		data = @read_data.read
		#puts "data - #{data}"
		@output.slice!(0,@read_data.pos)
		@read_data.rewind
		[sanitize_ansi_data(data), @status]
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

	def kill_all_children(interrupt)
		if @parent_pid.nil?
			@parent_pid = get_children_process(@bash.pid)[0] 
		end	
		get_children_process(@parent_pid).each do |p|
			system("kill #{interrupt} #{p}")
			sleep 1
		end
		sleep 1 #let the PS1 appear which was sent prior to the command execution
	end

	def kill_all
		kill_all_children(-9)
		begin
			@bash.close
		rescue Exception => e
			puts e
		end
	end
	
end


class TerminalServer < EM::Connection
  def receive_data(data)
    send_data(data)
  end
end

EventMachine.run do
  # hit Control + C to stop
  Signal.trap("INT")  { EventMachine.stop }
  Signal.trap("TERM") { EventMachine.stop }

  EventMachine.start_server("0.0.0.0", SERVER_PORT, TerminalServer)
end
