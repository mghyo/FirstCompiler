.data
r_st_1_0: .word 0
.text
fact_st_1_0:
addiu, $sp, $sp, -56
sw, $ra, 52($sp)
sw, $a0, 48($sp), # store to n_stf_fact_2_0
li, $t0, 0
sw, $t0, 28($sp), # store to $temp4
lw, $t0, 48($sp), # load from n_stf_fact_2_0
sw, $t0, 16($sp), # store to $temp1
li, $t0, 1
sw, $t0, 20($sp), # store to $temp2
li, $t0, 1
sw, $t0, 24($sp), # store to $temp3
lw, $t0, 16($sp), # load from $temp1
lw, $t1, 20($sp), # load from $temp2
bne, $t0, $t1, cond_0_stz_stf_fact_2_0 # if ($temp1 != $temp2) goto cond_0_stz_stf_fact_2_0
j, cond_1_after_stf_fact_2_0
cond_0_stz_stf_fact_2_0:
li, $t0, 0
sw, $t0, 24($sp), # store to $temp3
cond_1_after_stf_fact_2_0:
lw, $t0, 24($sp), # load from $temp3
lw, $t1, 28($sp), # load from $temp4
beq, $t0, $t1, if_after2__stf_fact_2_0 # if ($temp3 == $temp4) goto if_after2__stf_fact_2_0
li, $t0, 1
sw, $t0, 12($sp), # store to $temp0
lw, $v0, 12($sp), # load from $temp0
lw, $ra, 52($sp)
addiu, $sp, $sp, 56
jr, $ra
if_after2__stf_fact_2_0:
lw, $t0, 48($sp), # load from n_stf_fact_2_0
sw, $t0, 36($sp), # store to $temp5
li, $t0, 1
sw, $t0, 40($sp), # store to $temp6
lw, $t0, 36($sp), # load from $temp5
lw, $t1, 40($sp), # load from $temp6
sub, $t2, $t0, $t1
sw, $t2, 44($sp), # store to $temp7
lw, $a0, 44($sp), # load from $temp7
jal, fact_st_1_0
sw, $v0, 0($sp), # store to $temp8
lw, $t0, 0($sp), # load from $temp8
sw, $t0, r_st_1_0, # store to r_st_1_0
lw, $t0, 48($sp), # load from n_stf_fact_2_0
sw, $t0, 4($sp), # store to $temp9
lw, $t0, r_st_1_0, # load from r_st_1_0
sw, $t0, 8($sp), # store to $temp10
lw, $t0, 4($sp), # load from $temp9
lw, $t1, 8($sp), # load from $temp10
mul, $t2, $t0, $t1
sw, $t2, 32($sp), # store to $temp11
lw, $v0, 32($sp), # load from $temp11
lw, $ra, 52($sp)
addiu, $sp, $sp, 56
jr, $ra
main:
addiu, $sp, $sp, -20
sw, $ra, 16($sp)
li, $t0, 1
sw, $t0, r_st_1_0, # store to r_st_1_0
li, $t0, 5
sw, $t0, 0($sp), # store to $temp0
lw, $a0, 0($sp), # load from $temp0
jal, fact_st_1_0
sw, $v0, 4($sp), # store to $temp1
lw, $t0, 4($sp), # load from $temp1
sw, $t0, r_st_1_0, # store to r_st_1_0
lw, $t0, r_st_1_0, # load from r_st_1_0
sw, $t0, 8($sp), # store to $temp2
lw, $a0, 8($sp), # load from $temp2
li, $v0, 1
syscall, # printi
lw, $ra, 16($sp)
addiu, $sp, $sp, 20
jr, $ra
