/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * Jason Tieu 304047667

 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  int sign = (x >> 31) & 1;
  int signC = ~sign+1;
  int temp = 0; /* temp variable used later */
  int counter = 2; /* counter to increment bits */
  int copy1 = x;
  int copy2 = x;
  int tminCheck = x ^ (1 << 31);
  tminCheck = !tminCheck;
  tminCheck = ~tminCheck+1; //should be all ones if tmin
  
  x = (x & ~signC) | ((~x+1) & signC); //positive
  x = x + ~0;
  x = (x | x >> 1);
  x = (x | x >> 2);
  x = (x | x >> 4);
  x = (x | x >> 8);
  x = (x | x >> 16);
  x = (x + 1); //x is rounded up to next pwr

  /* now we do checks and increment counter so result is bit position
     of rounded up value of x */
  temp = !(x & ( 0xff | 0xff << 8));
  temp = ~temp + 1;
  counter += (temp & 16);

  temp = !(x & ( 0xff | 0xff << 16));
  temp = ~temp + 1;
  counter += (temp & 8);

  temp = 0x0f | 0x0f << 8;
  temp = temp | temp << 16;
  temp = !(x & temp);
  temp = ~temp + 1;
  counter += (temp & 4);

  temp = 0x33 | 0x33 << 8;
  temp = temp | temp << 16;
  temp = !(x & temp);
  temp = ~temp + 1;
  counter += (temp & 2);

  temp = 0x55 | 0x55 << 8;
  temp = temp | temp << 16;
  temp = !(x & temp);
  temp = ~temp + 1;
  counter += (temp & 1);

  /*if x is 0, you want to return 1*/
  copy2 = !copy2;
  copy2 = ~copy2 + 1;

  counter += ~((tminCheck)&1);

  /*add 1 to count if x is pwr of 2*/
  copy1 = copy1 & (copy1 + ~0);
  counter += !copy1;
  return (counter & ~copy2) | (copy2 & 1);
}
/* 
 * sm2tc - Convert from sign-magnitude to two's complement
 *   where the MSB is the sign bit
 *   Example: sm2tc(0x80000005) = -5.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 4
 */
int sm2tc(int x) {
  /* 0x80000000 is TMin
     want to subtract x from TMin
     if value is pos this will be a two's comp negation
     right shift of two's comp neg number will shift in one's to make a mask
     to select between x or the converted num
   */
  int sign = x >> 31;
  int TMin = 0x80 << 24;
  int neg_x = ~x + 1;
  return (~sign & x) | (((x & TMin) + neg_x) & sign);
}
/* 
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3
 */
int isNonNegative(int x) {
  /* get the sign bit shift all the way to the right 
     to get either 11...11(neg) or 00...00(nonneg)
   */
  int sign_bit = (x >> 31); //either all 1's or 0's
  return !(sign_bit ^ 0);
}

/*
 * rotateRight - Rotate x to the right by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateRight(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateRight(int x, int n) {
 
  int isNotZero = ((n | (~n + 1)) >> 31) & 1; //check to see if n is 0 or not
  int all_1_or_0 = (isNotZero << 31) >> 31; //either get 11..11 or 00..00 
  int neg_n = ~n+ 1;
 
  int shift1 = (all_1_or_0 | 32) & (~all_1_or_0); //if n = 0
  int shift2 = (all_1_or_0 & (32 + neg_n));//if n != 0
  int final_shift = shift1 | shift2;

  //for n = 0, should be 111...111
  //for n != 0, first get 1111..00 (n number of 0s) and negate to get 000..11 (n number of 1s)
  int right_mask = ~(all_1_or_0 << n);
  
  //get the bits that are to be placed on left after shift
  //right shift bits to the left (32 - n spots)
  int right_x = (x & right_mask) << (final_shift); 
  
  //if n = 0, should be 111...111
  //for n != 0, first get 1111..00 (32-n number of 0s) and negate to get 000..11 (32-n num of 1s)
  int left_mask = ~(all_1_or_0 << final_shift);

  int left_x = (x >> n) & left_mask; //gets only the vals that were shifted

  return left_x | right_x;
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
  /* got the sign and used temp to check its sign to round towards zero
   */
  int sign = x >> 31;
  int temp = sign & ((1<<n)+(~1+1));
  return (x + temp) >> n;
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  /* make a mask of all odd bits 1010..1010 (0xAAAAAAA)
     and check it with x
   */
  int odd_mask = 0xAA;
  odd_mask = (odd_mask << 8) + 0xAA;
  odd_mask = (odd_mask << 8) + 0xAA;
  odd_mask = (odd_mask << 8) + 0xAA; //shift it 3 times so it's all 1010..1010
  x = x & odd_mask;
  return !(x ^ odd_mask);
}
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  //using inclusive or to do Xor and replacing | with ~ and &
  return ~(~x & ~y) & ~(x & y);
}
/*
 * isTmin - returns 1 if x is the minimum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmin(int x) {
  //if x is Tmin, then x+x should = 0 due to overflow
  return !x ^ !(x+x);
}
