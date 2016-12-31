let rec sort x =
	match x with
	| [] -> []
	| i::l -> insert i (sort l)
	and insert elem li= 
	match li with
	| [] -> [elem]
	| i::l -> if elem < i then elem::i::l
				else i::insert elem l;;