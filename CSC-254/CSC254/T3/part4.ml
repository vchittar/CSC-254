let rec fact x = 
	if x = 1 then 1
	else x*(fact (x-1));;
	
let binomial n k = 
	fact n / (fact k * fact (n - k));;