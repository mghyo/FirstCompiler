.text
main:
# enter main
# variable i assigned register 1
# variable j assigned register 0
addiu, $sp, $sp, -16
sw, $ra, 12($sp)
li, $s1, 5, # store to i
ble, $s1, 42, label0 # if (i <= 42) goto label0
li, $s0, 0, # store to j
j, label1
label0:
li, $s0, 1, # store to j
j, label1
label1:
move, $a0, $s0, # move of j to fn arg/ret
li, $v0, 1
syscall, # printi
lw, $ra, 12($sp)
addiu, $sp, $sp, 16
jr, $ra
