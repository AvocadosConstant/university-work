import java.io.*;

public class Hung_p1 {
    
    public static void main(String[] args) {
        String filename = args[0];
        String word = args[1];

        System.out.printf("Filename: %s | Word: %s%n", filename, word);

        String string = "";
        try{
            InputStream ips=new FileInputStream(filename); 
            InputStreamReader ipsr=new InputStreamReader(ips);
            BufferedReader br=new BufferedReader(ipsr);
            String line;
            while ((line=br.readLine())!=null){
                System.out.println(line);
                string+=line+"\n";
            }
            br.close(); 
        }       
        catch (Exception e){
            System.out.println(e.toString());
        }
    }
}
