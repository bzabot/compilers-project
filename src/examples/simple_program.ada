procedure Simple is
begin
    x := 10;
    y := 20;
    z := x + y;
    put_line(z);
    if z > 15 then
        put_line(1);
    else
        put_line(0);
    end if;
end Simple;
