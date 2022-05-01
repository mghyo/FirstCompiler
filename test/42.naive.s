.text
main:
addiu, $sp, $sp, -12
sw, $ra, 8($sp)
li, $t0, 40
add, $t2, $t0, 2
sw, $t2, 0($sp), # store to $temp1
lw, $t0, 0($sp), # load from $temp1
sw, $t0, 4($sp), # store to i
lw, $a0, 4($sp), # load from i
li, $v0, 1
syscall, # printi
lw, $ra, 8($sp)
addiu, $sp, $sp, 12
jr, $ra
