require 'faye'
require 'eventmachine'
require 'curb'
USER = "pocha"

EM.run {
  client = Faye::Client.new('http://localhost:9292/faye')

  client.subscribe('/pocha/0/output') do |message|
    puts message.inspect
  end
  client.subscribe('/pocha/0/execute') do |message|
    puts message.inspect
  end
	
  http = Curl.get("http://localhost:3001/pocha/0")
  puts "you should see linux user below"
  client.publish('/pocha/0/execute',"whoami")
  client.publish('/pocha/0/execute',"whoami")
  client.publish('/pocha/0/execute',"whoami")
  client.publish('/pocha/0/execute',"whoami")
  client.publish('/pocha/0/execute',"whoami")
}
