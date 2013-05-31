require 'faye'
require 'eventmachine'
require 'curb'
USER = "pocha"

EM.run {
  client = Faye::Client.new('http://localhost:3000/remote/faye')
  sending_time = nil
  receiving_time = nil

  subscription = client.subscribe('/output/**') do |message|
	  puts "#{Time.now - sending_time} output - #{message.inspect}"
	  if /(\$|>)\s*\z/.match(message['data']) 
	  	  puts "-----------------"
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
