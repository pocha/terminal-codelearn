require 'faye'
require 'eventmachine'
require 'curb'
USER = "pocha"

EM.run {
  client = Faye::Client.new('http://localhost:3000/remote/faye')

  client.subscribe('/input/*') do |message|
    puts "input - #{message.inspect}"
  end
  client.subscribe('/output/*') do |message|
    puts "output - #{message.inspect}"
  end
	
  #sending inputs
  puts "sending input"
  client.publish('/input/pocha',"hey")
  client.publish('/input/tocha',"hi")
}
