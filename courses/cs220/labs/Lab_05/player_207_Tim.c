rps timRand(int round)
{
    time_t t;

    /* Intializes random number generator */
    if (round==0) srand((unsigned) time(&t)); 
	
    char i = rand()%3;
    switch(i) {
		case 0: return Rock;
		case 1: return Scissors;
	}
	return Paper;
}

//Finds the length of the current run of opponent throws from the given round. eg. RSPSPRRR will return 3.
int timCheckRunLength(int round, rps *opphist) {
    int i = 1;
    
    while(round > 0 && opphist[round] == opphist[round - 1])
    {
        i++;
        round--;
    }

    return i;    
}

rps getWeaker(rps hand)
{
    if(hand == Rock) return Scissors;
    else if(hand == Scissors) return Paper;
    else return Rock;
}

rps getStronger(rps hand)
{
    if(hand == Rock) return Paper;
    else if(hand == Scissors) return Rock;
    else return Scissors;
}

rps player_207_Tim(int round,rps *myhist,rps *opphist) {

    if(round == 0) return Scissors;

    //printf(">Current run length is %d\n", timCheckRunLength(round, opphist));
    

     
    //If I won last round
    if(eval_round(myhist[round - 1], opphist[round - 1],NULL,NULL) == Player1)
    {
        return myhist[round - 1];
    }
    else
    {
        return getStronger(myhist[round - 1]);
    }

    return timRand(round);	
    
    

    //return getWeaker(myhist[round -1]);
    return Paper;
}

register_player(player_207_Tim,"207Tim");
