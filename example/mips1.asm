j start
func: addi $s1, $s1, 1
jr $ra
start: addi $t1, $zero, 100
addi $t2, $zero, 200
add $t3, $t1, $t2
sub $t4, $t2, $t1
and $t5, $t1, $t2
or $t6, $t1, $t2
sll $s1, $t1, 2
sw $s1, 0($sp)
addi $s1, $s1, 100
lw $s1, 0($sp)
jal func
beq $s1, $zero, L1
addi $s1, $s1, 1
L1: addi $s2, $s1, 1