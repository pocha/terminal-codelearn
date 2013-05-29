class ProcessIncoming

  def incoming(message, callback)
    puts "Ext Message: #{message.inspect}"
    if message['channel'] =~ /\/input\/*/
		user = message['channel'].split("/")[2]
		ActiveUsers.add(user)
		ActiveUsers.get_user_count
		#process message probably in a thread
		FAYE_CLIENT.publish("/output/#{user}", "hi")
    end
    callback.call(message)
  end

  # def meta_channels?(channel)
    #if ("/app/activities" =~ /\/app\/.*/ || "/app/activities" =~ /\/app\/.*/
  # end

  #TODO disable publishing by users or use only user-published msgs
end
