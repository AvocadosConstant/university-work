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
	int game=0;
	int bet=1;
	
	player p1_guess,p2_guess;
	
#include "players.h"

	char * p1_name;
	char * p2_name;
	char * tourney_winner;
	
	p1_guess=next_unbeaten();
	p1_name=unbeaten_name();
	
	while (p1_guess) {
	
		p2_guess=next_unbeaten();
		p2_name=unbeaten_name();
		
		if (!p2_guess) {
			tourney_winner=p1_name;
			p1_guess=next_unbeaten();
			p1_name=unbeaten_name();
			if (p1_guess && 0==strcmp(tourney_winner,p1_name)) break; 
			if (p1_guess) printf("This round is finished... %s gets a bye\n",tourney_winner);
			continue;
		}
		
		printf("Playing %s against %s\n",p1_name,p2_name);
	
		for(game=0;game<GAMES;game++) {
			myhist[game]=Unknown;
			opphist[game]=Unknown;
		}
		myscore=0;
		oppscore=0;
	
		for(game=0;game<GAMES;game++) {
			rps p1g=p1_guess(game,myhist,opphist);
			rps p2g=p2_guess(game,opphist,myhist);
			myhist[game]=p1g;
			opphist[game]=p2g;
			// winner result=eval_round(p1g,p2g,p1_name,p2_name);
			winner result=eval_round(p1g,p2g,NULL,NULL);
			// print_winner(result,argv[1],argv[2]);
			if (result==Player1) { 
				myscore+=bet;
				bet=1;
			} else if (result==Player2) {
				oppscore+=bet;
				bet=1;
			} else bet = bet * 2;
		}
	
		printf("After %d games, %s score is %d, %s score is %d\n",GAMES,p1_name,myscore,p2_name,oppscore);
	
		if (myscore==oppscore) printf("It's a tie!\n");
		else if (myscore > oppscore) { 
			printf("%s Won!!!!\n",p1_name);
			player_lost(p2_name);
		} else { 
			printf("%s Won!!!\n",p2_name);
			player_lost(p1_name);
		}
		p1_guess=next_unbeaten();
		p1_name=unbeaten_name();
		
		if (!p1_guess) {
			printf("This round is finished... even number of players this round.\n");
			p1_guess=next_unbeaten();
			p1_name=unbeaten_name();
		}
	}
	
	printf("The tournament winner is.... %s\n",tourney_winner);
	
	return(0);

}
