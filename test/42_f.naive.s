.text
main:
addiu, $sp, $sp, -12
sw, $ra, 8($sp)
li.s, $f0, 40.0
li.s, $f2, 2.0
add.s, $f4, $f0, $f2
s.s, $f4, 0($sp), # store to $temp1
l.s, $f0, 0($sp), # load from $temp1
s.s, $f0, 4($sp), # store to i
l.s, $f12, 4($sp), # load from i
li, $v0, 2
syscall, # printf
lw, $ra, 8($sp)
addiu, $sp, $sp, 12
jr, $ra
