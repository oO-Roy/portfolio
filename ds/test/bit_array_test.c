#include <stdio.h>

#include "bit_array.h"
	
int main()
{	
	char buff[100] = {0};
		
	if (0xFFFFFFFFFFFFFFFF == SetAll(0))
	{
		printf("Set all: SUCCSSES!\n");
	}
	
	if (0 == ResetAll(0xFFFFFFFFFFFFFFFF))
	{
		printf("Reset all: SUCCSSES!\n");
	}								
		
	if (0xB == SetOn(0x08, 0x03)) 
	{
		printf("Set on: SUCCSSES!\n");
	}	
	
	if (0x08 == SetOff(0xB, 0x03))
	{
		printf("Set off: SUCCSSES!\n");
	}			

	if (0x8000000000000000 == SetBit(0, 63, 1)) 
	{
		printf("Set bit: SUCCSSES!\n");
	}
	
	if (1 == GetVal(0x0000000000000800, 11))
	{
		printf("Get val: SUCCSSES!\n");
	}
	
	if (0x800 == Flip(0, 11))
	{
		printf("Flip: SUCCSSES!\n");
	}	

	if (0xFFFF0000FFFF0000 ==  Mirror(0x0000FFFF0000FFFF))
	{
		printf("Mirror: SUCCSSES!\n");
	}

	if (0x80000000FFFFFFFF ==  MirrorLUT(0xFFFFFFFF00000001))
	{
		printf("MirrorLUT: SUCCSSES!\n");
	}
	
	if (0xe00000001FFFFFFF ==  RotateRight(0x00000000FFFFFFFF, 3))
	{
		printf("Rotate right: SUCCSSES!\n");
	}

	if (0x7FFFFFFF8 ==  RotateLeft(0x00000000FFFFFFFF, 3))
	{
		printf("Rotate left: SUCCSSES!\n");
	}
	
	if (24 ==  CountOn(0xA2A2A2A2A2A2A2A2))
	{
		printf("Count on: SUCCSSES!\n");
	}
	
	if (24 ==  CountOnLUT(0xA2A2A2A2A2A2A2A2))
	{
		printf("Count on LUT: SUCCSSES!\n");
	}

	if (40 ==  CountOff(0xA2A2A2A2A2A2A2A2))
	{
		printf("Count off: SUCCSSES!\n");
	}

	printf("To string of 0xAAAAAAAAAAAAAAAA: %s\n", ToString(0xAAAAAAAAAAAAAAAA, buff));
	
	return 0;
}
