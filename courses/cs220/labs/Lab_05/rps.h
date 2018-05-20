#ifndef RPS_H
#define RPS_H

typedef enum {
	Rock,
	Paper,
	Scissors,
	Unknown
} rps;

typedef enum {
	Player1,
	Player2,
	Tie
} winner;

typedef rps (*player)(int round,rps * myhist,rps * opphist);

typedef struct {
	player p;
	char * p_name;
	int lost;
} register_entry;

void register_player(player p,char *p_name);
player pick_player(char * pname);
player next_unbeaten(void);
char * unbeaten_name(void);
void player_lost(char *pname);
winner eval_round(rps p1, rps p2,char *p1_name,char *p2_name);
void print_winner(winner result,char *p1_name,char *p2_name);
char * fmt_rps(rps c);

#endif
