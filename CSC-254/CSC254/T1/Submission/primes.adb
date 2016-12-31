 with Ada.Text_IO; use Ada.Text_IO;

    procedure primes_gen is
 	type Range_Type is range -5 .. 10;

   

   	    package Int_IO is new Ada.Text_IO.Integer_IO(INTEGER);
   		use Int_IO;
  
   		package Enum_IO is new Ada.Text_IO.Enumeration_IO(BOOLEAN);
   		use Enum_IO;

    n: Integer:=42;
begin
      if n <= 1 then Put(False);
      elsif n <= 3 then Put(True);
      elsif n mod 2 = 0 or n mod 3 = 0 then Put(False);
      end if;
	end prime_check;



end primes_gen;

  
