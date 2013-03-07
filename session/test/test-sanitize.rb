ANSI_COLOR_CODE = {
	0 => 'black',
	1 => 'red',
	2 => 'green',
	3 => 'yellow',
	4 => 'blue',
	5 => 'purple',
	6 => 'cyan',
	7 => 'white'
}


def sanitize_ansi_data(data) 
		data.gsub!(/\033\[[\d\;]{2,}m.*?\033\[0m/){ |data|
			span = "<span style='"
			content = ""
			/\033\[([\d\;]{2,})m(.*?)\033\[0m/.match(data) {|m|
				content = m[2]
				m[1].split(";").each do |code|
					#puts code
					if match = /(\d)(\d)/.match(code) 
						puts "#{match[2]} #{ANSI_COLOR_CODE[match[2].to_i]}"
						case match[1]
						when "3"
							span += "color: #{ANSI_COLOR_CODE[match[2].to_i]}; "
						when "4"
							span += "background-color: #{ANSI_COLOR_CODE[match[2].to_i]}; "
						else
							#do nothing
						end
					else
						span += "font-weight:bold; "
					end
				end
			}
			span += "'/>"
			"#{span}#{content}</span>"
		}
		data.gsub!(/\033\[m/,"")
		data.gsub!(/\033\[0m/,"")
	end
	
	data = "ls\r\n\e[0m\e[30;42mat-spi2\e[0m             \e[01;34mpulse-PKdhtXMmr18n\e[0m  test.rb               \e[01;34mvmware-pocha\e[0m\r\neye.log             \e[01;34mpulse-PNbbIFaT02i5\e[0m  tmpFHAF0y             \e[01;34mvmware-root\e[0m\r\n\e[01;34mkeyring-tpfZ5S\e[0m      \e[01;34mssh-oSjkDqzK2269\e[0m    tmp.txt\r\n\e[01;34mpulse-fhKjxmUqywSU\e[0m  \e[01;34mtest-app\e[0m            unity_support_test.1\r\n\e[32mpocha@ubuntu\e[0m \e[35m/tmp\e[0m$ "
	puts sanitize_ansi_data(data)
