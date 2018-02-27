#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct trees{
	int count;
	int size;
	int treeDensity;
	int pFire;
	int pBurn;
};

static void usage(){
	fprintf( stderr, "usage: wildfire [-p NUM] [-s NUM] \n" );
	exit(-1);
}

void print(int size,char forest[][size]){
	for(int i = 0; i < size; i++){
		for(int j =0; j < size;j++)
			printf("%c",forest[i][j]);
		printf("\n");
	}
}

void spread(struct trees *specs,int row,int col,int size,char forest[][size]){
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
	//printf("fores[i][j]: %c\n",forest[row][col]);
	//printf("neighbors: %d, neighborsBurn: %d, pnBurn: %f\n",neighbors,neighborsBurn,pnBurn);
	if(pnBurn > 0.25){
		srand(time(NULL));
		percent = (double)rand()/RAND_MAX;
		printf("percent: %f\n",percent);
		if(percent < ((double)specs->pFire/100) && forest[row][col] == 'Y'){
				forest[row][col] = '.';
		}
	}
	/*printf("----------------spread function--------------\n");
	for(int i = 0; i < size; i++){
		for(int j =0; j < size;j++)
			printf("%c",forest[i][j]);
		printf("\n");
	}
	printf("------------------------------\n");*/
}

void applySpread(struct trees *specs,int size,char forest[][size]){
	for(int i = 0; i < size;i++){
		for(int j = 0; j < size;j++){
			if(forest[i][j] == 'Y'){
				spread(specs,i,j,size,forest);
			}
		}
	}
	printf("----------------algorithm spread function--------------\n");
	for(int i = 0; i < size; i++){
		for(int j =0; j < size;j++)
			printf("%c",forest[i][j]);
		printf("\n");
	}
	printf("------------------------------\n");
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
			printf("%c",forest[i][j]);
		}
		printf("\n");
	}
}

void commandArgs(struct trees *forest, int argc,char** argv){
	forest->count = 0;
	if(argc == 5){
		forest->size = atoi(argv[1]);
		forest->pFire = atoi(argv[2]);
		forest->treeDensity = atoi(argv[3]);
		forest->pBurn = atoi(argv[4]);
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

int main(int argc,char** argv){
	int temp = 0;
	int flag = 0;
	int row,col,count,size,pFire,pBurn,td;
	struct trees forestSpecs;
	commandArgs(&forestSpecs,argc,argv);
	row = forestSpecs.size;
	col = forestSpecs.size;	
	char forest[row][col];
	printf("td: %d\n",forestSpecs.treeDensity);
	randomFill(&forestSpecs,row,forest);
	printf("generation 0\n");	
	while(temp < 1){
		applySpread(&forestSpecs,row,forest);
		temp++;
		printf("generation %d\n",temp+1);
	}		
}
