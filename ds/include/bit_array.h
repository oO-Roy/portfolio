#ifndef __BIT_ARRAY_H__
#define __BIT_ARRAY_H__

#include <stddef.h>

typedef size_t bit_arr_t;

/* turn all bits on */
bit_arr_t SetAll(bit_arr_t bit_arr);

/* turn all bits off */
bit_arr_t ResetAll(bit_arr_t bit_arr);

/* all the bits that are on in mask will be turned ON in bit_arr */
bit_arr_t SetOn(bit_arr_t bit_arr, size_t mask);

/* all the bits that are on in mask will be turned OFF in bit_arr  */
bit_arr_t SetOff(bit_arr_t bit_arr, size_t mask);

/* set the bit at index to value. value should be 1 or 0, 
   if any other value is sent then the behavior is undefined.
   if index is more then 64 the behavior is undefined */
bit_arr_t SetBit(bit_arr_t bit_arr, size_t index, size_t value); 

/*get the value of the bit at place index
  if index is more then 64 the behavior is undefined*/
size_t GetVal(bit_arr_t bit_arr, size_t index);

/* flip the bit at location index (we changed it to flip only 1 bit at Ori's request) */
bit_arr_t Flip(bit_arr_t bit_arr, size_t index);

/* mirrorLUT (reverse) the order of the bits */
bit_arr_t MirrorLUT(bit_arr_t bit_arr);

/* mirror (reverse) the order of the bits */
bit_arr_t Mirror(bit_arr_t bit_arr);

/* shift bits to the right n times */
bit_arr_t RotateRight(bit_arr_t bit_arr, size_t n);

/* shift bits to the left n times */
bit_arr_t RotateLeft(bit_arr_t bit_arr, size_t n);

/* count (with LUT) the number of on bits in the bit array */
size_t CountOnLUT(bit_arr_t bit_arr);

/* count the number of on bits in the bit array */
size_t CountOn(bit_arr_t bit_arr);

/* count the number of off bits in the bit array */
size_t CountOff(bit_arr_t bit_arr);

/* creates a string of length 65 (64 for the 64 bits and 1 more
for the null terminator) of 0's and 1's that are the order same as bit_arr, and returns it */
char *ToString(bit_arr_t bit_arr, char *str);


#endif /* __BIT_ARRAY_H__ */
