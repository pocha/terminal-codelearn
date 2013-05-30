require ::File.expand_path("../session.rb",  __FILE__)
#require 'json'
require 'moped'
#require 'active_support/core_ext/hash/slice'

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
REEL_SERVER_PORT = 3001


$mongodb_session = Moped::Session.new([ DEFAULT_HOST+":"+MONGODB_PORT ])
$mongodb_session.use "terminal_commands"

#making function global as it is needed by both TerminalUser & MyServer

class ActiveUsers
	@@users = {}
	class << self 
		def create_user(user,terminal_no)
			puts "inside create_user"
			if not @@users[user]
				@@users[user] =  {:terminals => {terminal_no => TerminalUser.new(user,terminal_no)}, :time => Time.now } 
			elsif not @@users[user][:terminals][terminal_no]
				@@users[user][:terminals][terminal_no] = TerminalUser.new(user)
				@@users[user][:time] = Time.now
			end
			@@users[user][:terminals][terminal_no]
		end
		
		def destroy_user(user)
			if @@users[user]
				@@users[user][:terminals].each do |terminal_no, terminal_user|
					terminal_user.kill_all
				end
				@@users[user] = nil
			end
		end

		def destroy_all_users #To be called when quitting
			@@users.each do |user|
				destroy_user(user)
			end
			@@users = nil
		end
		
		def handle_request(hash)
			puts "inside handle_request #{hash.inspect}"
			user, terminal_no, command, type = hash.values_at("user", "terminal_no", "command", "type")
			#put begin rescue to check if the user exists. destroy & recreate the user if needed
			begin
				case type
				when "execute"
					@@users[user][:time] = Time.now
					@@users[user][:terminals][terminal_no].execute(command)
				when "kill"
					@@users[user][:terminals][terminal_no].kill_all_children(-2)
				when "reset"
					@@users[user][:terminals][terminal_no].kill_all
				when "create"
					create_user(user,terminal_no)
				when "destroy"
					destroy_user(user)
				end
			rescue Exception => e
				puts "Error - #{e}"
				puts "backtrace - #{e.backtrace}"
				FAYE_CLIENT.publish("/output/#{user}/#{terminal_no}", {:status => "error"})
			end
		end
	end
end

class TerminalUser
	

	def initialize(user, terminal_no)
		puts "initializing inside TerminalUser new"
		@bash = Session::Bash::new({:prog => "su #{user}"})
		@user = user
		@terminal_no = terminal_no
		#@output = ""
		#@read_data = StringIO::new(@output)
		#@check_data = StringIO::new(@output)
		@bash.outproc = lambda {|out| 
			#@output << out
			#@status = ( /(\$|>)\s*\z/.match(out) ) ? "complete" : "waiting"
			FAYE_CLIENT.publish("/output/#{@user}/#{@terminal_no}", {:status => "success", :data => sanitize_ansi_data(out)})
			#TODO - insert into mongodb	
		}
		#@status = "waiting"
		@bash._initialize
	end

	def execute(command)
		#@status = "waiting" #changing state so that execution hangs till output appear
		#puts @bash.inspect
		#puts "Executing command - #{command}"
		#TODO - insert command into mongodb
		@bash.execute(command)
		#sleep 1
		#block_for_output_to_come
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
		#Could not find a Ruby way to do this
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

