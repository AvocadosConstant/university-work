import java.io.File;
import java.io.PrintWriter;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class program1 {
    
    public static String readLineFromFile(String filename) throws FileNotFoundException {
        File file = new File(filename);
        Scanner scan = new Scanner(file);
        return scan.nextLine();
    }

    public static void main(String[] args) {
        if(args.length != 3) {
            System.err.println("Must have 3 command line arguments!");
            return;
        }

        String seqX = "", seqY = "";
        try {
            seqX = readLineFromFile(args[0]);
            seqY = readLineFromFile(args[1]);
        }
        catch (FileNotFoundException ex) {
            System.err.println("Can't read from file");
            System.exit(1);
        }
        System.out.println(seqX + "\n" + seqY);

        File outfile = new File(args[2]);
    }
}
