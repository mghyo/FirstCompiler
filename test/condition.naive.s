.text
main:
addiu, $sp, $sp, -16
sw, $ra, 12($sp)
li, $t0, 5
sw, $t0, 4($sp), # store to i
lw, $t0, 4($sp), # load from i
ble, $t0, 42, label0 # if (i <= 42) goto label0
li, $t0, 0
sw, $t0, 8($sp), # store to j
j, label1
label0:
li, $t0, 1
sw, $t0, 8($sp), # store to j
j, label1
label1:
lw, $a0, 8($sp), # load from j
li, $v0, 1
syscall, # printi
lw, $ra, 12($sp)
addiu, $sp, $sp, 16
jr, $ra
