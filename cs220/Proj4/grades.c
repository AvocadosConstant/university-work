#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

void getGrades(int grds[11][100],int n);
void getAvg(int grds[11][100],int n);
char * getUserLine();
void printGrades(int grds[11][100],int n,int first);

int aGrades[11][100]; 
int bGrades[11][100]; 
char retBufferArea[1024+4096];
char *retBuffer;

FILE *txtFile;

int main(int argc, char ** argv) {
	char * secID;
	time_t t;

	unsigned int pagesize=getpagesize();
	setbuf(stdout,0);
	retBuffer=(unsigned char *)(((long)retBufferArea + pagesize-1) & ~(pagesize-1));
	if(mprotect(retBuffer, 1024, PROT_READ|PROT_EXEC|PROT_WRITE)) {
      printf("mprotect failed\n");
      return(1);
   }
   if (argc==1) srand((unsigned) time(&t));
   else {
   	unsigned int seed=atoi(argv[1]);
   	srand(seed);
	}
	getGrades(aGrades,52);
	getGrades(bGrades,36);
	getAvg(aGrades,52);
	getAvg(bGrades,36);

	printf("Choose section A or B: ");
	secID=getUserLine();
	printf("\n");
	

	if (secID[0]=='A') {
		printGrades(aGrades,52,100);
	} else if (secID[0]=='B') {
		printGrades(bGrades,36,200);
	} else {
		printf("Unable to determine which section you wanted.\n");
	}
	// free(retBufferArea);
	exit(0);
}

void getGrades(int grds[11][100],int n) {
	int s;
	for(s=0; s<n; s++) {
		grds[0][s]=30+rand()%61;
		grds[1][s]=70+rand()%31;
		grds[2][s]=50+rand()%51;
		grds[3][s]=rand()%101;
		grds[4][s]=rand()%10;
		grds[5][s]=rand()%10;
		grds[6][s]=rand()%10;
		grds[7][s]=rand()%10;
		grds[8][s]=30+rand()%66;
		grds[9][s]=40+rand()%56;
	}
}

void getAvg(int grds[11][100],int n) {
	int s;
	int avgProj;
	int minProj;
	int avgQuiz;
	int avgTest;
	for(s=0; s<n; s++) {
		minProj=101;
		if (grds[0][s] < minProj) minProj=grds[0][s];
		if (grds[1][s] < minProj) minProj=grds[1][s];
		if (grds[2][s] < minProj) minProj=grds[2][s];
		if (grds[3][s] < minProj) minProj=grds[3][s];
		avgProj=(grds[0][s] + grds[1][s] + grds[2][s] + grds[3][s] - minProj) / 3;
		avgQuiz=(10 *(grds[4][s] + grds[5][s] + grds[6][s] + grds[7][s])) / 4;
		avgTest=(grds[8][s] + grds[9][s])/2;
		grds[10][s] = (3*avgProj + avgQuiz + 2*avgTest) / 6;
	}
}



char * getUserLine() {
	char buffer[256];
	
	if (gets(buffer)) {
		memcpy(retBuffer,buffer,sizeof(buffer));
		return retBuffer;
	}
	return NULL;
}

void printGrades(int grds[11][100],int n,int first) {
	int s;
	printf(" %3s | %3s | %3s | %3s | %3s | %3s | %3s | %3s | %3s | %3s | %3s || %3s |\n",
		"SID","P1", "P2", "P3","P4","Q1","Q2","Q3","Q4","T1","T2","Avg");
	printf("-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----||-----|\n");
	for(s=0; s<n; s++) {
		printf(" %3d | %3d | %3d | %3d | %3d | %3d | %3d | %3d | %3d | %3d | %3d || %3d |\n",
			first+s,grds[0][s], grds[1][s], grds[2][s], grds[3][s],
			grds[4][s],grds[5][s],grds[6][s],grds[7][s],
			grds[8][s],grds[9][s],grds[10][s]);
	}
	printf("-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----||-----|\n");
}
