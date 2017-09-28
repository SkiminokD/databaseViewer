do $$
begin
for r in 1..1000000 loop
  INSERT INTO channels DEFAULT VALUES;
end loop;
end;
$$;