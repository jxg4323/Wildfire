#define _BSD_SOURCE
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
	int windSpeed;
	char windDirection;
};

static void usage(){
	fprintf(stderr, "usage: wildfire [loop count] [size] [fireCatchProb] [density] [proporition burning] [windSpeed] [windDirection]\n" );
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
	int flag = 0;
	double percent = 0;
	double pnBurn = 0;
	int windN = 0;
	int neighborsBurn = 0;
	int neighbors = 0;
	int i = row;
	int j = col;
	
	int speed = 0;
	char direction;
	if(specs->windSpeed > 0)
		speed = specs->windSpeed;
	if(specs->windDirection != '\0')
		direction = specs->windDirection;
	if(i+speed > size-1)
		speed-=1;


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

	if(forest[i-1][j-1] == '*' && i-1 > 0 && j-1 > 0 ){
		neighborsBurn+=1;
	}else if(forest[i][j-1] == '*' && j-1 > 0){//wind coming from the west
		if(direction == 'W'){
			for(int k = 0; k < speed && (j-speed) > size -1;k++){
				if(forest[i][j-speed] == '*'){
					neighborsBurn+=1;
					windN += 1;
				}
			}
			flag = 1;
		}
		neighborsBurn+=1;
	}else if(forest[i+1][j-1] == '*' && i+1 < size-1 && j-1 > 0){
		neighborsBurn+=1;
	}else if(forest[i-1][j] == '*' && i-1 > 0){//wind coming from the north
		if(direction == 'N'){
			for(int k = 0; k < speed && (i-speed) > size -1;k++){
				if(forest[i][j-speed] == '*'){
					neighborsBurn+=1;
					windN += 1;
				}
			}
			flag = 1;
		}
		neighborsBurn+=1;
	}else if(forest[i-1][j+1] == '*' && i-1 > 0 && j+1 < size-1){
		neighborsBurn+=1;
	}else if(forest[i][j+1] == '*' && j+1 < size-1){//wind coming from the south
		if(direction == 'S'){
			for(int k = 0; k < speed && (j+speed) > size -1;k++){
				if(forest[i][j-speed] == '*'){
					neighborsBurn+=1;
					windN += 1;
				}
			}
			flag = 1;
		}
		neighborsBurn+=1;
	}else if(forest[i+1][j+1] == '*' && i+1 < size-1  && j+1 < size-1){
		neighborsBurn+=1;
	}else if(forest[i+1][j] == '*' && i+1 < size-1){//wind coming from the east
		if(direction == 'E'){
			for(int k = 0; k < speed && (j-speed) > size -1;k++){
				if(forest[i][j-speed] == '*'){
					neighborsBurn+=1;
					windN += 1;
				}
			}
			flag = 1;
		}
		neighborsBurn+=1;
	}
	
	if(flag == 1 && (specs->windSpeed) > 0){
		pnBurn = (neighbors+neighborsBurn > 0) ? (double)neighborsBurn/(neighbors+neighborsBurn) : 0;
		pnBurn += (pnBurn*(windN/(specs->windSpeed)) );
	}else{
		pnBurn = (neighbors+neighborsBurn > 0) ? (double)neighborsBurn/(neighbors+neighborsBurn) : 0;
	}
	
	if(pnBurn > 0.25 || forest[row][col] == '*'){
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
						forest[i][j] = '*';
						break;
					case '*':
						forest[i][j] = '_';
						break;
				}
			}
		}
	}
	printSeq(specs,size,forest);
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
	//randomly make certain trees burn
	for(int i = 0;i < specs->pBurn;i++){
		row = rand();
		row %= size;
		col = rand();
		col %= size;
		if(forest[row][col] == 'Y'){				
			forest[row][col] = '*';
		}else{
			i--;
		}
	}
	//all non-tree,and non-burning tree cells are empty
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			if(forest[i][j] != 'Y' && forest[i][j] != '*')
				forest[i][j] = ' ';
		}
	}
	//print generation 0
	printSeq(specs,size,forest);
}

void commandArgs(struct trees *forest, int argc,char** argv){
	forest->count = 0;
	forest->windSpeed = 0;
	forest->windDirection = '\0';
	
	switch(argc){
		case 1:
			if(argv[1] == "help")
				usage();
			break;
		case 5://basic no loop count
			forest->size = atoi(argv[1]);
			forest->pFire = atoi(argv[2]);
			forest->treeDensity = atoi(argv[3]);
			forest->pBurn = atoi(argv[4]);
			break;
		case 6://loop count set
			forest->count = atoi(argv[1]);
			forest->size = atoi(argv[2]);
			forest->pFire = atoi(argv[3]);
			forest->treeDensity = atoi(argv[4]);
			forest->pBurn = atoi(argv[5]);
			break;
		case 7://wind direction but no count
			forest->size = atoi(argv[1]);
			forest->pFire = atoi(argv[2]);
			forest->treeDensity = atoi(argv[3]);
			forest->pBurn = atoi(argv[4]);
			forest->windSpeed = atoi(argv[5]);
			forest->windDirection = *argv[6];
			break;
		case 8://wind direction with count
			forest->count = atoi(argv[1]);
			forest->size = atoi(argv[2]);
			forest->pFire = atoi(argv[3]);
			forest->treeDensity = atoi(argv[4]);
			forest->pBurn = atoi(argv[5]);
			forest->windSpeed = atoi(argv[6]);
			forest->windDirection = *argv[7];
			break;
		default:
			usage();
			break;
	}
	
	if(forest->count < 0){
		usage();
		printf("-pN was negative\n");
	}
	if(forest->size < 5 || forest->size > 40){
		usage();
		printf("The size (X) must be an integer in [5...40].\n");
	}
	if(forest->pFire < 0 || forest->pFire > 100){
		usage(); 
		printf("The probability (X) must be an integer in [5...40].\n");
	}
	if(forest->treeDensity < 0 || forest->treeDensity > 100){
		usage();
		printf("The density (X) must be an integer in [5...40].\n");
	}
	if(forest->pBurn < 0 || forest->pBurn > 100){
		usage();
		printf("The proportion (X) must be an integer in [5...40].\n");
	}
	if(forest->windSpeed < 0 || forest->windSpeed > 3){
		usage();
		printf("The windSpeed (X) must be an integer in [0..3].\n");
	}
}

int checkForFires(int size,char forest[][size]){
	int check = -1;
	for(int i = 0; i < size; i++){
		for(int j =0; j < size;j++){
			if(forest[i][j] == '*')
				check = 0;
		}
	}
	return check;
}

void simulation(struct trees *specs,int size,char forest[][size]){
	if(specs->count == 0){
		//run continuosly until all fires are out
		int num = 0;
		clear();
		printf("\ngeneration: %d\n",num);	
		while(checkForFires(size,forest) == 0){
			applySpread(specs,size,forest);
			printf("\ngeneration: %d\n",num+=1);
			usleep(81000);
		}
	}else{
		int count = 0;
		printf("\ngeneration: %d\n",count);
		while(count < specs->count && checkForFires(size,forest) == 0){
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
	simulation(&forestSpecs,row,forest);
}
