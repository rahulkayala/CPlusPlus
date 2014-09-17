#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


//Cache (15,4,2)

//Stream of Address to test
unsigned short addrStream[] = {0x95e3,0x95e6,0xa63f,0x56ec,0x36ba,0xa63a,0xcae7,0x56e0,0x2aba,0x7536,0xd5e8,0xcab6,0xcaef,0xa63f,0xeaba,0xe636,0x2aee,0x0abc,0xd5e2,0x56b6,0x9537,0x35ea,0x36bc,0x95e9,0x7536,0xcab7,0xb5e5,0xd5e2,0xeabb,0x15e6,0x2ab3,0xcae7,0xd5ee,0x15ef,0x8ab5,0x35ea,0x95e6,0x2aba,0xb6e4,0x56ec,0xcabf,0xa63a}; 

//Index Storing The Tag Accessed in Cache
struct index
{
	unsigned char tag[4]; // 4 Blocks in a set
	int written[4]; //Compulsory Miss for each block in a set
	int timeStamp[4]; //Counts the one which was used, lowest count gets kicked out	
};

// Total Number of Indices possible are 2^(C-B-S) = 2^9
struct index indice[512];

//System Time Stamp
int systemTimeStamp = 1;

char* Retr(unsigned short address)
{
	//Returns H for Hit, C for Compulsary Miss, O for Other Miss
	unsigned short tag, index;
	int blk, minTime, minBlock = 0;

	tag = (address & 0xe000) >> 13; // Retrieving MSB 3 bits
	index = (address & 0x1ff0) >> 4; // Retrieving Bits 4 to 12
	
	//printf("Index: %u, Tag: %u,\nLRU Reg :  %d %d %d %d\nLRU Time: %d %d %d %d\n\n",index,tag,indice[index].LRUStore[0].block,indice[index].LRUStore[1].block,indice[index].LRUStore[2].block,indice[index].LRUStore[3].block,indice[index].LRUStore[0].time,indice[index].LRUStore[1].time,indice[index].LRUStore[2].time,indice[index].LRUStore[3].time);

	for (blk=0;blk<4;blk++)
	{
		if(indice[index].written[blk] == 0) //First Time Access Only
		{
			indice[index].written[blk] = 1;
			indice[index].tag[blk] = tag;
			indice[index].timeStamp[blk] = systemTimeStamp;
			return "Miss - Compulsory";
		}
		else if(indice[index].tag[blk] == tag)
		{
			indice[index].timeStamp[blk] = systemTimeStamp;
			return "Hit";
		}
	}
	
	//Traversing for LRU Block - one with the LOWEST Time Stamp
	minTime = indice[index].timeStamp[0]; //Taking Block 0 timeStamp for initialisation
	blk = 0;
	while(blk < 4)
	{
		if(minTime > indice[index].timeStamp[blk])
		{
			minTime = indice[index].timeStamp[blk];
			minBlock = blk;
		}
		
		blk++;
	}

	// Inserting New Block into LRU position
	indice[index].timeStamp[minBlock] = systemTimeStamp; 
	indice[index].tag[minBlock] = tag;
	return "Miss - Other";
}



int main(int argc, char* argv)
{
	unsigned short tempAddress;
	char* result;
	int i, j;
	FILE *file;

	//Initialising Indices
	for(i=0; i < 512; i++)
	{
		for(j=0;j<4;j++)
		{
			indice[i].written[j] = 0;
			indice[i].timeStamp[j] = 0;
		}
	}

	//Setting up Result File
	file = fopen("Q2SolutionV.csv","w+");
	fprintf(file,"Address,Cache Response\n");
	
	//Checking for each address in the stream
	for( i=0;i < sizeof(addrStream)/sizeof(unsigned short); i++)
	{
		result = Retr(addrStream[i]);
		systemTimeStamp++;		
		fprintf(file,"%04X,%s\n",addrStream[i],result);
	}
	printf("\n");
}

