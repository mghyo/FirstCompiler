.text
main:
addiu, $sp, $sp, -12
sw, $ra, 8($sp)
# start of block - loading into registers
# variable i is assigned register $s0
lw, $s0, 4($sp), # load from i
# variable $temp1 is assigned register $s1
lw, $s1, 0($sp), # load from $temp1
li, $t0, 40
add, $s1, $t0, 2
move, $s0, $s1
# begin spilling
# end of block
move, $a0, $s0, # move of i to fn arg/ret
li, $v0, 1
syscall, # printi
# start of block - loading into registers
# begin spilling
# end of block
lw, $ra, 8($sp)
addiu, $sp, $sp, 12
jr, $ra
