require 'thread'

TOTAL_USERS = 10
DOMAIN = "http://localhost:3001"
ITERATIONS_PER_USER = 1

user_count = 1
while user_count <= TOTAL_USERS do
	system("adduser --force-badname --disabled-password --ingroup root --gecos '' user_#{user_count};")
	
	f= File.open("user_#{user_count}.urls","w")
	f_r = File.open("urls.siege")
	data = f_r.read.gsub("__DOMAIN__",DOMAIN).gsub("__USER__","user_#{user_count}")
	f.write(data)
	f_r.close
	f.close
	
	user_count = user_count + 1
end

#restart the server
system("cd ../session/sample/; eye l test.eye; eye restart test; cd -; sleep 5")

lines = `cat urls.siege | grep -v "#" | wc -l`.chomp
iterations = lines.to_i * ITERATIONS_PER_USER
threads = []
data = {}
user_count = 1
while user_count <= TOTAL_USERS do
	puts "User #{user_count} ready to siege sir .."
	threads << Thread::new(iterations, user_count, data) do |iterations, user_count, data|
		cmd = "siege -c 1 -d 0.5 -r #{iterations} -f user_#{user_count}.urls -v"
		puts cmd
		system(cmd)
	end
	user_count = user_count + 1
end
threads.each do |t|
	t.join(30)
	puts "one join done - if it gets stuck long, hit Control C to end the simulation"
end
puts " ... done ... aye aye soldiers. Showing data :-"
puts data

#stop the server, else it will crib about user logged in
system("cd ../session/sample/; eye stop test; cd -;sleep 5")

#remove users
user_count = 1
while user_count <= TOTAL_USERS do
	system("userdel -rf user_#{user_count}")
	user_count = user_count + 1
end

puts "done. There might be some siege process running. Kill them via 'ps -ef | grep siege'"
