.text
foo:
# enter foo
addiu, $sp, $sp, -4
sw, $ra, 0($sp)
li, $v0, 42
lw, $ra, 0($sp)
addiu, $sp, $sp, 4
jr, $ra
main:
# enter main
# variable $t0 assigned register 0
addiu, $sp, $sp, -8
sw, $ra, 4($sp)
# spilling for jal
sw, $s0, 0($sp), # store to $t0
jal, foo
# unspilling
lw, $s0, 0($sp), # load from $t0
move, $s0, $v0, # store to $t0
move, $a0, $s0, # move of $t0 to fn arg/ret
li, $v0, 1
syscall, # printi
lw, $ra, 4($sp)
addiu, $sp, $sp, 8
jr, $ra
