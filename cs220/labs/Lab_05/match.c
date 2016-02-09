#include "rps.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define GAMES 50

int main(int argc,char **argv) {

	rps myhist[GAMES];
	rps opphist[GAMES];
	int myscore=0;
	int oppscore=0;
	int round=0;
	int bet=1;

	/* Argument... player1 / player2 */
	if (argc != 3) { 
		printf("Please enter %s <player1> <player2>\n",argv[0]);
		return(1);
	}
	
	player p1_guess,p2_guess;
	
#include "players.h"
	
	p1_guess=pick_player(argv[1]);
	p2_guess=pick_player(argv[2]);
	
	for(round=0;round<20;round++) {
		myhist[round]=Unknown;
		opphist[round]=Unknown;
	}
	
	for(round=0;round<GAMES;round++) {
		rps p1g=p1_guess(round,myhist,opphist);
		rps p2g=p2_guess(round,opphist,myhist);
		myhist[round]=p1g;
		opphist[round]=p2g;
		winner result=eval_round(p1g,p2g,argv[1],argv[2]);
		print_winner(result,argv[1],argv[2]);
		if (result==Player1) { 
			myscore+=bet;
			bet=1;
		} else if (result==Player2) {
			oppscore+=bet;
			bet=1;
		} else bet = bet * 2;
	}
	
	printf("After %d games, %s score is %d, %s score is %d\n",GAMES,argv[1],myscore,argv[2],oppscore);
	
	if (myscore==oppscore) printf("It's a tie!\n");
	else if (myscore > oppscore) printf("%s Won!!!!\n",argv[1]);
	else printf("%s Won!!!\n",argv[2]);
	
	return(0);

}
