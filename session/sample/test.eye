Eye.load("./eye/*.rb") # load submodules

Eye.config do
  logger "#{File.expand_path(File.join(File.dirname(__FILE__)))}/eye.log" # eye logger
  logger_level Logger::DEBUG
end

Eye.application "test" do
  working_dir File.expand_path(File.join(File.dirname(__FILE__)))
  stdall "trash.log" # logs for processes by default
  env "APP_ENV" => "production" # global env for each processes
  triggers :flapping, :times => 10, :within => 1.minute
  stop_on_delete true # process will stopped before delete

    chain :grace => 5.seconds, :action => :restart # restarting with 5s interval, one by one.
  	
    process("reel-server") do
      pid_file "1.pid" # will be expanded with working_dir
      start_command "ruby ./reel-server.rb"
      daemonize true
      stdall "reel-server.log"

      checks :cpu, :below => 30, :every => 1.minute
      checks :memory, :below => 50.megabytes, :every => 1.minute
    end
  
end
