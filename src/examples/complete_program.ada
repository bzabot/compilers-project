procedure Complete_Example is
begin
   result := (5 + 3) * 2;
   Put_Line (result);
   counter := 0;
   while counter < 5 loop
      if counter mod 2 = 0 then
         Put_Line (counter);
      else
         temp := counter + 10;
         Put_Line (temp);
      end if;
      counter := counter + 1;
   end loop;
   Get_Line (user_input);
   Put_Line (user_input);
end Complete_Example;
