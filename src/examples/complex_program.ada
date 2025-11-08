procedure Complex_Program is
begin
   a := 10;
   b := 20;
   c := 30;
   d := 40;
   result1 := (a + b) * (c - d) / 2;
   result2 := a * b + c * d - (a + c) * (b + d);
   if result1 > result2 then
      if a > b then
         max_val := a;
      else
         max_val := b;
      end if;
      Put_Line (max_val);
   else
      if c > d then
         max_val := c;
      else
         max_val := d;
      end if;
      Put_Line (max_val);
   end if;
   counter := 0;
   while counter < 3 loop
      temp := counter * 2;
      if temp > 2 then
         Put_Line (temp);
      else
         other := temp + 5;
         Put_Line (other);
      end if;
      counter := counter + 1;
   end loop;
   if (a < b and c > d) or (result1 = result2) then
      Put_Line (100);
   else
      Put_Line (200);
   end if;
   final := ((a + b) * c) - (d / 2) + (result1 mod 10);
   Put_Line (final);
end Complex_Program;
