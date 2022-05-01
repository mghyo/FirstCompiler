.text
foo:
addiu, $sp, $sp, -4
sw, $ra, 0($sp)
# start of block - loading into registers
# begin spilling
# end of block
li, $v0, 42
lw, $ra, 0($sp)
addiu, $sp, $sp, 4
jr, $ra
main:
addiu, $sp, $sp, -8
sw, $ra, 4($sp)
# start of block - loading into registers
# begin spilling
# end of block
jal, foo
sw, $v0, 0($sp), # store to $t0
# start of block - loading into registers
# variable $t0 is assigned register $s0
lw, $s0, 0($sp), # load from $t0
# begin spilling
# end of block
move, $a0, $s0, # move of $t0 to fn arg/ret
li, $v0, 1
syscall, # printi
# start of block - loading into registers
# begin spilling
# end of block
lw, $ra, 4($sp)
addiu, $sp, $sp, 8
jr, $ra
