require 'ruby-prof'

module CodeProfiler

	# profile logger. As of now, it will log the records inside performance/hande-request-graph.txt file
	def self.profile_logger(file_name, start_time,request_url)
		RubyProf.start
		yield
		results = RubyProf.stop
			    
		# Print a Graph profile to text
		profile_type = "graph"
		File.open "../performance/#{file_name}-#{profile_type}.txt", 'a+' do |file|
			RubyProf::GraphPrinter.new(results).print(file)
		end
		
		record_time_and_url(file_name,"graph",start_time, Time.now, request_url)


		profile_type = "flat"

		File.open "../performance/#{file_name}-#{profile_type}.txt", 'a+' do |file|
			RubyProf::FlatPrinter.new(results).print(file)
		end

		record_time_and_url(file_name,"flat",start_time, Time.now, request_url)

		# File.open "../performance/#{file_name}-stack.html", 'w' do |file|
			# 	RubyProf::CallStackPrinter.new(results).print(file)
		# end
	end	

	# Method for recording the time and url
	def self.record_time_and_url(file_name,profile_type,start_time,end_time, request_url)
		File.open "../performance/#{file_name}-#{profile_type}.txt", 'a+' do |file|
			file.puts "***** request_url: #{request_url}----------start_time: #{start_time}------------end_time: #{end_time}------------total_time_required: #{end_time - start_time}"
		end
	end
	

end