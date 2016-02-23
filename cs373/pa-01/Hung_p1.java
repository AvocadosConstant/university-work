import java.io.*;
import java.util.ArrayList;
import java.util.Set;
import java.util.TreeSet;

/*
    This code is ugly and terrible and sad
    don't judge me pls it's for class
*/
public class Hung_p1 {
    
    public static ArrayList<Integer[]> states;
    public static ArrayList<Integer[]> transitions;

    public static final int START_STATE = 0;
    public static final int ACCEPT_STATE = 1;
    public static final int NULL_STATE = 2;
    public static final int START_AND_ACCEPT_STATE = 3;

    public static void parseNFA(String input) {
        String[] words = input.split("\\t");
        if(words[0].equals("state")) {                      //  reading in state
            Integer[] state = new Integer[2];
            state[0] = Integer.parseInt(words[1]);
            if(words.length == 4) state[1] = START_AND_ACCEPT_STATE;                 
            else if(words.length == 3) {
                if(words[2].equals("start")) state[1] = START_STATE;     
                else state[1] = ACCEPT_STATE;                              
            }
            else state[1] = NULL_STATE;                                  
            states.add(state);
        }
        else {                                              //  reading in transition
            Integer[] transition = new Integer[3];
            transition[0] = Integer.parseInt(words[1]);
            transition[1] = (char)(words[2].charAt(0)) - '0';
            transition[2] = Integer.parseInt(words[3]);
            transitions.add(transition);
        }
    }

    public static void main(String[] args) {
        String filename = args[0];
        String word = args[1];

        //  Initialize vectors
        states = new ArrayList<Integer[]>();
        transitions = new ArrayList<Integer[]>();

        try{
            InputStream ips=new FileInputStream(filename); 
            InputStreamReader ipsr=new InputStreamReader(ips);
            BufferedReader br=new BufferedReader(ipsr);
            String line;
            while ((line=br.readLine())!=null){
                parseNFA(line);
            }
            br.close(); 
        }       
        catch (Exception e){
            System.out.println(e.toString());
        }
        //printNFA();
/*
        for(Integer[] state : states) {
            System.out.println(state[0] + " " + state[1]);
        }
*/
        //  Find start state
        int start = -1;
        startLoop: for(Integer[] state : states) {
            if(state[1] == START_STATE || state[1] == START_AND_ACCEPT_STATE) {
                start = state[0];
                break startLoop;
            }
        }
        if(start == -1) {
            System.out.println("ERROR:  No start state found!");
        }

        ArrayList<Integer> configs = new ArrayList<Integer>();
        configs.add(start);
        String curWord = word;

        while(curWord.length() > 0) {
            int symbol = (int)(curWord.charAt(0)) - '0';                //  retrieve current symbol to be read
            ArrayList<Integer> tempConfigs = new ArrayList<Integer>();  //  temporary set to keep track of destination states
            for(int i = 0; i < configs.size(); i++) {                   //  loop through current states
                for(Integer[] trans : transitions) {                    //  find all valid transitions
                    if(trans[0] == configs.get(i) && trans[1] == symbol) {
                        boolean exists = false;
                        for(int j : tempConfigs) if(j == trans[2]) exists = true;   //  only add unique states
                        if(!exists) tempConfigs.add(trans[2]);          //  add destinations to tempConfigs
                    }
                }
            }
            configs = tempConfigs;          //  replace current configs with new calculated states
            curWord = curWord.substring(1);
        }

        boolean hasAcceptState = false;
        for(int i : configs) {
            for(Integer[] state : states) {
                if(i == state[0] && (state[1] == ACCEPT_STATE || state[1] == START_AND_ACCEPT_STATE)) hasAcceptState = true;
            }
        }

        if(hasAcceptState) {
            System.out.print("accept ");    
            for(int i : configs) {
                for(Integer[] state : states) {
                    if(i == state[0] && (state[1] == ACCEPT_STATE || state[1] == START_AND_ACCEPT_STATE)) System.out.print(i + " ");
                }
            }
        }
        else {
            System.out.print("reject ");    
            for(int i : configs) {
                for(Integer[] state : states) {
                    if(i == state[0] && (state[1] == START_STATE || state[1] == NULL_STATE)) System.out.print(i + " ");
                }
            }
        }
        System.out.println();
    }
}
