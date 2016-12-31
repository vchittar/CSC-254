isprime :: Num ((a -> a -> a) -> a1 -> a2)
isprime input = if input <= 1 then False else if input <= 3 then True else if input mod 2 == 0 || input mod 3 == 0 then False else True

main = do 
if isprime 8
then putStrLn "Scheme sucks too"
else putStrLn "Screw Ocaml"