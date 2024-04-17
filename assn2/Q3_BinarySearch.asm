################################################################################

.data

newline:
  .asciiz "\n"
space:
  .asciiz " "
str0:
  .asciiz "INFO: array[] = "
str1:
  .asciiz "INFO: $t0 = "
str2:
  .asciiz ", $t1 = "
str3:
  .asciiz "INFO: M = "

strv0:
  .asciiz "INFO: $v0 = "

array0:
  .align 4
  .word 1,2,3,4,5,6,7,8

################################################################################

.text

# $a0 = n, $a1 = &array[0]
printArray:
  addi $sp, $sp, -16
  sw $ra,  0($sp)
  sw $s0,  4($sp)
  sw $s1,  8($sp)
  sw $s2, 12($sp)
  move $s0, $a0
  li $s1, 0
  move $s2, $a1
  li $v0, 4
  la $a0, str0
  syscall
printArray_loop:
  beq $s1, $s0, printArray_return
  li $v0, 1
  lw $a0, ($s2)
  syscall
  li $v0, 4
  la $a0, space
  syscall
  addi $s1, $s1, 1
  addi $s2, $s2, 4
  j printArray_loop
printArray_return:
  li $v0, 4
  la $a0, newline
  syscall
  lw $ra,  0($sp)
  lw $s0,  4($sp)
  lw $s1,  8($sp)
  lw $s2, 12($sp)
  addi $sp, $sp, 16
  jr $ra

# binarySearch
#
#   $a0: N
#   $a1: &array[0]
#   $a2: M
#
#   $v0: (M exists in array) ? index : -1
#
binarySearch:

  # stack <-- $ra, $s0, $s1, $s2, $s3
  addi $sp, $sp, -20
  sw $ra,  0($sp)
  sw $s0,  4($sp)
  sw $s1,  8($sp)
  sw $s2, 12($sp)
  sw $s3, 16($sp)

  # $s0 = $a0; $s1 = $a1; $s2 = $a2
  move $s0, $a0
  move $s1, $a1
  move $s2, $a2

  # printArray($s1)
  move $a0, $s0
  move $a1, $s1
  jal printArray

  # print_string(str3); print_int(M); print_string(newline)
  li $v0, 4
  la $a0, str3
  syscall
  li $v0, 1
  move $a0, $s2
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  # $t0 = 0; $t1 = N; $v0 = -1
  move $t0, $zero
  move $t1, $s0
  li $v0, -1

binarySearch0:

  # $s3 = $v0
  move $s3, $v0

  # print_string(str1); print_int($t0); print_string(str2); print_int($t1); print_str(newline)
  li $v0, 4
  la $a0, str1
  syscall
  li $v0, 1
  move $a0, $t0
  syscall
  li $v0, 4
  la $a0, str2
  syscall
  li $v0, 1
  move $a0, $t1
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  # $v0 = $s3
  move $v0, $s3

  add $t2, $t0, $t1
  srl $t2, $t2, 1

  sll $t3, $t2, 2
  add $t3, $t3, $s1
  lw $t4, 0($t3)

  bne $t4 $s2, continue
  move $v0, $t2

continue:
  beq $t0, $t1, binarySearch1
  bgt $t4, $s2, searchRight

searchLeft:
  move $t1, $t2
  j binarySearch0

searchRight:
  move $t0, $t2
  j binarySearch0

binarySearch1:

  # $ra, $s0, $s1, $s2, $s3 <-- stack
  lw $ra,  0($sp)
  lw $s0,  4($sp)
  lw $s1,  8($sp)
  lw $s2, 12($sp)
  lw $s3, 16($sp)
  addi $sp, $sp, 20

  # return
  jr $ra

.globl main
main:

  # stack <-- $ra, $s0
  addi $sp, $sp, -8
  sw $ra, 0($sp)
  sw $s0, 4($sp)

## Example 1
  # binarySearch(8, array0, 5)
  li $a0, 8
  la $a1, array0
  li $a2, 5
  jal binarySearch
  # $s0 = $v0; print_string(strv0); print_int($s0); print_string(newline)
  move $s0, $v0
  li $v0, 4
  la $a0, strv0
  syscall
  li $v0, 1
  move $a0, $s0
  syscall
  li $v0, 4
  la $a0, newline
  syscall

## Example 2
  # binarySearch(8, array0, 2)
  li $a0, 8
  la $a1, array0
  li $a2, 2
  jal binarySearch
  # $s0 = $v0; print_string(strv0); print_int($s0); print_string(newline)
  move $s0, $v0
  li $v0, 4
  la $a0, strv0
  syscall
  li $v0, 1
  move $a0, $s0
  syscall
  li $v0, 4
  la $a0, newline
  syscall

## Example 3
  # binarySearch(8, array0, 7)
  li $a0, 8
  la $a1, array0
  li $a2, 7
  jal binarySearch
  # $s0 = $v0; print_string(strv0); print_int($s0); print_string(newline)
  move $s0, $v0
  li $v0, 4
  la $a0, strv0
  syscall
  li $v0, 1
  move $a0, $s0
  syscall
  li $v0, 4
  la $a0, newline
  syscall

## Example 4
  # binarySearch(8, array0, 1)
  li $a0, 8
  la $a1, array0
  li $a2, 1
  jal binarySearch
  # $s0 = $v0; print_string(strv0); print_int($s0); print_string(newline)
  move $s0, $v0
  li $v0, 4
  la $a0, strv0
  syscall
  li $v0, 1
  move $a0, $s0
  syscall
  li $v0, 4
  la $a0, newline
  syscall

## Example 5
  # binarySearch(8, array0, 0)
  li $a0, 8
  la $a1, array0
  li $a2, 0
  jal binarySearch
  # $s0 = $v0; print_string(strv0); print_int($s0); print_string(newline)
  move $s0, $v0
  li $v0, 4
  la $a0, strv0
  syscall
  li $v0, 1
  move $a0, $s0
  syscall
  li $v0, 4
  la $a0, newline
  syscall

## Example 5
  # binarySearch(8, array0, 9)
  li $a0, 8
  la $a1, array0
  li $a2, 9
  jal binarySearch
  # $s0 = $v0; print_string(strv0); print_int($s0); print_string(newline)
  move $s0, $v0
  li $v0, 4
  la $a0, strv0
  syscall
  li $v0, 1
  move $a0, $s0
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  # $ra, $s0 <-- stack
  lw $ra, 0($sp)
  lw $s0, 4($sp)
  addi $sp, $sp, 4

  # return;
  jr $ra

################################################################################

