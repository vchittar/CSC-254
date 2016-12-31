require_relative "dumpreader.rb"

#read symbol table from dump
symbols = readdump(`dwarfdump #{ARGV[0]}`)
#print symbols
#print symbols
#symbols = readdump("dumpnew.txt")
#Source and Target Files
sourceFiles = []
targetFiles = []

#read/parse
#parseC = 0

#TODO:
#False detect partially matched variable names
#False detect comments
#False detect string contents
#Headers
#EC

def convert(tableread)
	tableread[0].each do |filename|
		#puts filename
		text = File.open(filename).read
		text = text.gsub("<", "&lt").gsub(">", "&gt")
		#convert code to html
		textlines = text.split(/\r*\n/)
		counter = 1
		text = "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/></head>\n<body><pre>"
		textlines.each do |line|
			line = "<a name=\""+counter.to_s+"\">"+line+"</a>\n"
			text += line
			counter = counter + 1
		end
		text = text + "</pre></body></html>"
		#modify text based on dump (may need to reorder it)
		tableread[1].each do |var|
			#puts var[0]+" "+var[1]+" "+var[2].to_s+" "+var[3].to_s
			if var.last == -2 or var.last == 1
				#Need to change this to allow multi-files
				appearances = text.split(/(?<=\n)/)
				text = ""
				appearances.each do |temp|
					if not temp.include? (">"+var[0])
						if not temp.include? "<a name=\""+var[2].to_s+"\">" or filename != var[1]
							if temp =~ (temp.scan '\W'+var[0]+'\W')
								temp = temp.gsub(var[0],"<a href=\""+var[1].gsub(/\./,"_")+".html"+"#"+var[2].to_s+"\">"+var[0]+"</a>")
							end
							text = text + temp
						else
							temp = temp.gsub(var[0],"<a name=\"searched\">"+var[0]+"</a>")
							text = text + temp
						end
					else
						text = text + temp
					end
				end
				#text = text.gsub(var[0],"<a href=\""+var[1]+"#"+var[2].to_s+"\">"+var[0]+"</a>")
			elsif var.last == -1
				if var[1] == filename.split(/\//).last
					processed = false
					balance = 0
					counter = 0
					splitter = text.split(Regexp.new "(?=<a name=\""+var[2].to_s+"\">)")
					text = splitter.first
					temptext = splitter.last
					skipping = false
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
					
					appearances = usefultext.split(/(?<=\n)/)
					appearances.each do |temp|
						if not temp.include? (">"+var[0])
							if not temp.include? "<a name=\""+var[2].to_s+"\">"
								if temp =~ (Regexp.new '\W'+var[0]+'\W',true)
									temp = temp.gsub(var[0],"<a href=\""+var[1].gsub(/\./,"_")+".html"+"#"+var[2].to_s+"\">"+var[0]+"</a>")
								end
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
			elsif var.last == -5
				if var[1] == filename.split(/\//).last
					appearances = text.split(Regexp.new "(?<="+var[0]+")")
					text = ""
					appearances.each do |temp|
						if not temp.include? (">"+var[0])
							if not temp.include? "<a name=\""+var[2].to_s+"\">"
								if temp =~ (Regexp.new '\W'+var[0]+'\W',true)
									temp = temp.gsub(var[0],"<a href=\""+var[1].gsub(/\./,"_")+".html"+"#"+var[2].to_s+"\">"+var[0]+"</a>")
								end
								text = text + temp
							else
								temp = temp.gsub(var[0],"<a name=\"searched\">"+var[0]+"</a>")
								text = text + temp
							end
						else
							text = text + temp
						end
					end
				end
				#puts "Local: "+ appearances
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
					
					#appearances = usefultext.split(Regexp.new "(?<="+var[0]+")")
					appearances = usefultext.split(/(?<=\n)/)
					appearances.each do |temp|
						if not temp.include? (">"+var[0])
							if not temp.include? "<a name=\""+var[2].to_s+"\">"
								if temp =~ (Regexp.new '\W'+var[0]+'\W',true)
									temp = temp.gsub((Regexp.new '(?<=\W'+var[0]+'\W)'),"<a href=\""+var[1].gsub(/\./,"_")+".html"+"#"+var[2].to_s+"\">"+var[0]+"</a>")
								end
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
		#htmlParse(text)

		#print text
		#text = text.gsub(/\r*\n/, "<br>\n")
		#puts text
		File.open(filename.gsub(/\./,"_")+".html", "w") do |out|
			out.write(text)
			#out.write(replaceGT)
		end
	end
end

#Get all filenames of current folder
#filenames = Dir.entries(".")
#filenames.each do |filename|
	#If this file is a C source or header file, generate html
	#if filename =~ /\.c$/i or filename =~ /\.h$/i
		#puts filename
		#convert(filename,symbols)
	#end
#end
convert(symbols)