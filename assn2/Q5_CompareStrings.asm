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

  # Set index register $t0 to 0 (loop index)
  move $t0, $zero       # 루프 횟수 카운팅
  move $t3, $zero       # 1st string 길이
  move $t4, $zero       # 2nd string 길이


loop:
  # Load byte from each string
  lb $t1, 0($a0)  # Load byte from address in $a0
  lb $t2, 0($a1)  # Load byte from address in $a1

  beq $t1, $zero, string1Ended
  beq $t2, $zero, first_string_greater

    # Increment string pointers for next iteration
    addi $a0, $a0, 1
    addi $a1, $a1, 1

    # Increment index register $t0
    addi $t0, $t0, 1

      # Continue loop
      j loop

 string1Ended:
   bne $t2, $zero, second_string_greater
   j sameLength

 sameLength:
  move $t0, $zero
  bne $t1, $t2, characters_differ
  beq $t1, $zero, strings_equal

first_string_greater:
  li $v0, 1
  j end_compare

characters_differ:
  # Determine which string is greater
  slt $t3, $t1, $t2  # If $t1 < $t2, $t3 = 1
  bne $t3, $zero, second_string_greater
  li $v0, 1           # $v0 = 1 if first string is greater
  j end_compare

second_string_greater:
  li $v0, -1          # $v0 = -1 if second string is greater
  j end_compare

strings_equal:
  li $v0, 0           # Strings are equal
  j end_compare

end_compare:

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

