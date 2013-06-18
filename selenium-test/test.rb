require 'selenium-webdriver'
require 'test/unit'

class TerminalTests < Test::Unit::TestCase
    def setup
        # create selenium objects
		@driver = Selenium::WebDriver.for :firefox
		@driver.navigate.to 'http://www.codelearn.org:1134/client.html'
		@wait = Selenium::WebDriver::Wait.new :timeout => 4

		@output = @driver.find_element(:id, 'output')
		@input = @driver.find_element(:name, 'command')

		#buttons
		@execute = @driver.find_element(:id, 'execute')
		@reset = @driver.find_element(:id, 'reset')
		@kill = @driver.find_element(:id, 'kill')
		@wait.until {@execute.enabled?}		
	end

	def clear_output
		@driver.execute_script("document.getElementById('output').innerHTML = ''")
	end

	def test_pwd

		@input.send_keys("pwd")
		@execute.click

		@wait.until {@execute.enabled?}		
		dir = `pwd`	

		assert(@output.text[/#{dir}/],"Output does not contain '#{dir}' when command 'pwd' is executed")
	end

	def test_kill_process

		command = "sleep 100"

		@input.send_keys("#{command}")
		@execute.click
		
		@wait.until {@execute.enabled?}		
		result = `ps -ef | grep 'sleep 100' | awk '{print $8"\s"$9}'`
		result = result.split("\n")[0]

		assert(result[/#{command}/],"Sleep 100 is not running")

		@kill.click
		
		@wait.until {@execute.enabled?}		
		result = `ps -ef | grep 'sleep 100' | awk '{print $8"\s"$9}'`
		result = result.split("\n")[0]

		assert(!result[/#{command}/],"Sleep 100 was not killed")
	end

	def test_reset

		@input.send_keys("echo $$")
		@execute.click
		
		@wait.until {@execute.enabled?}
		result = @output.text
		pid1 = result.split("\n")[1]

		@reset.click

		@wait.until {@execute.enabled?}		
		#clear_output

		@input.send_keys("echo $$")
		@execute.click

		@wait.until {@execute.enabled?}
		result = @output.text
		pid2 = result.split("\n")[1]

		assert(pid2 != pid1,"Process after reset is same")
	end

	def teardown
		@driver.quit  
	end

end
