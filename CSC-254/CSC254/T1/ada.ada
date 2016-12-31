--
-- Trivial function.
--
with Gnat.Io; use Gnat.Io;
procedure f1 is
   -- A small function.
   function Sumsqr(X, Y: Integer) return Integer is
   begin
      return X*X + Y*Y;
   end;

   -- How 'bout a nice, tender variable?
   I: Integer;
begin
   I := Sumsqr(3, 14);
   Put(I);
   New_Line;

   Put(Sumsqr(I, 4));
   New_Line;
end f1;