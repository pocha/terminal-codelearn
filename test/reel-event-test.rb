require 'faye'
require 'eventmachine'
require 'curb'
require 'net/sftp'

SERVER = "localhost"
USER = ""
PASSWORD = ""
TIMES = 20

puts "Removing execute.txt if it exists on server..."

Net::SFTP.start(SERVER,USER,:password => PASSWORD) do |sftp|
	begin
		sftp.remove!('execute.txt')
		puts "execute.txt removed"
	rescue
		puts "execute.txt not found"
	end
end

puts "Connection ended"

times = []
i = 1

EM.run {
  
	client = Faye::Client.new("http://#{SERVER}:3000/remote/faye")
  sending_time = nil
  receiving_time = nil

  subscription = client.subscribe('/output/**') do |message|
		if /(\$|>)\s*\z/.match(message['data']) 
			diff = Time.now - sending_time
			times << diff	  
			puts "#{diff} output - #{message.inspect}"
			i=i.next
			if(i > TIMES)
				EM.stop
			end
	  	puts "#{i}.-----------------"
		  message = {:user => USER, :terminal_no => 0, :type => "execute", :command => "whoami"}
		  sending_time = Time.now
		  puts "sending input #{message.inspect}"
		  client.publish('/input', message)
	  end 
  end

  subscription.callback do
	  sending_time = Time.now
	  puts "sending input"
	  message = {:user => USER, :terminal_no => 0}
	  client.publish('/input', message.merge({:type => "create"}))
	  #client.publish('/input', message.merge({:type => "destroy"}))
  end
  #sending inputs
}


puts "Retrieving execute.txt from server..."

Net::SFTP.start(SERVER,USER,:password => PASSWORD) do |sftp|
	sftp.download!('execute.txt', 'initial.txt')
	puts "execute.txt downloaded"
end

puts "Connection ended"

proc_times = []
server_times = []

File.open("initial.txt", 'r').each do |line|
	if line[/^processing time/i]
		proc_times << Float(line[/\d*\.\d*$/])
	elsif line[/^server time/i]
		server_times << Float(line[/\d*\.\d*$/])
	end
end

File.open("final.txt", 'w') do |file|
	avg_client = times.inject{ |sum, el| sum + el } /	times.size
	avg_proc = proc_times.inject{ |sum, el| sum + el } / proc_times.size
	avg_server = server_times.inject{ |sum, el| sum + el } / server_times.size
	file.puts "Average Client Time:     #{'%.4f' % avg_client} s\n"
	file.puts "Average Processing Time: #{'%.4f' % avg_proc} s\n"
	file.puts "Average Server Time:     #{'%.4f' % avg_server} s"
end

