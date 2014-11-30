import java.io.IOException;

public class RTreeSearch {
    public static void main(String[] args) throws IOException {
        RTree h = new RTree();
        Tuple[] t = h.rangeSearch(2563, 5000, 9000, 8966);
        for(int i = 0; i < t.length; i++)
            System.out.println(t[i]);
    }
}
