.text
foo:
addiu, $sp, $sp, -4
sw, $ra, 0($sp)
li, $v0, 42
lw, $ra, 0($sp)
addiu, $sp, $sp, 4
jr, $ra
main:
addiu, $sp, $sp, -8
sw, $ra, 4($sp)
jal, foo
sw, $v0, 0($sp), # store to $t0
lw, $a0, 0($sp), # load from $t0
li, $v0, 1
syscall, # printi
lw, $ra, 4($sp)
addiu, $sp, $sp, 8
jr, $ra
