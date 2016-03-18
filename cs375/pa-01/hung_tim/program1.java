import java.io.File;
import java.io.PrintWriter;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Scanner;
import java.util.Arrays;

/**
 * @author  Tim Hung <thung1@binghamton.edu>
 * 
 * Recursive, memoized, bottom-up dynamic approach to the least common subsequence problem.
 * 
 * Developed for CS 375 (Design and Analysis of Algorithms) Spring 2016 at Binghamton University.
 * Programming Assignment 1: Task 1
 */
public class program1 {
  
    /** 
     * Memoization matrix used to store lengths of longest common subsequences of subproblems.
     */
    public static int[][] memo;

    /**
     * Memoization matrix used to store direction of computation.
     *
     * Contains "—" for left, "|" for up, and "\" for diagonal.
     */
    public static char[][] dir;

    /**
     * Returns first line of file from given filename
     *
     * @param   filename Name of file to read.
     * @return  First line of read file.
     * @exception FileNotFoundException Something goes wrong with reading the file.
     */
    public static String readLineFromFile(String filename) throws FileNotFoundException {
        File file = new File(filename);
        Scanner scan = new Scanner(file);
        return scan.nextLine();
    }

    public static int fillMemos(char[] x, char[] y) {
        for(int row = 0; row < x.length; row++) {
            for(int col = 0; col < y.length; col++) {
                if(x[row] == y[col]) {
                    program1.memo[row][col] = (row == 0 || col == 0) ? 1 : program1.memo[row-1][col-1] + 1;
                    program1.dir[row][col] = '\\';   //  Diagonal arrow
                } 
                else {
                    int up = (row == 0) ? 0 : program1.memo[row-1][col];
                    int lf = (col == 0) ? 0 : program1.memo[row][col-1];
                    if(up >= lf) {
                        program1.memo[row][col] = up;
                        program1.dir[row][col] = '|';
                    }
                    else {
                        program1.memo[row][col] = lf;
                        program1.dir[row][col] = '—';
                    }
                }
            }
        }
        return program1.memo[x.length-1][y.length-1];
    }

    /**
     *  Finds length of the longest common subsequence of two char arrays. 
     *
     * @param x     Represents first sequence
     * @param y     Represents second sequence
     * @param i     Current index of x
     * @param j     Current index of y
     * @return Length of the longest common subsequence.
     */
    public static int lcs(char[] x, char[] y, int i, int j) {
        if(x.length == 0 || y.length == 0 || i < 0 || j < 0) return 0;

        //  Checks memo to see if this subproblem has already been solved. If so, just return the answer!
        if(program3.memo[i][j] > -1) return program3.memo[i][j];

        else if(x[i] == y[j]) program3.memo[i][j] = (i == 0 && j == 0) ? 1 : lcs(x, y, i-1, j-1) + 1; 
        else program3.memo[i][j] = Math.max(lcs(x, y, i-1, j), lcs(x, y, i, j-1));
        return program3.memo[i][j]; 
    }

    public static String constructLCS(char[] seq, int i, int j) {
        if(i < 0 || j < 0) return "";
        if(program1.dir[i][j] == '\\') return constructLCS(seq, i-1, j-1) + seq[i];
        if(program1.dir[i][j] == '|') return constructLCS(seq, i-1, j);
        return constructLCS(seq, i, j-1);
    }

    /**
     * Prints out an integer matrix.
     *
     * @param m Matrix to be printed
     */
    public static void printMatrix(int[][] m) {
        System.err.println();
        for(int i = 0; i < m.length; i++) {
            for(int j = 0; j < m[0].length; j++) System.err.print(m[i][j]);
            System.err.println();
        }
    }
    /**
     * Prints out a char matrix.
     *
     * @param m Matrix to be printed
     */
    public static void printMatrix(char[][] m) {
        System.err.println();
        for(int i = 0; i < m.length; i++) {
            for(int j = 0; j < m[0].length; j++) System.err.print(m[i][j]);
            System.err.println();
        }
    }

    public static void main(String[] args) throws IOException{
        if(args.length != 3) {System.err.println("Must have 3 command line arguments! Exiting..."); System.exit(1);}

        //  Read in strings from files as char arrays
        char[] seqX = {}, seqY = {};
        try {
            seqX = readLineFromFile(args[0]).toCharArray();
            seqY = readLineFromFile(args[1]).toCharArray();
        }
        catch (FileNotFoundException ex) {
            System.err.println("Can't read from file! Exiting..."); 
            System.exit(1);
        }

        //  Initialize memoization matrices.
        program1.memo = new int[seqX.length][seqY.length];
        program1.dir = new char[seqX.length][seqY.length];
        //for(int[] row : program1.memo) Arrays.fill(row, -1);

        //  Print out read char arrays for debugging
        System.err.printf("%nString from %s: ", args[0]);
        for(char c : seqX) System.err.print(c); 
        System.err.printf("%nString from %s: ", args[1]);
        for(char c : seqY) System.err.print(c); System.err.println();


        //  Find LCS length and time the execution
        long startTime = System.nanoTime();

        //int lcsLength = lcs(seqX, seqY, seqX.length - 1, seqY.length - 1);
        int lcsLength = fillMemos(seqX, seqY);
        String constructedLCS = constructLCS(seqX, seqX.length-1, seqY.length-1);
        long endTime = System.nanoTime();
        double duration = (endTime - startTime) / 1000000.0;    //In milliseconds

        printMatrix(program1.memo);
        printMatrix(program1.dir);

        System.err.printf("%nLCS is %s and is length %d. Execution took %f ms.%nWriting into file %s.%n%n", constructedLCS, lcsLength, duration, args[2]);
        
        //  Write results to output
        File outfile = new File(args[2]);
        PrintWriter writer = new PrintWriter(outfile, "UTF-8");
        if(Math.max(seqX.length, seqY.length) > 10) writer.printf("%d%n%f ms%n", lcsLength, duration);
        else {
            for(int i = 0; i < program1.memo.length; i++) {
                for(int j = 0; j < program1.memo[0].length; j++) writer.printf("%d",program1.memo[i][j]);
                writer.println();
            }
            writer.printf("%s%n%f ms%n", constructedLCS, duration);
        }
        writer.close();
    }
}
