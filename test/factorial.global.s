.data
r_st_1_0: .word 0
.text
fact_st_1_0:
# enter fact_st_1_0
# variable $temp0 is spilled!
# variable $temp1 is spilled!
# variable $temp10 is spilled!
# variable $temp11 is spilled!
# variable $temp2 assigned register 7
# variable $temp3 assigned register 6
# variable $temp4 assigned register 5
# variable $temp5 assigned register 4
# variable $temp6 assigned register 3
# variable $temp8 assigned register 2
# variable $temp9 assigned register 1
# variable n_stf_fact_2_0 assigned register 0
addiu, $sp, $sp, -56
sw, $ra, 52($sp)
move, $s0, $a0, # store to n_stf_fact_2_0
li, $s5, 0, # store to $temp4
sw, $s0, 16($sp), # store to $temp1
li, $s7, 1, # store to $temp2
li, $s6, 1, # store to $temp3
lw, $t0, 16($sp), # load from $temp1
bne, $t0, $s7, cond_0_stz_stf_fact_2_0 # if ($temp1 != $temp2) goto cond_0_stz_stf_fact_2_0
j, cond_1_after_stf_fact_2_0
cond_0_stz_stf_fact_2_0:
li, $s6, 0, # store to $temp3
cond_1_after_stf_fact_2_0:
beq, $s6, $s5, if_after2__stf_fact_2_0 # if ($temp3 == $temp4) goto if_after2__stf_fact_2_0
li, $t0, 1
sw, $t0, 12($sp), # store to $temp0
lw, $v0, 12($sp), # load from $temp0
lw, $ra, 52($sp)
addiu, $sp, $sp, 56
jr, $ra
if_after2__stf_fact_2_0:
move, $s4, $s0, # store to $temp5
li, $s3, 1, # store to $temp6
sub, $t2, $s4, $s3
sw, $t2, 44($sp), # store to $temp7
lw, $a0, 44($sp), # load from $temp7
# spilling for jal
sw, $s2, 0($sp), # store to $temp8
sw, $s0, 48($sp), # store to n_stf_fact_2_0
jal, fact_st_1_0
# unspilling
lw, $s2, 0($sp), # load from $temp8
lw, $s0, 48($sp), # load from n_stf_fact_2_0
move, $s2, $v0, # store to $temp8
sw, $s2, r_st_1_0, # store to r_st_1_0
move, $s1, $s0, # store to $temp9
lw, $t0, r_st_1_0, # load from r_st_1_0
sw, $t0, 8($sp), # store to $temp10
lw, $t1, 8($sp), # load from $temp10
mul, $t2, $s1, $t1
sw, $t2, 32($sp), # store to $temp11
lw, $v0, 32($sp), # load from $temp11
lw, $ra, 52($sp)
addiu, $sp, $sp, 56
jr, $ra
main:
# enter main
# variable $temp1 assigned register 1
# variable $temp2 assigned register 0
addiu, $sp, $sp, -20
sw, $ra, 16($sp)
li, $t0, 1
sw, $t0, r_st_1_0, # store to r_st_1_0
li, $t0, 5
sw, $t0, 0($sp), # store to $temp0
lw, $a0, 0($sp), # load from $temp0
# spilling for jal
sw, $s1, 4($sp), # store to $temp1
jal, fact_st_1_0
# unspilling
lw, $s1, 4($sp), # load from $temp1
move, $s1, $v0, # store to $temp1
sw, $s1, r_st_1_0, # store to r_st_1_0
lw, $t0, r_st_1_0, # load from r_st_1_0
move, $s0, $t0, # store to $temp2
move, $a0, $s0, # move of $temp2 to fn arg/ret
li, $v0, 1
syscall, # printi
lw, $ra, 16($sp)
addiu, $sp, $sp, 20
jr, $ra
