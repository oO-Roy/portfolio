#include <assert.h>

#include "bit_array.h"

#define CHAR_MAX_NUMS 256

#define SIZE_T_BITS_SIZE 64
#define INT_BITS_SIZE 32
#define SHORT_BITS_SIZE 16
#define CHAR_BITS_SIZE 8
#define NIBBLE_BITS_SIZE 4
#define HALF_NIBBLE_BITS_SIZE 2

int LUTMirror[CHAR_MAX_NUMS];
int LUTCountOn[CHAR_MAX_NUMS];

static unsigned char ByteMirror(unsigned char byte);
static void InitiateMirrorLUT();
static void InitiateCountOnLUT();

bit_arr_t SetAll(bit_arr_t bit_arr)		
{
	assert(CHAR_BITS_SIZE == sizeof(bit_arr));
	
	return (~0);
}

bit_arr_t ResetAll(bit_arr_t bit_arr)		
{
	assert(CHAR_BITS_SIZE == sizeof(bit_arr));
	
	return 0;
}

bit_arr_t SetOn(bit_arr_t bit_arr, size_t mask)
{
	assert((CHAR_BITS_SIZE == sizeof(bit_arr)));
	assert((SIZE_T_BITS_SIZE > mask));
	
	return (bit_arr | mask);
}

bit_arr_t SetOff(bit_arr_t bit_arr, size_t mask)
{									     	
	assert((CHAR_BITS_SIZE == sizeof(bit_arr)));
	assert((SIZE_T_BITS_SIZE > mask));
	
	return (bit_arr ^ mask);      
}								 
								
bit_arr_t SetBit(bit_arr_t bit_arr, size_t index, size_t value)	                                                                
{																					 						
	assert(CHAR_BITS_SIZE == sizeof(bit_arr));
	assert((1 == value) | (0 == value));
	assert(SIZE_T_BITS_SIZE > index);
	
	return (bit_arr & ~((size_t) 1 << index | value << index));
}		
																					
size_t GetVal(bit_arr_t bit_arr, size_t index)									
{
	assert(CHAR_BITS_SIZE == sizeof(bit_arr));
	assert(SIZE_T_BITS_SIZE > index);
	
	return (((1 << index) & bit_arr) >> index);											       
}

bit_arr_t Flip(bit_arr_t bit_arr, size_t index)
{
	assert(CHAR_BITS_SIZE == sizeof(bit_arr));
	assert(SIZE_T_BITS_SIZE > index);
	
	return ((1 << index) ^ bit_arr); 
}

bit_arr_t MirrorLUT(bit_arr_t bit_arr)		
{
	size_t i = 0;
	size_t tmp_bit_arr = 0;
	
	assert(CHAR_BITS_SIZE == sizeof(bit_arr));
	
	if (0 == LUTMirror[1])
    {
    	InitiateMirrorLUT();
    }
	
	tmp_bit_arr = LUTMirror[bit_arr & 0xFF];
	
	for ( ; i < CHAR_BITS_SIZE - 1 ; ++i)
	{
		bit_arr >>= CHAR_BITS_SIZE;
		tmp_bit_arr <<= CHAR_BITS_SIZE;
		
		tmp_bit_arr |= LUTMirror[bit_arr & 0xFF];
	}
	
	return tmp_bit_arr; 
}

bit_arr_t Mirror(bit_arr_t bit_arr)		
{
	assert(CHAR_BITS_SIZE == sizeof(bit_arr));
	
	bit_arr = (bit_arr & 0xFFFFFFFF00000000) >> INT_BITS_SIZE 
			| (bit_arr & 0x00000000FFFFFFFF) << INT_BITS_SIZE;  
	bit_arr = (bit_arr & 0xFFFF0000FFFF0000) >> SHORT_BITS_SIZE 	
			| (bit_arr & 0x0000FFFF0000FFFF) << SHORT_BITS_SIZE;
	bit_arr = (bit_arr & 0xFF00FF00FF00FF00) >> CHAR_BITS_SIZE 	
			| (bit_arr & 0x00FF00FF00FF00FF) << CHAR_BITS_SIZE;
	bit_arr = (bit_arr & 0xF0F0F0F0F0F0F0F0) >> NIBBLE_BITS_SIZE 	
			| (bit_arr & 0x0F0F0F0F0F0F0F0F) << NIBBLE_BITS_SIZE; 
	bit_arr = (bit_arr & 0xCCCCCCCCCCCCCCCC) >> HALF_NIBBLE_BITS_SIZE	
			| (bit_arr & 0x3333333333333333) << HALF_NIBBLE_BITS_SIZE; 

	return ((bit_arr & 0xAAAAAAAAAAAAAAAA) >> 1 	
			| (bit_arr & 0x5555555555555555) << 1); 
}

bit_arr_t RotateRight(bit_arr_t bit_arr, size_t n)
{
	assert(CHAR_BITS_SIZE == sizeof(bit_arr));
	assert(SIZE_T_BITS_SIZE > n);
	
	return (bit_arr >> n | ((((~(0xFFFFFFFFFFFFFFFF << n ))) & bit_arr) 
											<< (SIZE_T_BITS_SIZE - n)));  																			
}

bit_arr_t RotateLeft(bit_arr_t bit_arr, size_t n)
{
	assert(CHAR_BITS_SIZE == sizeof(bit_arr));
	assert(SIZE_T_BITS_SIZE > n);
	
	return (bit_arr << n | ((((~(0xFFFFFFFFFFFFFFFF >> n ))) & bit_arr) 
											>> (SIZE_T_BITS_SIZE - n)));
}

size_t CountOnLUT(bit_arr_t bit_arr)	
{
    assert(CHAR_BITS_SIZE == sizeof(bit_arr));
    
    if (0 == LUTCountOn[1])
    {
    	InitiateCountOnLUT();
    }
    
    return (LUTCountOn[bit_arr & 0xFF] 
    		+ LUTCountOn[(bit_arr >> CHAR_BITS_SIZE) & 0xFF] 
    		+ LUTCountOn[(bit_arr >> SHORT_BITS_SIZE) & 0xFF] 
    		+ LUTCountOn[(bit_arr >> (SHORT_BITS_SIZE + CHAR_BITS_SIZE)) & 0xFF] 
    		+ LUTCountOn[(bit_arr >> INT_BITS_SIZE) & 0xFF]
    		+ LUTCountOn[(bit_arr >> (INT_BITS_SIZE + CHAR_BITS_SIZE)) & 0xFF] 
    		+ LUTCountOn[(bit_arr >> (INT_BITS_SIZE + SHORT_BITS_SIZE)) & 0xFF] 
    		+ LUTCountOn[(bit_arr >> (SIZE_T_BITS_SIZE - CHAR_BITS_SIZE))]);
}

size_t CountOn(bit_arr_t bit_arr)	
{
	assert(CHAR_BITS_SIZE == sizeof(bit_arr));
	
	bit_arr = (bit_arr & 0x5555555555555555) 
			+ ((bit_arr >> 1) & 0x5555555555555555); 
    bit_arr = (bit_arr & 0x3333333333333333) 
    		+ ((bit_arr >> HALF_NIBBLE_BITS_SIZE) & 0x3333333333333333); 
    bit_arr = (bit_arr & 0x0F0F0F0F0F0F0F0F) 
    		+ ((bit_arr >> NIBBLE_BITS_SIZE) & 0x0F0F0F0F0F0F0F0F);
    bit_arr = (bit_arr & 0x00FF00FF00FF00FF) 
    		+ ((bit_arr >> CHAR_BITS_SIZE) & 0x00FF00FF00FF00FF); 
    bit_arr = (bit_arr & 0x0000FFFF0000FFFF) 
    		+ ((bit_arr >> SHORT_BITS_SIZE) & 0x0000FFFF0000FFFF); 
    
    return (bit_arr = (bit_arr & 0x00000000FFFFFFFF) 
    	+ ((bit_arr >> INT_BITS_SIZE) & 0x00000000FFFFFFFF)); 
}

size_t CountOff(bit_arr_t bit_arr)	
{
    assert(CHAR_BITS_SIZE == sizeof(bit_arr));
    
    return (SIZE_T_BITS_SIZE - CountOn(bit_arr)); 
}

char *ToString(bit_arr_t bit_arr, char *str)
{
	size_t i = 0;
	char *s_runner = (str + 64);
	
	assert(str);
	assert(CHAR_BITS_SIZE == sizeof(bit_arr));
	
	*s_runner = '\0';
	--s_runner;
	
	for ( ; i < SIZE_T_BITS_SIZE ; bit_arr >>= 1, --s_runner, ++i)
	{
		*s_runner = ((bit_arr & 1) + '0');
	}	
	
	return str;
}

static unsigned char ByteMirror(unsigned char byte) 
{	
	assert(1 == sizeof(byte));
	
	byte = (byte & 0xF0) >> NIBBLE_BITS_SIZE 	
		| (byte & 0x0F) << NIBBLE_BITS_SIZE; 
	byte = (byte & 0xCC) >> HALF_NIBBLE_BITS_SIZE	
		| (byte & 0x33) << HALF_NIBBLE_BITS_SIZE; 

	return ((byte & 0xAA) >> 1 	| (byte & 0x55) << 1);
}

static void InitiateMirrorLUT()
{
	int i = 0;
	
	LUTMirror[0] = 0;
	
	for ( ; i < CHAR_MAX_NUMS ; ++i)
	{
		LUTMirror[i] = ByteMirror((unsigned char)i);
	}
}

static void InitiateCountOnLUT()
{
	int i = 0;
	
	LUTCountOn[0] = 0;
	
	for ( ; i < CHAR_MAX_NUMS ; ++i)
	{
		LUTCountOn[i] = (i & 1)
		+ LUTCountOn[i / HALF_NIBBLE_BITS_SIZE];
	}
}
