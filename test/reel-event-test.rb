require 'faye'
require 'eventmachine'
require 'curb'
USER = "pocha"

EM.run {
  client = Faye::Client.new('http://localhost:3000/remote/faye')

  subscription = client.subscribe('/output/**') do |message|
	  puts "#{Time.now} output - #{message.inspect}"
	  if /(\$|>)\s*\z/.match(message['data']) 
	  puts "-----------------"
		  message = {:user => "pocha", :terminal_no => 0, :type => "execute", :command => "whoami"}
		  puts "#{Time.now} sending input #{message.inspect}"
		  client.publish('/input', message)
	  end 
  end

  subscription.callback do
	  puts "#{Time.now} sending input"
	  message = {:user => "pocha", :terminal_no => 0}
	  client.publish('/input', message.merge({:type => "create"}))
	  #client.publish('/input', message.merge({:type => "destroy"}))
  end

  #sending inputs
}
