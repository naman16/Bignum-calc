/* Bignum Calculator */

/* Given 2 numbers between base 2 and 36, this calculator can perform basic arithmetic and
 logical operators on these numbers.
 It also checks for valid inputs and gives an error pointing out what the mistake is if the input
 is not valid.
 If arithmetic operation is to be performed on 2 numbers with opposite signs, call Subtraction. 
 Arithmetic Operations - Addition, Subtraction
 Logical Operations - Greater, Less, Equal
 */

/* Credits - Andy Exley and Emery Mizero for providing the skeletal framework for this code
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignum_math.h"


/*
 * Returns true if the given char is a digit from 0 to 9
 */
bool is_digit(char c) {
	return c >= '0' && c <= '9';
}

/*
 * Returns true if lower alphabetic character
 */
bool is_lower_alphabetic(char c) {
	return c >= 'a' && c <= 'z';
}

/*
 * Returns true if upper alphabetic character
 */
bool is_upper_alphabetic(char c) {
	return c >= 'A' && c <= 'Z';
}

/*
 * Convert a string to an integer
 * returns 0 if it cannot be converted.
 */
int string_to_integer(char* input) {
	int result = 0;
	int length = strlen(input);
    int num_digits = length;
	int sign = 1;

	int i = 0;
    int factor = 1;

    if (input[0] == '-') {
		num_digits--;
		sign = -1;
    }

	for (i = 0; i < num_digits; i++, length--) {
		if (!is_digit(input[length-1])) {
			return 0;
		}
		if (i > 0) factor*=10;
		result += (input[length-1] - '0') * factor;
	}

    return sign * result;
}

/*
 * Returns true if the given base is valid.
 * that is: integers between 2 and 36
 */
bool valid_base(int base) {
	if(!(base >= 2 && base <= 36)) {
		return false;
	}
	return true;
}
/*
 * converts from an array of characters (string) to an array of integers
 */
/* Adding -1 at the end of the array to denote that the entire character array has been
   iterated over and has been changed to integers. Also the -1 at the end is useful for finding
   the length of the number.
 */

int* string_to_integer_array(char* str) {
	int* result;
	int i, str_offset = 0;
		result = malloc((strlen(str) + 1) * sizeof(int));
		result[strlen(str)] = -1;
	for(i = str_offset; str[i] != '\0'; i++)
    {
		if(is_digit(str[i]))
        {
			result[i - str_offset] = str[i] - '0';//converting a digit into it's ASCII
		}
        else if (is_lower_alphabetic(str[i]))
        {
			result[i - str_offset] = str[i] - 'a' + 10;//getting the value of the character in the number base
		}
        else if (is_upper_alphabetic(str[i]))
        {
			result[i - str_offset] = str[i] - 'A' + 10;//getting the value of the character in the number base
		}
    }
	return result;
}

/*
 * finds the length of a bignum...
 * simply traversing the bignum until a negative number is found.
 */
int bignum_length(int* num) {
	int len = 0;
	while(num[len] >= 0) { len++; }
	return len;
}

/*
 *
 * Returns true if the given string (char array) is a valid input,
 * that is: digits 0-9, letters A-Z, a-z
 * and it should not violate the given base and shouldn't be negative
 */
bool valid_input(char* input, int base)
{

int i=0;
i=0;
int len = strlen(input);
for (i=0;i<len;i++)
{
	if (!(is_digit(input[i]) ||is_lower_alphabetic(input[i])||is_upper_alphabetic(input[i])))
	{
		return false;
	}
}
if(input[0]== '-')
{
	return false;
}
int* intarray;
intarray= string_to_integer_array(input);
int j=0;
	for (j=0;j<bignum_length(intarray);j++)
	{
		if(intarray[j]>=base)
		{
			return false;
		}
	}
	return true;
}
// Removing leading zeroes because 0's before any number is redundant.

int* removingleadingzeroes(int* input1){
    int i=0;
    while (i<((bignum_length(input1))-1) && input1[i]==0){
        i=i+1;
    }
    int diff=bignum_length(input1)-i+1;
    int* output = (int*) malloc (sizeof(int) * diff);//creating an array to copy the numbers, leaving the leading zeroes.
    output[diff-1]=-1;
    int j=0;
    int index=0;
    for(j=i;j<bignum_length(input1);j++){
        output[index]=input1[j];
        index=index+1;
    }
    return output;
    
}

/*
  Prints out a bignum using digits and lower-case characters. For uniformity, incase the result has alphabets, printing it in
  lowercase.
 */
void bignum_print(int* num) {
	int i;
	if(num == NULL)
	{
		return;
	}
  char alphabet[]= {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
	if(num[bignum_length(num)]==-2){
		printf("%s","-");
	}
    int* result = (int*) malloc (sizeof(int) * bignum_length(num));
    result = removingleadingzeroes(num);
	int j=0;
    i=bignum_length(result);
	for(j =0;j<i;j++)
	{
		if(result[j]>=10)
		{
			char x=alphabet[result[j]-10];
			printf("%c",x);
		}
		else
		{
			printf("%i",result[j]);
		}
	}
	printf("\n");
}

/*
 *	Helper for reversing the result that we built backward.
 *  see add(...) below
 */
void reverse(int* num) {
	int i, len = bignum_length(num);
	for(i = 0; i < len/2; i++) {
		int temp = num[i];
		num[i] = num[len-i-1];
		num[len-i-1] = temp;
	}
}


/*
 * Used to add two numbers with the same sign.
 *
 */
int* add(int* input1, int* input2, int base) {
	int len1 = bignum_length(input1);
	int len2 = bignum_length(input2);
	int resultlength = ((len1 > len2)? len1 : len2) + 2;
	int* result = (int*) malloc (sizeof(int) * resultlength);
	int r = 0;
	int carry = 0;
	int sign = input1[len1];
    int num1, num2;

	len1--;
	len2--;

	while (len1 >= 0 || len2 >= 0) {
        if (len1 >= 0) {
            num1 = input1[len1];
        } else {
            num1 = 0;
        }

        if (len2 >= 0) {
            num2 = input2[len2];
        } else {
            num2 = 0;
        }
		result[r] = (num1 + num2 + carry) % base;
		carry = (num1 + num2 + carry) / base;
		len1--;
		len2--;
		r++;
	}
	if (carry > 0) {
        result[r] = carry;
        r++;
    }
	result[r] = sign;
	reverse(result);
	return result;
}
// Checking for equality in this function
int checkequals(int* input1, int* input2){
	if(bignum_length(input1)!=bignum_length(input2)){
	return 0;
	}
	int i=0;
	for(i=0;i<bignum_length(input1);i++){
			if(input1[i]!=input2[i]){
				return 0;
			}
		}
	return 1;
}

// Comparing the two numbers in this function

int comparison(int* input1, int* input2){
	if (bignum_length(input1)>bignum_length(input2)){
		return 1;
    }
	else if (bignum_length(input1)<bignum_length(input2)){
		return 0;
	}
	else{
		int i=0;
		int diff=0;
		while(i<bignum_length(input1)){
			 diff = input1[i]-input2[i];
			 if(diff>0){
				 return 1;
			 }
			 else if (diff<0){
				 return 0;
			 }
			 else{
				i=i+1;
			}
			}
			return 0;
		}
	}

// Doing the main subtraction in this function.
int* subtraction(int* input1, int* input2, int base){
	int len1 = bignum_length(input1);
	int len2 = bignum_length(input2);
	int resultlength = ((len1 > len2)? len1 : len2);
	int* result = (int*) malloc (sizeof(int) * resultlength);
	int num1, num2;
	len1--;
	len2--;
	int t=0;
	int s=0;
    int resultpos=resultlength-1;
	while(len1>=0){
		num1=input1[len1];
		if (len2 >=0) {
			num2=input2[len2];
		}
		else {
			num2=0;
        }
		t=num1-num2;
		if (t<0)
		{
			s=base+num1-num2;
            result[resultpos]=s;
            input1[len1-1]=input1[len1-1]-1;
		}
        else{
            result[resultpos]=t;
        }
        len1=len1-1;
        len2=len2-1;
        resultpos=resultpos-1;
	}
	return result;
}
//Function for setting up subtraction based on which number is greater
int* subtract(int* input1, int* input2, int base){
	int len1=bignum_length(input1);
	int len2=bignum_length(input2);
	int resultlength = ((len1 > len2)? len1 : len2);
	int* result = (int*) malloc (sizeof(int) * resultlength);
	int compare=comparison(input1,input2);
	if(compare==1 || (compare==0 && checkequals(input1, input2)==1)){
		result=subtraction(input1, input2, base);
		result[resultlength]=-1;
	}
	else{
		result=subtraction(input2, input1, base);
		result[resultlength]=-2;
	}
	return result;
}


/*
 * This function, based on the operation selected
 * decides which operation is to be performed and returns the result of performing that operation.
 */
int* perform_math(int* input1, int* input2, char op, int base) {

    input1 = removingleadingzeroes(input1);
    input2 = removingleadingzeroes(input2);
    int len1 = bignum_length(input1);
    int len2 = bignum_length(input2); 
	int resultlength = ((len1 > len2)? len1 : len2) + 1;
	int* result = (int*) malloc (sizeof(int) * resultlength);
    
	if(op == '+') {
		result=add(input1, input2, base);
	}
	if(op == '-') {
		result=subtract(input1, input2, base);
	}
	if(op== '>'){
      result[0]=comparison(input1, input2);
		}
	if(op=='<'){ // doing this for the less than function
		result[0]=comparison(input2, input1);
		}
	if(op=='='){
		result[0]=checkequals(input1, input2);
  }
	return result;
}

/*
 * Print to "stderr" and exit program
 */
void print_usage(char* name) {
	fprintf(stderr, "----------------------------------------------------\n");
	fprintf(stderr, "Usage: %s base input1 operation input2\n", name);
	fprintf(stderr, "base must be number between 2 and 36, inclusive\n");
	fprintf(stderr, "input1 and input2 are arbitrary-length integers\n");
	fprintf(stderr, "Two operations are allowed '+' and '-'\n");
	fprintf(stderr, "----------------------------------------------------\n");
	exit(1);
}


/*
 * Main function for this program.
 */
int main(int argc, char** argv) {

	int input_base;

    int* input1;
    int* input2;
    int* result;

	if(argc != 5) {
		print_usage(argv[0]);
	}

	input_base = string_to_integer(argv[1]);

	if(!valid_base(input_base)) {
		fprintf(stderr, "Invalid base: %s\n", argv[1]);
		print_usage(argv[0]);
	}

	if(!valid_input(argv[2], input_base)) {
		fprintf(stderr, "Invalid input1: %s\n", argv[2]);
		print_usage(argv[0]);
	}

	if(!valid_input(argv[4], input_base)) {
		fprintf(stderr, "Invalid input2: %s\n", argv[4]);
		print_usage(argv[0]);
	}

        char op = argv[3][0];
	if(op != '-' && op != '+' && op != '<' && op != '>' && op != '=') {
		fprintf(stderr, "Invalid operation: %s\n", argv[3]);
		print_usage(argv[0]);
	}

	input1 = string_to_integer_array(argv[2]);
    input2 = string_to_integer_array(argv[4]);

    result = perform_math(input1, input2, argv[3][0], input_base);
		if(op =='<' || op == '>' || op=='='){
			if(result[0]==1){
				printf("%s\n","true");
			}
			else{
				printf("%s\n","false");
			}
		}
		else{
    	printf("Result: ");
    	bignum_print(result);
	  	printf("\n");
    }
	exit(0);
}
