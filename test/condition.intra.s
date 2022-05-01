.text
main:
addiu, $sp, $sp, -16
sw, $ra, 12($sp)
# start of block - loading into registers
# variable i is assigned register $s0
lw, $s0, 4($sp), # load from i
li, $s0, 5
# begin spilling
# end of block
ble, $s0, 42, label0 # if (i <= 42) goto label0
# start of block - loading into registers
li, $t0, 0
sw, $t0, 8($sp), # store to j
# begin spilling
# end of block
j, label1
label0:
# start of block - loading into registers
li, $t0, 1
sw, $t0, 8($sp), # store to j
# begin spilling
# end of block
j, label1
label1:
# start of block - loading into registers
# variable j is assigned register $s0
lw, $s0, 8($sp), # load from j
# begin spilling
# end of block
move, $a0, $s0, # move of j to fn arg/ret
li, $v0, 1
syscall, # printi
# start of block - loading into registers
# begin spilling
# end of block
lw, $ra, 12($sp)
addiu, $sp, $sp, 16
jr, $ra
