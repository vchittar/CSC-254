let is_prime n =
  if n = 2 then true
  else if n < 2 || n mod 2 = 0 then false
  else
    let rec loop k =
      if k * k > n then true
      else if n mod k = 0 then false
      else loop (k+2)
    in loop 3;;
	
let genprime n =
	let li = 2::[] in
	let rec prime_rec (i: int) (l: int list): int list= 
		if i = n then l
		else 
			if is_prime i = true 
			then (prime_rec (i+1) (i::l))
			else (prime_rec (i+1) l)
	in prime_rec 3 li;;


let prime_partitions_rec input k =
	let primes = (genprime input) in
	let rec partitions_rec_rec (i: int) (l: int list list): (int list) list =
		if i >= ((List.length primes) / 2) 
		then l
		else if (((List.nth primes i) <> (input - (List.nth primes i))) && is_prime (input - (List.nth primes i))) 
				then partitions_rec_rec (i+1) ([(List.nth primes i);(input - (List.nth primes i))]::l)
				else partitions_rec_rec (i+1) l
	in partitions_rec_rec k [];;
					

let rec print_list = function
[] -> ()
| e::l -> print_int e; print_string" ";print_list l;;

let rec print_list_list l i = 
	if i = ((List.length l)-1)
		then begin
			print_list (List.nth l i);
			print_string "\n" end
		else begin print_list (List.nth l i);
			print_string "\n";
			print_list_list l (i+1) end;; 

print_list_list (prime_partitions_rec 100 0) 0;