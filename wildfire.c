#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "display.h"

struct trees{
	int count;
	int size;
	int treeDensity;
	int pFire;
	int pBurn;
};

static void usage(){
	fprintf( stderr, "usage: wildfire [loop count] [size] [fireCatchProb] [density] [proporition burning] \n" );
	exit(-1);
}

void printSeq(struct trees *specs,int size,char forest[][size]){
	if(specs->count > 0){	
		for(int i = 0; i < size; i++){
			for(int j =0; j < size;j++)
				printf("%c",forest[i][j]);
			printf("\n");
		}
	}else{
		for(int i = 0; i < size; i++){
			for(int j =0; j < size;j++){
				set_cur_pos(i,j);
				put(forest[i][j]);
			}
			put(' ');
		}
	}
}

int spread(struct trees *specs,int row,int col,int size,char forest[][size]){
	
	int result = -1;
	double percent = 0;
	int neighborsBurn = 0;
	int neighbors = 0;
	int i = row;
	int j = col;

	if(forest[i-1][j-1] == 'Y' && i-1 > 0 && j-1 > 0){
		neighbors+=1;
	}else if(forest[i][j-1] == 'Y' && j-1 > 0){
		neighbors+=1;
	}else if(forest[i+1][j-1] == 'Y' && i+1 < size-1 && j-1 > 0){
		neighbors+=1;
	}else if(forest[i-1][j] == 'Y' && i-1 > 0){
		neighbors+=1;
	}else if(forest[i-1][j+1] == 'Y' && i-1 > 0 && j+1 < size-1){
		neighbors+=1;
	}else if(forest[i][j+1] == 'Y' && j+1 < size-1){
		neighbors+=1;
	}else if(forest[i+1][j+1] == 'Y' && i+1 < size-1 && j+1 < size-1){
		neighbors+=1;
	}else if(forest[i+1][j] == 'Y' && i+1 < size-1){
		neighbors+=1;
	}

	if(forest[i-1][j-1] == '.' && i-1 > 0 && j-1 > 0){
		neighborsBurn+=1;
	}else if(forest[i][j-1] == '.' && j-1 > 0){
		neighborsBurn+=1;
	}else if(forest[i+1][j-1] == '.' && i+1 < size-1 && j-1 > 0){
		neighborsBurn+=1;
	}else if(forest[i-1][j] == '.' && i-1 > 0){
		neighborsBurn+=1;
	}else if(forest[i-1][j+1] == '.' && i-1 > 0 && j+1 < size-1){
		neighborsBurn+=1;
	}else if(forest[i][j+1] == '.' && j+1 < size-1){
		neighborsBurn+=1;
	}else if(forest[i+1][j+1] == '.' && i+1 < size-1  && j+1 < size-1){
		neighborsBurn+=1;
	}else if(forest[i+1][j] == '.' && i+1 < size-1){
		neighborsBurn+=1;
	}
	
	double pnBurn = (neighbors > 0) ? (double)neighborsBurn/(neighbors+neighborsBurn) : 0;

	if(pnBurn > 0.25 || forest[row][col] == '.'){
		percent = (double)rand()/RAND_MAX;
		if(percent < ((double)specs->pFire/100)){
			result = 0;		
		}
	}

	return result;
}

void applySpread(struct trees *specs,int size,char forest[][size]){
	char forestCopy[size][size];
	memcpy(forestCopy,forest,sizeof(char)*size*size);

	for(int i = 0; i < size;i++){
		for(int j = 0; j < size;j++){
			if(spread(specs,i,j,size,forestCopy) == 0){
				switch(forest[i][j]){
					case 'Y':
						forest[i][j] = '.';
						break;
					case '.':
						forest[i][j] = ' ';
						break;
				}
			}
		}
	}
	printSeq(specs,size,forest);
	/*if(specs->count > 0){	
		for(int i = 0; i < size; i++){
			for(int j =0; j < size;j++)
				printf("%c",forest[i][j]);
			printf("\n");
		}
	}else{
		for(int i = 0; i < size; i++){
			for(int j =0; j < size;j++){
				set_cur_pos(i,j);
				put(forest[i][j]);
			}
			put(' ');
		}
	}*/
}

void randomFill(struct trees *specs,int size,char forest[][size]){
	int row = 0;
	int col = 0;
	int td = specs->treeDensity;
	int temp  = (size*size)*td;
	td = temp/100;
	
	srand(time(NULL));	
	//fill forest with proper number of trees
	for(int i = 0;i < td;i++){
		row = rand();
		row %= size;
		col = rand();
		col %= size;
		if(forest[row][col] == 'Y'){
			--i;
		}else{
			forest[row][col] = 'Y';
		}
	}
	double t = (double)specs->pBurn/100;
	specs->pBurn = t * td;
	srand(time(NULL));
	for(int i = 0;i < specs->pBurn;i++){
		row = rand();
		row %= size;
		col = rand();
		col %= size;
		if(forest[row][col] == 'Y'){				
			forest[row][col] = '.';
		}else{
			i--;
		}
	}

	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			if(forest[i][j] != 'Y' && forest[i][j] != '.')
				forest[i][j] = ' ';
		}
	}
	printSeq(specs,size,forest);
}

void commandArgs(struct trees *forest, int argc,char** argv){
	forest->count = 0;
	if(argc == 5){
		forest->size = atoi(argv[1]);
		forest->pFire = atoi(argv[2]);
		forest->treeDensity = atoi(argv[3]);
		forest->pBurn = atoi(argv[4]);
		forest->count = 0;
	}else if(argc == 6){
		forest->count = atoi(argv[1]);
		forest->size = atoi(argv[2]);
		forest->pFire = atoi(argv[3]);
		forest->treeDensity = atoi(argv[4]);
		forest->pBurn = atoi(argv[5]);
	}else{
		usage();
	}
	if(forest->count < 0)
		usage();
	if(forest->size < 5 || forest->size > 40)
		usage();
	if(forest->pFire < 0 || forest->pFire > 100)
		usage(); 
	if(forest->treeDensity < 0 || forest->treeDensity > 100)
		usage();
	if(forest->pBurn < 0 || forest->pBurn > 100)
		usage();
}

int checkForFires(int size,char forest[][size]){
	int check = -1;
	for(int i = 0; i < size; i++){
		for(int j =0; j < size;j++){
			if(forest[i][j] == '.')
				check = 0;
		}
	}
	return check;
}

void simulation(struct trees *specs,int size,char forest[][size]){
	if(specs->count == 0){
		//run continuosly until all fires are out
		int num = 0;
		while(checkForFires(size,forest) == 0){
			clear();
			applySpread(specs,size,forest);
			printf("\ngeneration: %d\n",num+=1);
		}
	}else{
		int count = 0;
		while(count < specs->count){
			applySpread(specs,size,forest);
			count+=1;
			printf("generation: %d\n",count);
		}
	}
}

int main(int argc,char** argv){
	int count = 0;
	int row,col;
	struct trees forestSpecs;
	commandArgs(&forestSpecs,argc,argv);
	row = forestSpecs.size;
	col = forestSpecs.size;	
	char forest[row][col];
	//printf("td: %d\n",forestSpecs.treeDensity);
	randomFill(&forestSpecs,row,forest);
	printf("generation: 0\n");	
	simulation(&forestSpecs,row,forest);
}
