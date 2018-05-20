all: round match tourney
.PHONY: clean test


PLAYERS= player_rock.c player_scissors.c player_paper.c player_random.c
	

round: round.c rps.h rps.c $(PLAYERS) players.h
	gcc -g -o round round.c rps.c
	
match: match.c rps.h rps.c $(PLAYERS) players.h
	gcc -g -o match match.c rps.c
	
tourney: tournament.c rps.h rps.c $(PLAYERS) players.h
	gcc -g -o tourney tournament.c rps.c			

test: round match tourney
	round rock scissors
	match rock scissors
	tourney

	
clean:
	rm round match tourney
