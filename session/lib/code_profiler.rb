require 'ruby-prof'

module CodeProfiler

	# profile logger. As of now, it will log the records inside performance/hande-request-graph.txt file
	def self.profile_logger(file_name, start_time)
		RubyProf.start
		yield
		results = RubyProf.stop
			    
		# Print a Graph profile to text
		File.open "../performance/#{file_name}-graph.txt", 'a+' do |file|
			RubyProf::GraphPrinter.new(results).print(file)
		end
		
		record_time(file_name,start_time, Time.now)
		# File.open "../performance/#{file_name}-flat.txt", 'w' do |file|
			# RubyProf::FlatPrinter.new(results).print(file)
			# RubyProf::FlatPrinterWithLineNumbers.new(results).print(file)
		# end

		# File.open "../performance/#{file_name}-stack.html", 'w' do |file|
			# 	RubyProf::CallStackPrinter.new(results).print(file)
		# end
	end	

	# Method for recording the time
	def self.record_time(file_name,start_time, end_time)
		File.open "../performance/#{file_name}-graph.txt", 'a+' do |file|
			file.puts "***** start_time: #{start_time}------------end_time: #{end_time}------------total_time_required: #{end_time - start_time}"
		end
	end
	
end