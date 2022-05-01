.text
main:
# enter main
# variable $temp1 assigned register 1
# variable i assigned register 0
addiu, $sp, $sp, -12
sw, $ra, 8($sp)
li, $t0, 40
add, $s1, $t0, 2
move, $s0, $s1, # store to i
move, $a0, $s0, # move of i to fn arg/ret
li, $v0, 1
syscall, # printi
lw, $ra, 8($sp)
addiu, $sp, $sp, 12
jr, $ra
