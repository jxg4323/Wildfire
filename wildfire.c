#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void error(){
	fprintf( stderr, "usage: use_getopt [-p NUM] [-s NUM] \n" );
}

int main(int argc,char** argv){
	char* forest;
	int opt = 0;
	int temp = 0;
	int row,col;
	while( (opt = getopt(argc,argv,"n::s:p:t:b:")) != -1){
		switch( opt ){
			case 'n'://fized count
				temp = (int)strtol(optarg,NULL,10);
				printf("Fixed count: %d\n",temp);
				break;
			case 's'://size of grid
				row = (int)strtol(optarg,NULL,10);
				col = row;
				printf("Size: %d\n",row);
				break;
			case 'p'://probability of a tree catching fire
				temp = (int)strtol(optarg,NULL,10);
				printf("Probability: %d\n",temp);
				break;
			case 't'://tree density
				temp = (int)strtol(optarg,NULL,10);
				printf("Tree Density: %d\n",temp);
				break;
			case 'b'://proportion burning
				temp = (int)strtol(optarg,NULL,10);
				printf("Proportion Burning: %d\n",temp);
				break;
			default://error when collecting command line args
				error();
				break;
		}
	}
	forest = (char *)malloc(row*col*sizeof(char));
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			*(forest + i*j + j) = 'Y';
			printf("%c",*(forest + i*j + j));
		}
		printf("\n");
	}
}
