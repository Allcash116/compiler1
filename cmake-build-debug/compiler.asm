TITLE ;KOvrigin vladislav m3o-232b-22
INCLUDE Irvine32.inc

.data
.code
main PROC
push 5
push 3
pop eax
pop ebx
cmp eax, ebx
ja pos0
push 0
jmp neg0
pos0:
push 1
neg0:
pop eax
cmp eax, 0
jne START1
jmp else1
START1:

push ' '
push 33
push ' '
push 100
push 108
push 114
push 111
push 119
push ' '
push 111
push 108
push 108
push 101
push 104
mov ecx, 14
print_Loop2:
pop eax
call WriteChar
loop print_Loop2
call crlf

End1:
pop eax
cmp eax, 0
 jmp end3
else1:

push ' '
push 97
push 121
push 108
push 97
push 103
push ' '
push 111
push 108
push 108
push 101
push 104
mov ecx, 12
print_Loop4:
pop eax
call WriteChar
loop print_Loop4
call crlf

End3:
exit
main ENDP

END main
