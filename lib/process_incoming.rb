class ProcessIncoming

  def incoming(message, callback)
    puts "#{Time.now} Incoming - #{message['channel']}: #{message.inspect}"
    if message['channel'] == "/input"
		ActiveUsers.handle_request(message['data'])
    end
    callback.call(message)
  end

  def outgoing(message,callback)
	if message['channel'] == "/input" or message['channel'] =~ /\/output/
    	puts "#{Time.now} Outgoing - #{message['channel']}: #{message.inspect} "
	end
	callback.call(message)
  end 

  # def meta_channels?(channel)
    #if ("/app/activities" =~ /\/app\/.*/ || "/app/activities" =~ /\/app\/.*/
  # end

  #TODO disable publishing by users or use only user-published msgs
end
