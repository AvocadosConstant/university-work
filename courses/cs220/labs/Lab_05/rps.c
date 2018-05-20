#include "rps.h"

#include <stdio.h>
#include <stdlib.h>

register_entry p_register[100];
int num_reg=0;
int last_winner=-1;

void register_player(player p,char *p_name) {
	p_register[num_reg].p=p;
	p_register[num_reg].p_name=p_name;
	p_register[num_reg].lost=0;
	num_reg++;
}

player pick_player(char * pname) {
	int i;
	for (i=0; i<num_reg; i++) {
		if (0==strcmp(pname,p_register[i].p_name)) return p_register[i].p;
	}
	printf("Unrecognized player... %s\n",pname);
	exit(0);
}

player next_unbeaten(void) {
	last_winner++;
	while (last_winner<num_reg) {
		if (p_register[last_winner].lost==0) return p_register[last_winner].p;
		last_winner++;
	}
	last_winner=-1;
	return NULL; /* Indicate end of round, and reset for next round */
}

char * unbeaten_name(void) {
	if (last_winner==-1) return "";
	return p_register[last_winner].p_name;
}
		

void player_lost(char *pname) {
	int i;
	for (i=0; i<num_reg; i++) {
		if (0==strcmp(pname,p_register[i].p_name)) { 
			p_register[i].lost++;
			return;
		}
	}
	printf("Unrecognized player... %s\n",pname);
	exit(0);
}

winner eval_round(rps p1, rps p2, char * p1_name, char * p2_name) {
	if (p1_name !=NULL) printf("Rock/Paper/Scissors... %s vs. %s\n",fmt_rps(p1),fmt_rps(p2));
	if ((p1==Unknown) && (p2 == Unknown)) return Tie;
	if (p1==Unknown) return Player2;
	if (p2==Unknown) return Player1;
	
	if (p1==p2) return Tie;
	if (p1==Rock && p2==Scissors) return Player1;
	if (p1==Scissors && p2==Paper) return Player1;
	if (p1==Paper && p2==Rock) return Player1;
	return Player2;
	
}

void print_winner(winner result,char * p1_name,char *p2_name) {
	switch(result) {
		case Player1 : printf("Player %s won\n",p1_name);
			return;
		case Player2 : printf("Player %s won\n",p2_name);
			return;
		default : printf("Player %s and %s tied\n",p1_name,p2_name);
	}
	return;
}


char * fmt_rps(rps c) {
	switch(c) {
		case Rock : return "Rock";
		case Paper : return "Paper";
		case Scissors : return "Scissors";
		default : return "Unknown";
	}
	return "Unknown";
}
