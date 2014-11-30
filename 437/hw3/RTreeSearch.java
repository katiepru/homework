import java.io.IOException;

public class RTreeSearch {
    public static void main(String[] args) throws IOException {
        RTree h = new RTree();
        Tuple t = h.pointSearch(2563, 8966);
        System.out.println(t);
    }
}
