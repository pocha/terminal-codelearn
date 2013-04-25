require 'ruby-prof'

module CodeProfiler

	def self.profile_logger(file_name)
		RubyProf.start
		yield
		results = RubyProf.stop
			    
		# Print a Graph profile to text
		File.open "../performance/#{file_name}-graph.txt", 'a+' do |file|
			RubyProf::GraphPrinter.new(results).print(file)
		end
			 
		# File.open "../performance/#{file_name}-flat.txt", 'w' do |file|
			# RubyProf::FlatPrinter.new(results).print(file)
			# RubyProf::FlatPrinterWithLineNumbers.new(results).print(file)
		# end

		# File.open "../performance/#{file_name}-stack.html", 'w' do |file|
			# 	RubyProf::CallStackPrinter.new(results).print(file)
		# end
	end	
	
end