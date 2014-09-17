#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

//Cache (13,4,0)

//Stream of Address to test
unsigned short addrStream[] = {0x95e3,0x95e6,0xa63f,0x56ec,0x36ba,0xa63a,0xcae7,0x56e0,0x2aba,0x7536,0xd5e8,0xcab6,0xcaef,0xa63f,0xeaba,0xe636,0x2aee,0x0abc,0xd5e2,0x56b6,0x9537,0x35ea,0x36bc,0x95e9,0x7536,0xcab7,0xb5e5,0xd5e2,0xeabb,0x15e6,0x2ab3,0xcae7,0xd5ee,0x15ef,0x8ab5,0x35ea,0x95e6,0x2aba,0xb6e4,0x56ec,0xcabf,0xa63a}; 


//Index Storing The Tag Accessed in Cache
struct index{
	unsigned char tag;
	int written;
};

// Total Number of Indices possible are 2^(C-B-S) = 2^9
struct index indice[512];

char* Retr(unsigned short address)
{
	//Returns H for Hit, C for Compulsary Miss, O for Other Miss
	unsigned short tag, index;
	
	tag = (address & 0xe000) >> 13; // Retrieving MSB 3 bits
	index = (address & 0x1ff0) >> 4; // Retrieving Bits 4 to 12
	
	if(indice[index].written == 0) //First Time Access Only
	{
		indice[index].written = 1;
		indice[index].tag = tag;
		return "Miss - Compulsory";
	}
	else if(indice[index].tag == tag)
	{
		return "Hit";
	}
	else
	{
		indice[index].tag = tag;
		return "Miss - Other";
	}

}

int main(int argc, char* argv)
{
	unsigned short tempAddress;
	char* result;
	int i = 0;
	FILE *file;

	//Initialising Indices
	while(i < 512)
	{
		indice[i++].written = 0;
	}
	
	//Setting up Result File
	file = fopen("Q1Solution.csv","w+");
	fprintf(file,"Address,Cache Response\n");
	//Checking for each address in the stream
	for( i=0;i < sizeof(addrStream)/sizeof(unsigned short); i++)
	{
		result = Retr(addrStream[i]);		
		fprintf(file,"%04X,%s\n",addrStream[i],result);
	}
	printf("\n");
}

