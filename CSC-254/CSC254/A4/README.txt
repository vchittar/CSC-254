Vishnu Chittari
Edward Du

Project 4 - Cross Indexer 
CSC 254

Included Files:
xref.rb - main code
dumpnew.txt - contains dd output
dumpreader.rb - parse dwarfdump
HTML folder which contains the necessary output for the provided files along with the index page

How to run:
ruby xref.rb ./<program>


The purpose of this assignment was to write a scripting program (xref.rb) that uses the output of dwarfdump to construct a set of web pages. 

We used ruby to write our program. The approach to the problem was done by first parsing the output of dwarfdump and generate parse table and file list. We then sort the symbols by the level they are at (from innermost like local variables in logical blocks to outermost such as global functions) so that when processing, the program will process the local variables first and not worry about same name variables. We then began to parse the said program into its HTML counterpart. We added in the necessary tags and kept a track of the lines by giving each of them a line number so our program could go to the apporpriate declaration of the certain object. We used the generated symbol table and file list and used it in xref.rb. We determined scope in a somewhat special way. Because of the fact that parameters will have a equal number of opening and closing brackets after them and the content in between is the scope, we count the number of the brackets and end when the number of brackets are equal. Local variables are similar, but for local variables, their scope ends when there is one more closing brackets than opening brackets. That's how we determine the scope of each variable. Seperating the files felt appropriate so we could have code clarity. The data used from dumpreader.rb was used in xref.rb to construct the web page. We first started by replacing certain keywords such as "<" or ">" and changing them to their appropriate HTML counterparts. We used dumpreader.rb to store important symbols from the output of dwarfdump to be later used in xref.rb. 

While building the symbol table we used/stored:
DW_TAG_compile_unit
DW_AT_name
DW_AT_decl_file
DW_TAG_variable 
DW_TAG_subprogram
DW_TAG_formal_parameter
DW_TAG_formal_parameter

In order to link the source code to the identifiers we looped through the symbol table to find a matching name. If they were in the same scope and same type they were linked together after being matched. After linking each file, they were then output to their respective .html page. We also made an index.html page so users can pick what part of the file they wanted to examine. We made sure that words such as "Apple" and "pineapple" did not have any conflicts and matched them accurtately. We checked the file to add the file names to index.html. We made sure that the scopes matched based on the parenthesis {}, we kept track of the {} to make sure that we were in the correct scope. 

Extra Credit: We changed the color of certain syntax for our HTML page. 

NOTE: Our project takes about 3-8 mins to run (3 minutes on my Personal Computer and 7:20 on CSUG Cycle 2). Please dont exit in the meantime. 