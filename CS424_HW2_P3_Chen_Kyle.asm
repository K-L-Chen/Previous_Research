# CSCI 424, Homework 2

# switch to the Data segment
.data
# global data is defined here
# Don't forget the backslash-n (newline character)
.align 2

Homework:
	.asciiz "CSCI 424 Homework 2\n"
Name_1:
	.asciiz "Kyle\n"
Name_2:
	.asciiz "Chen\n"

#Strings for if the input string is/is not a palindrome
Input_Str:
	.asciiz "Enter string (max 10 characters)\n"

Statement:
	.asciiz "The string "
	
Is_Palindrome:
	.asciiz "is a Palindrome\n"

Not_Palindrome:
	.asciiz "is not a Palindrome\n"
	
#Malloc space for 10 chars
input_size:
	.space 11

#new line
nl:
	.asciiz "\n"
#space
sp:
	.asciiz " "
	
# switch to the Text segment
.text
# the program is defined here
.globl main

main:
# Whose program is this?
	la $a0, Homework
	jal Print_string
	la $a0, Name_1
	jal Print_string
	la $a0, Name_2
	jal Print_string

#STRING INPUT
	la $a0, Input_Str
	jal Print_string	#Ask for input
	
	#la $t0, input_size
	addi $v0, $zero, 8 	#system call code for read_str
	la $a0, input_size	#initialize size for input
	li $a1, 12
	syscall			#grab input values
	
	jal Print_string	#Check if string is input correctly
	
	
	
#PALINDROME FUNCTION
	move $s1, $a0		#get address of input
	
	move $t0, $v0
	
	jal GetLenStr
	
	move $s2, $v0		#get input size
	move $v0, $t0		#set $v0 back to original value
	move $t0, $a0		#retain $a0
	move $a0, $s2

######################DEBUG#############################
	#add $s2, $s2, $s1	#end of max possible string
	#jal Print_integer
	#la $a0, nl # newline
	#jal Print_string
######################DEBUG#############################

	move $a0, $t0
	
	subi $s2, $s2, 1	#get proper value for length
	
	move $t0, $a0
	move $s0, $t0

####################DEBUG###########################
	#add $t1, $s1, $s2
	#move $t1, $s2
	#move $a0, $t1
	#jal Print_integer
	#la $a0, nl # newline
	#jal Print_string
####################DEBUG###########################
	move $a0, $t0
	add $a0, $a0, $t1
	#jal Print_string		DEBUG
	move $a0, $t0

	addi $t5, $t1, 0 #setup pointer 2

Loop:
	#li $t4, 0
	beq $t4, $t5, ExitLoop_True
	
	addu $t6, $s0, $t4		#$t6 = address of string ($s0) + pointer 1
	lb $s3, 0($t6)			#$s3 = value at $t6
	addu $t7, $s0, $t5		#$t7 = address of string ($s0) + pointer 2
	lb $s4, 0($t7)			#$s4 = value at $t7
	
############################DEBUG#######################################
	#move $a0, $t6
	#jal Print_string
	#jal Print_integer
	#move $a0, $t7
	#jal Print_string
############################DEBUG#######################################
	
	bne $s3, $s4, ExitLoop_False	#if $s3 != $s4, goto False
	
	move $t0, $t4
	addiu $t4, $t4, 1		#increment pointer 1, decriment pointer 2
	subiu $t5, $t5, 1
	beq $t0, $t5, ExitLoop_True	#if pointer cross each other,
					#then there exists no different values,
					#and we are done
	
	j Loop
	
ExitLoop_True:				#PRINT THE WHOLE STATEMENT
	la $a0, Statement
	jal Print_string
	move $a0, $s0
	jal Print_string
	la $a0, sp
	jal Print_string
	la $a0, Is_Palindrome
	jal Print_string
	jal Exit

ExitLoop_False:				#PRINT THE WHOLE STATEMENT
	la $a0, Statement
	jal Print_string
	move $a0, $s0
	jal Print_string
	la $a0, sp
	jal Print_string
	la $a0, Not_Palindrome
	jal Print_string
	jal Exit
	
GetLenStr:
	li	$t0, 0
	li	$t2, 0
	
LenStr_Loop:				#Loop until null character found
	add	$t2, $a0, $t0
	lb	$t1, 0($t2)		#save value at $t2 in $t1
	beqz	$t1, LenStr_ExitLoop	#check if value is NULL ASCII character
	addiu	$t0, $t0, 1
	j	LenStr_Loop
		
LenStr_ExitLoop:
	subi	$t0, $t0, 1
	add	$v0, $zero, $t0
	add	$t0, $zero, $zero	#reset $t0
	jr	$ra
		
.globl Print_integer
Print_integer: 				# print the integer in register a0
	addi $v0, $zero, 1
	syscall
	jr $ra
	
.globl Print_string
Print_string: 				# print the string whose starting address is in register a0
	addi $v0, $zero, 4
	syscall
	jr $ra
	
.globl Exit
Exit: 					# end the program, no explicit return status
	addi $v0, $zero, 10
	syscall
	jr $ra