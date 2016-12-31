def readdump(file)
	#List of files
	filenames = []
	#List of symbols
	textsymbols = []
	#Split the dump by each compile unit
	textlist = file.split("DW_TAG_compile_unit\n")
	#Skip the first part
	textlist = textlist[1..-1]
	#Process each unit
	textlist.each do |text|
		splitted = text.split("LOCAL_SYMBOLS:\n")
		#Parse and update filename
		tempname = splitted.first.split(/DW_AT_name\s+/).last.split(/\n/).first.gsub(/\"/,"")
		if not filenames.include? tempname
			filenames.push(tempname)
		end
		#Parse the information needed for each symbol and add them to textsymbols
		text = splitted.last.split(".debug_line").first
		symbols = text.split(/(?=< \d>)/)
		
		#Data structure for textsymbols: [name, file, lineNum, state(number = variable, -1 = parameter, -2 = global functions)]
		symbols.each do |element|
			if (element.include? "DW_TAG_variable" or element.include? "DW_TAG_subprogram" or element.include? "DW_TAG_formal_parameter") and element.include? "DW_AT_decl_file" and not element.include? "/usr/include/"
				state = -1
				if element.include? "DW_TAG_subprogram" and element.include? "< 1>"
					state = -2
				elsif element.include? "DW_TAG_formal_parameter"
					state = -1
				else
					state = element[/.*< ([^>]*)/,1].to_i
				end
				name = ""
				file = ""
				lineNum = 0
				templines = element.split(/\n/)
				templines.each do |line|
					if line.include? "DW_AT_name"
						name = line.split(/\s/).last.gsub(/\"/,"")
					elsif line.include? "DW_AT_decl_file"
						file = line.split(/\//).last
					elsif line.include? "DW_AT_decl_line"
						lineNum = line.split(/(?=0x)/).last.to_i(16)
					end
				end
				#puts "Found"+element
				textsymbols.push([name,file,lineNum,state])
			end
		end
		
		
	end
	#Sort the symbol table so that local variables are processed first
	textsymbols.sort!{|a,b| b[3] <=> a[3]}
	return [filenames, textsymbols]
end