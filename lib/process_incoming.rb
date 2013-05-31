class ProcessIncoming

  @@latency = {}

  def incoming(message, callback)
    if message['channel'] == "/input"
    	#puts "#{Time.now} Incoming - #{message['channel']}: #{message.inspect}"
			@@latency[message['data']["user"]] = Time.now
			ActiveUsers.handle_request(message['data'])
   	elsif message['channel'] =~ /\/output/ and message['data'][:data] =~ /(\$|>)\s*$/
			user = message['channel'].split("/")[2]
			diff = Time.now - @@latency[user]	
			
			File.open("../execute.txt" , 'a+') do |file|			
				file.puts "Processing Time: #{diff}\n"
			end		

		end
    callback.call(message)
  end

  def outgoing(message,callback)
		if message['channel'] =~ /\/output/ and !message['data'].nil? and message['data'][:data] =~ /(\$|>)\s*$/
			user = message['channel'].split("/")[2]
			diff = Time.now  - @@latency[user]

			File.open("../execute.txt" , 'a+') do |file|			    	
				file.puts "Server Time: #{diff}"
			end	

		end
	callback.call(message)
  end 

  # def meta_channels?(channel)
    #if ("/app/activities" =~ /\/app\/.*/ || "/app/activities" =~ /\/app\/.*/
  # end

  #TODO disable publishing by users or use only user-published msgs
end
