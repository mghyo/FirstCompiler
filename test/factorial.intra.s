.data
r_st_1_0: .word 0
.text
fact_st_1_0:
addiu, $sp, $sp, -56
sw, $ra, 52($sp)
sw, $a0, 48($sp), # store to n_stf_fact_2_0
# start of block - loading into registers
# variable n_stf_fact_2_0 is assigned register $s0
lw, $s0, 48($sp), # load from n_stf_fact_2_0
# variable $temp2 is assigned register $s1
lw, $s1, 20($sp), # load from $temp2
# variable $temp1 is assigned register $s2
lw, $s2, 16($sp), # load from $temp1
li, $t0, 0
sw, $t0, 28($sp), # store to $temp4
move, $s2, $s0
li, $s1, 1
li, $t0, 1
sw, $t0, 24($sp), # store to $temp3
# begin spilling
# end of block
bne, $s2, $s1, cond_0_stz_stf_fact_2_0 # if ($temp1 != $temp2) goto cond_0_stz_stf_fact_2_0
# start of block - loading into registers
# begin spilling
# end of block
j, cond_1_after_stf_fact_2_0
cond_0_stz_stf_fact_2_0:
# start of block - loading into registers
li, $t0, 0
sw, $t0, 24($sp), # store to $temp3
# begin spilling
# end of block
cond_1_after_stf_fact_2_0:
# start of block - loading into registers
# variable $temp4 is assigned register $s0
lw, $s0, 28($sp), # load from $temp4
# variable $temp3 is assigned register $s1
lw, $s1, 24($sp), # load from $temp3
# begin spilling
# end of block
beq, $s1, $s0, if_after2__stf_fact_2_0 # if ($temp3 == $temp4) goto if_after2__stf_fact_2_0
# start of block - loading into registers
# variable $temp0 is assigned register $s0
lw, $s0, 12($sp), # load from $temp0
li, $s0, 1
# begin spilling
# end of block
move, $v0, $s0, # move of $temp0 to fn arg/ret
lw, $ra, 52($sp)
addiu, $sp, $sp, 56
jr, $ra
if_after2__stf_fact_2_0:
# start of block - loading into registers
# variable n_stf_fact_2_0 is assigned register $s0
lw, $s0, 48($sp), # load from n_stf_fact_2_0
# variable $temp6 is assigned register $s1
lw, $s1, 40($sp), # load from $temp6
# variable $temp5 is assigned register $s2
lw, $s2, 36($sp), # load from $temp5
move, $s2, $s0
li, $s1, 1
sub, $t2, $s2, $s1
sw, $t2, 44($sp), # store to $temp7
# begin spilling
# end of block
lw, $a0, 44($sp), # load from $temp7
jal, fact_st_1_0
sw, $v0, 0($sp), # store to $temp8
# start of block - loading into registers
# variable n_stf_fact_2_0 is assigned register $s0
lw, $s0, 48($sp), # load from n_stf_fact_2_0
# variable $temp9 is assigned register $s1
lw, $s1, 4($sp), # load from $temp9
# variable $temp8 is assigned register $s2
lw, $s2, 0($sp), # load from $temp8
# variable $temp11 is assigned register $s3
lw, $s3, 32($sp), # load from $temp11
# variable $temp10 is assigned register $s4
lw, $s4, 8($sp), # load from $temp10
sw, $s2, r_st_1_0, # store to r_st_1_0
move, $s1, $s0
lw, $t0, r_st_1_0, # load from r_st_1_0
move, $s4, $t0
mul, $s3, $s1, $s4
# begin spilling
# end of block
move, $v0, $s3, # move of $temp11 to fn arg/ret
lw, $ra, 52($sp)
addiu, $sp, $sp, 56
jr, $ra
main:
addiu, $sp, $sp, -20
sw, $ra, 16($sp)
# start of block - loading into registers
li, $t0, 1
sw, $t0, r_st_1_0, # store to r_st_1_0
# begin spilling
# end of block
# start of block - loading into registers
li, $t0, 5
sw, $t0, 0($sp), # store to $temp0
# begin spilling
# end of block
lw, $a0, 0($sp), # load from $temp0
jal, fact_st_1_0
sw, $v0, 4($sp), # store to $temp1
# start of block - loading into registers
# variable r_st_1_0 is assigned register $s0
lw, $s0, r_st_1_0, # load from r_st_1_0
# variable $temp2 is assigned register $s1
lw, $s1, 8($sp), # load from $temp2
# variable $temp1 is assigned register $s2
lw, $s2, 4($sp), # load from $temp1
move, $s0, $s2
move, $s1, $s0
# begin spilling
# end of block
move, $a0, $s1, # move of $temp2 to fn arg/ret
li, $v0, 1
syscall, # printi
# start of block - loading into registers
# begin spilling
# end of block
lw, $ra, 16($sp)
addiu, $sp, $sp, 20
jr, $ra
