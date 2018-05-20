#include "rps.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc,char **argv) {

	/* Argument... player1 / player2 */
	if (argc != 3) { 
		printf("Please enter %s <player1> <player2>\n",argv[0]);
		return(1);
	}
	
#include "players.h"
	
	player p1_guess,p2_guess;
	
	p1_guess=pick_player(argv[1]);
	p2_guess=pick_player(argv[2]);
	
	winner result=eval_round(p1_guess(0,NULL,NULL),p2_guess(0,NULL,NULL),argv[1],argv[2]);
	print_winner(result,argv[1],argv[2]);
	
	
	
	return(0);

}


