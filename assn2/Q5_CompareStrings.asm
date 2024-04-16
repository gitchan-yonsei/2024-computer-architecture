################################################################################

.data

newline:
  .asciiz "\n"
str1:
  .asciiz "INFO: $a0 = "
str2:
  .asciiz "INFO: $a1 = "
str3:
  .asciiz "INFO: $v0 = "

inputStr0:
  .align 1
  .asciiz "This is One String!"
inputStr1:
  .align 1
  .asciiz "This is Another String!"
inputStr2:
  .align 1
  .asciiz "ABCDEFGHIJKLMNOP"
inputStr3:
  .align 1
  .asciiz "ABCDEFGHIJKMNLOP"
inputStr4:
  .align 1
  .asciiz "abcdefghijklmnop"

################################################################################

.text

# compareStrings
#
# $a0: the starting memory address of string0
# $a1: the starting memory address of string1
#
# $v0 =  1 (if string0 > string1)
#        0 (if string0 = string1)
#       -1 (if string0 < string1)
compareStrings:

  # stack <-- $ra
  addi $sp, $sp, -4
  sw $ra, 0($sp)

################################################################################
# FIXME

  nop

# FIXME
################################################################################

  # $ra <-- stack
  lw $ra, 0($sp)
  addi $sp, $sp, 4

  # return $v0
  jr $ra

.globl main
main:

  # stack <-- $ra, $s0
  addi $sp, $sp, -8
  sw $ra, 0($sp)
  sw $s0, 4($sp)

  ### Example 1

  # print_string str1; print_string inputStr0; print_string newline
  li $v0, 4
  la $a0, str1
  syscall
  li $v0, 4
  la $a0, inputStr0
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  # print_string str2; print_string inputStr1; print_string newline
  li $v0, 4
  la $a0, str2
  syscall
  li $v0, 4
  la $a0, inputStr1
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  # $v0 = compareStrings(inputStr0, inputStr1)
  la $a0, inputStr0
  la $a1, inputStr1
  jal compareStrings

  # $s0 = $v0
  move $s0, $v0

  # print_string str3; print_int $v0; print_string newline
  li $v0, 4
  la $a0, str3
  syscall
  li $v0, 1
  move $a0, $s0
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  ### Example 2

  # print_string str1; print_string inputStr1; print_string newline
  li $v0, 4
  la $a0, str1
  syscall
  li $v0, 4
  la $a0, inputStr1
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  # print_string str2; print_string inputStr0; print_string newline
  li $v0, 4
  la $a0, str2
  syscall
  li $v0, 4
  la $a0, inputStr0
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  # $v0 = compareStrings(inputStr1, inputStr0)
  la $a0, inputStr1
  la $a1, inputStr0
  jal compareStrings

  # $s0 = $v0
  move $s0, $v0

  # print_string str3; print_int $v0; print_string newline
  li $v0, 4
  la $a0, str3
  syscall
  li $v0, 1
  move $a0, $s0
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  ### Example 3

  # print_string str1; print_string inputStr0; print_string newline
  li $v0, 4
  la $a0, str1
  syscall
  li $v0, 4
  la $a0, inputStr0
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  # print_string str2; print_string inputStr0; print_string newline
  li $v0, 4
  la $a0, str2
  syscall
  li $v0, 4
  la $a0, inputStr0
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  # $v0 = compareStrings(inputStr0, inputStr0)
  la $a0, inputStr0
  la $a1, inputStr0
  jal compareStrings

  # $s0 = $v0
  move $s0, $v0

  # print_string str3; print_int $v0; print_string newline
  li $v0, 4
  la $a0, str3
  syscall
  li $v0, 1
  move $a0, $s0
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  ### Example 4

  # print_string str1; print_string inputStr2; print_string newline
  li $v0, 4
  la $a0, str1
  syscall
  li $v0, 4
  la $a0, inputStr2
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  # print_string str2; print_string inputStr3; print_string newline
  li $v0, 4
  la $a0, str2
  syscall
  li $v0, 4
  la $a0, inputStr3
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  # $v0 = compareStrings(inputStr2, inputStr3)
  la $a0, inputStr2
  la $a1, inputStr3
  jal compareStrings

  # $s0 = $v0
  move $s0, $v0

  # print_string str3; print_int $v0; print_string newline
  li $v0, 4
  la $a0, str3
  syscall
  li $v0, 1
  move $a0, $s0
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  ### Example 5

  # print_string str1; print_string inputStr3; print_string newline
  li $v0, 4
  la $a0, str1
  syscall
  li $v0, 4
  la $a0, inputStr3
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  # print_string str2; print_string inputStr2; print_string newline
  li $v0, 4
  la $a0, str2
  syscall
  li $v0, 4
  la $a0, inputStr2
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  # $v0 = compareStrings(inputStr3, inputStr2)
  la $a0, inputStr3
  la $a1, inputStr2
  jal compareStrings

  # $s0 = $v0
  move $s0, $v0

  # print_string str3; print_int $v0; print_string newline
  li $v0, 4
  la $a0, str3
  syscall
  li $v0, 1
  move $a0, $s0
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  ### Example 6

  # print_string str1; print_string inputStr2; print_string newline
  li $v0, 4
  la $a0, str1
  syscall
  li $v0, 4
  la $a0, inputStr2
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  # print_string str2; print_string inputStr4; print_string newline
  li $v0, 4
  la $a0, str2
  syscall
  li $v0, 4
  la $a0, inputStr4
  syscall
  li $v0, 4
  la $a0, newline
  syscall

  # $v0 = compareStrings(inputStr2, inputStr4)
  la $a0, inputStr2
  la $a1, inputStr4
  jal compareStrings

  # $s0 = $v0
  move $s0, $v0

  # print_string str3; print_int $v0; print_string newline
  li $v0, 4
  la $a0, str3
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
  addi $sp, $sp, 8

  # return;
  jr $ra

################################################################################

