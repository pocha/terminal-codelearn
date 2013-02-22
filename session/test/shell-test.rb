require '../lib/session'
bash = Session::Bash.new
bash.outproc = lambda{|out| puts "#{ out }"}
bash.errproc = lambda{|err| raise err}
bash.execute('while test 1; do echo 42; sleep 1; done') 
