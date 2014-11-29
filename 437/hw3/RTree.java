import java.util.Random;
import java.util.ArrayList;
import java.util.Arrays;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.IOException;

public class RTree {

    private static final String dataset = "project3dataset30K.txt";
    private static final String AB = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    private static Random rnd = new Random();

    private Tuple[] tuples;

    private static String genRandomString(int size) {
        StringBuilder sb = new StringBuilder(size);
        for( int i = 0; i < size; i++ )
            sb.append( AB.charAt(rnd.nextInt(AB.length())));
        return sb.toString();
    }

    private void tuplesFromFile(String filename) throws IOException {
        ArrayList<Tuple> tuplesList = new ArrayList<Tuple>();
        BufferedReader br;
        try {
            br = new BufferedReader(new FileReader(filename));
        } catch(FileNotFoundException e) {
            System.out.println("Warning: Dataset not found " + dataset);
            return;
        }
        for(String line; (line = br.readLine()) != null; ) {
            if(line.equals(""))
                break;
            String[] fields = line.split(",");
            int f1 = Integer.parseInt(fields[0]);
            int f2 = Integer.parseInt(fields[1]);
            tuplesList.add(new Tuple(f1, f2, genRandomString(500)));
        }
        this.tuples = new Tuple[tuplesList.size()];
        this.tuples = tuplesList.toArray(this.tuples);
        Arrays.sort(this.tuples);
    }

    public RTree() throws IOException {
        this.tuplesFromFile(dataset);
    }

}
