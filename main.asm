call get_6_names
call print_names
main:
	


jmp main

current_table:
times dd 0 24 

include "get_menu_names.asm"

print_names:
ret
