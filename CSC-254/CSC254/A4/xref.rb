require_relative "dumpreader.rb"

#read symbol table from dump
symbols = readdump(`dwarfdump #{ARGV[0]}`)



def convert(tableread, index)
	tableread[0].each do |filename|
		#Open Files
		text = File.open(filename).read
		text = text.gsub("<", "&lt").gsub(">", "&gt")
		textlines = text.split(/\r*\n/)
		#Add line numbers and HTML formats
		counter = 1
		text = "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/></head>\n<body><pre>"
		textlines.each do |line|
			line = "<a name=\""+counter.to_s+"\">"+line+"</a>\n"
			text += line
			counter = counter + 1
		end
		#Add HTML ending formats
		text = text + "</pre></body></html>"
		#For each symbol in the symbol table from dump, modify each line of text
		tableread[1].each do |var|
			#Case for global variables and functions
			if var.last == -2 or var.last == 1
				#Split each line
				appearances = text.split(/(?<=\n)/)
				text = ""
				#Counter for comment tags (/* and */)
				commenttag = 0
				appearances.each do |temp|
					#Add numbers of comment tags to commenttag
					commenttag = commenttag + temp.scan(/(?=\/\*)/).count
					#Change color of comments
					temp = temp.gsub(/\/\*(?!<searched>)/, "<font color=\"gray\">/*<searched>").gsub(/\*\/(?!<\/font>)/, "*/</font>")
					#If this variable/function is not modified already and not in comment
					if not temp.include? (">"+var[0]) and commenttag == 0
						#Change color of these keywords
						temp = temp.gsub(/(?<=([^\w]))if (?!<\/font>)/, "<font color=\"green\">if</font> ").gsub(/(?<=([^\w]))while(?!(<\/font>|\w))/, "<font color=\"green\">while</font>").gsub(/(?<=([^\w]))for(?!(<\/font>|\w))/, "<font color=\"green\">for</font>").gsub(/(?<=([^\w]))return(?!(<\/font>|\w))/, "<font color=\"green\">return</font>").gsub(/(?<=([^\w]))goto(?!(<\/font>|\w))/, "<font color=\"green\">goto</font>").gsub(/(?<=([^\w]))switch(?!(<\/font>|\w))/, "<font color=\"green\">switch</font>").gsub(/(?<=([^\w]))case(?!(<\/font>|\w))/, "<font color=\"green\">case</font>").gsub(/(?<=([^\w]))else(?!(<\/font>|\w))/, "<font color=\"green\">else</font>").gsub(/(?<=([^\w]))typedef(?!(<\/font>|\w))/, "<font color=\"brown\">typedef</font>").gsub(/(?<=([^\w]))struct(?!(<\/font>|\w))/, "<font color=\"brown\">struct</font>").gsub(/(?<=([^\w]))int(?!(<\/font>|\w))/, "<font color=\"red\">int</font>").gsub(/(?<=([^\w]))char(?!(<\/font>|\w))/, "<font color=\"red\">char</font>").gsub(/(?<=([^\w]))double(?!(<\/font>|\w))/, "<font color=\"red\">double</font>").gsub(/(?<=([^\w]))float(?!(<\/font>|\w))/, "<font color=\"red\">float</font>").gsub(/(?<=([^\w]))long(?!(<\/font>|\w))/, "<font color=\"red\">long</font>").gsub(/(?<=([^\w]))unsigned(?!(<\/font>|\w))/, "<font color=\"red\">unsigned</font>").gsub(/(?<=([^\w]))short(?!(<\/font>|\w))/, "<font color=\"red\">short</font>").gsub(/(?<=([^\w]))void(?!(<\/font>|\w))/, "<font color=\"red\">void</font>").gsub(/(?<=([^\w]))enum(?!(<\/font>|\w))/, "<font color=\"red\">enum</font>")
						#If this line is not the declaration line or this is not the file that contains declaration
						if not temp.include? "<a name=\""+var[2].to_s+"\">" or filename != var[1]
							#Add link if accurately matched (For example, 'apple' and 'pineapple' shouldn't match)
							temp = temp.gsub((Regexp.new '(?<=[^\w(//.*)])'+var[0]+'(?=[^\w<])'),"<a href=\""+var[1].split("/").last.gsub(/\./,"_")+".html"+"#"+var[2].to_s+"\">"+var[0]+"</a>")
							text = text + temp
						#If the function name is main and this is the declaration line, add it to index.html
						elsif temp.include? "<a name=\""+var[2].to_s+"\">" and filename == var[1] and var[0]=='main'
							index.write('<a href='+filename.split("/").last.gsub(/\./,"_")+'.html#'+var[2].to_s+">main</a>\n")
							temp = temp.gsub(var[0],"<a name=\"searched\">"+var[0]+"</a>")
							text = text + temp
						#Else, mark as searched and move on
						else
							temp = temp.gsub(var[0],"<a name=\"searched\">"+var[0]+"</a>")
							text = text + temp
						end
					else
						text = text + temp
					end
					#Comment - number of '*/'. Should return 0 if it is not in comment right now
					commenttag = commenttag - temp.scan(/(?=\*\/)/).count
				end
			#Case for parameters
			elsif var.last == -1
				#It should only be in the scope of the file that it's declared. 
				if var[1] == filename.split(/\//).last
					#processed, balance, and counter are variables to determine the scope
					processed = false
					balance = 0
					counter = 0
					#split the content at the line that the parameter is declared
					splitter = text.split(Regexp.new "(?=<a name=\""+var[2].to_s+"\">)")
					#Add the unused part of text to the output
					text = splitter.first
					temptext = splitter.last
					skipping = false
					#Count the number of brackets. If it has gone through at least a bracket (processed == true) and 
					#the numbers of open brackets and closed brackets are equal (balance == 0) then it is the end of the scope
					while processed == false or balance != 0
						counter = counter + 1
						if temptext[counter] == '"'
							if skipping == true
								skipping = false
							else
								skipping = true
							end
						end
						if temptext[counter] == '{' and skipping == false
							processed = true
							balance = balance + 1
						elsif temptext[counter] == '}' and skipping == false
							processed = true
							balance = balance - 1
						end
					end
					usefultext = temptext[0..counter]
					#Add links. The logic is the same as previous case
					appearances = usefultext.split(/(?<=\n)/)
					commenttag = 0
					appearances.each do |temp|
						commenttag = commenttag + temp.scan(/(?=\/\*)/).count - temp.scan(/(?=\*\/)/).count
						if not temp.include? (">"+var[0]) and commenttag == 0 and not temp =~ (Regexp.new "//.*"+var[0])
							if not temp.include? "<a name=\""+var[2].to_s+"\">"
								temp = temp.gsub((Regexp.new '(?<=[^\w(//.*)])'+var[0]+'(?=[^\w<])'),"<a href=\""+var[1].split("/").last.gsub(/\./,"_")+".html"+"#"+var[2].to_s+"\">"+var[0]+"</a>")
								text = text + temp
							else
								temp = temp.gsub(var[0],"<a name=\"searched\">"+var[0]+"</a>")
								text = text + temp
							end
						else
							text = text + temp
						end
					end
					text = text + temptext[counter+1..-1]
				end
			#Case for local variables in functions and local variables in logical blocks
			#The logic is the same with parameter case. The only difference is that when checking the scope,
			#there should be one more closing brackets than opening brackets, so balance == -1.
			else
				if var[1] == filename.split(/\//).last
					processed = false
					balance = 0
					counter = 0
					splitter = text.split(Regexp.new "(?=<a name=\""+var[2].to_s+"\">)")
					text = splitter.first
					temptext = splitter.last
					skipping = false
					while processed == false or balance != -1
						counter = counter + 1
						if temptext[counter] == '"'
							if skipping == true
								skipping = false
							else
								skipping = true
							end
						end
						if temptext[counter] == '{' and skipping == false
							processed = true
							balance = balance + 1
						elsif temptext[counter] == '}' and skipping == false
							processed = true
							balance = balance - 1
						end
					end
					
					usefultext = temptext[0..counter]
					appearances = usefultext.split(/(?<=\n)/)
					commenttag = 0
					appearances.each do |temp|
						commenttag = commenttag + temp.scan(/(?=\/\*)/).count - temp.scan(/(?=\*\/)/).count
						if not temp.include? (">"+var[0]) and commenttag == 0 and not temp =~ (Regexp.new "//.*"+var[0])
							if not temp.include? "<a name=\""+var[2].to_s+"\">"
								temp = temp.gsub((Regexp.new '(?<=[^\w(//.*)])'+var[0]+'(?=[^\w<])'),"<a href=\""+var[1].split("/").last.gsub(/\./,"_")+".html"+"#"+var[2].to_s+"\">"+var[0]+"</a>")
								text = text + temp
							else
								temp = temp.gsub(var[0],"<a name=\"searched\">"+var[0]+"</a>")
								text = text + temp
							end
						else
							text = text + temp
						end
					end
					text = text + temptext[counter+1..-1]
				end
			end
		end
		#Write the content into the folder
		File.open('html/'+filename.split("/").last.gsub(/\./,"_")+".html", "w") do |out|
			out.write(text)
		end
		#Link the file in index
		index.write('<a href='+filename.split("/").last.gsub(/\./,"_")+'.html>'+filename.split("/").last+"</a>\n")
	end
end
Dir.mkdir 'html' unless File.exists? 'html'
#Create and initialize index.html, process source files, and end the html
File.open('html/index.html',"w") do |out|
	out.write('<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/><title>index.html</title></head><body>'+Dir.pwd+'<br>'+Time.now.strftime("%d/%m/%Y %H:%M")+'<pre>')
	convert(symbols, out)
	out.write('</pre></body></html>')
end