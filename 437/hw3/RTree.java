import java.util.Random;
import java.util.ArrayList;
import java.util.Arrays;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.IOException;

public class RTree {

    private static final String AB = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    private static final int PGSIZE = 4096;
    private static Random rnd = new Random();
    private RNode[] leaves;
    private RNode root;

    private int numPages = 0;

    private Tuple[] tuples;

    private static double getEqVal(double a, double b, Tuple t) {
        return a*t.maxX() + b*t.maxY();
    }

    public Tuple[] maxEquation(double a, double b) {
        // Go through last page and find maximizing value
        RNode lastLeaf = leaves[leaves.length-1];
        double maxVal = 0;
        Tuple maxT = null;
        for(int i = 0; i < lastLeaf.length; i++) {
            RNodeContents tuple = lastLeaf.getItem(i);
            double val = getEqVal(a, b, (Tuple) tuple);
            if(val > maxVal) {
                maxVal = val;
                maxT = (Tuple) tuple;
            }
        }
        ArrayList<Tuple> result = new ArrayList<Tuple>();
        result.add(maxT);
        maxEquation(a, b, this.root, result);
        System.out.println("Number of accessed pages: " + this.numPages);
        this.numPages = 0;
        return result.toArray(new Tuple[0]);
    }

    public Tuple[] rangeSearch(int minX, int minY, int maxX, int maxY) {
        ArrayList<Tuple> tupleList = new ArrayList<Tuple>();
        BoundingBox search = new BoundingBox(minX, minY, maxX, maxY);
        this.rangeSearch(search, tupleList, this.root);
        System.out.println("Number of accessed pages: " + this.numPages);
        this.numPages = 0;
        return tupleList.toArray(new Tuple[0]);
    }

    public Tuple[] pointSearch(int x, int y) {
        ArrayList<Tuple> tupleList = new ArrayList<Tuple>();
        BoundingBox search = new BoundingBox(x, y, x, y);
        this.rangeSearch(search, tupleList, this.root);
        System.out.println("Number of accessed pages: " + this.numPages);
        this.numPages = 0;
        return tupleList.toArray(new Tuple[0]);
    }

    /*private Tuple pointSearch(RNode root, int x, int y) {
        if(root.isLeaf) {
            for(int i = 0; i < root.length; i++) {
                Tuple t = (Tuple) root.getItem(i);
                if(t.minX() == x && t.minY() == y)
                    return t;
            }
            return null;
        }
        for(int i = 0; i < root.length; i++) {
            BoundingBox b = (BoundingBox) root.getItem(i);
            if(!b.contains(x, y))
                continue;
            System.out.println(b.child);
            Tuple t = this.pointSearch(b.child, x, y);
            if(t != null)
                return t;
        }
        return null;
    } */

    public void maxEquation(double a, double b, RNode root, ArrayList<Tuple> result) {
        this.numPages++;
        if(root.isLeaf) {
            for(int i = 0; i < root.length; i++) {
                Tuple t = (Tuple) root.getItem(i);
                double val = getEqVal(a, b, t);
                double maxVal = getEqVal(a, b, result.get(0));
                if(val == maxVal && t != result.get(0))
                    result.add(t);
                else if(val > maxVal) {
                    result.clear();
                    result.add(t);
                }
            }
            return;
        }
        double maxVal = getEqVal(a, b, result.get(0));
        for(int i = 0; i < root.length; i++) {
            BoundingBox c = (BoundingBox) root.getItem(i);
            if((a*c.maxX() + b*c.maxY()) >= maxVal) {
                maxEquation(a, b, c.child, result);
            }
        }
    }

    private void rangeSearch(BoundingBox search, ArrayList<Tuple> tupleList, RNode root) {
        this.numPages++;
        if(root.isLeaf) {
            for(int i = 0; i < root.length; i++) {
                Tuple t = (Tuple) root.getItem(i);
                if(search.contains(t.minX(), t.minY()))
                    tupleList.add(t);
            }
            return;
        }
        for(int i = 0; i < root.length; i++) {
            BoundingBox curr = (BoundingBox) root.getItem(i);
            if(!search.intersects(curr))
                continue;
            rangeSearch(search, tupleList, curr.child);
        }
    }

    private void buildBottomUp(RNode[] currLevel) {
        BoundingBox[] boxes = new BoundingBox[currLevel.length];
        for(int i = 0; i < boxes.length; i++) {
            int minX = currLevel[i].minX();
            int maxX = currLevel[i].maxX();
            int minY = currLevel[i].minY();
            int maxY = currLevel[i].maxY();
            boxes[i] = new BoundingBox(minX, minY, maxX, maxY);
            boxes[i].setChild(currLevel[i]);
        }
        int boxesPerPage = PGSIZE/boxes[0].size();
        RNode[] newLevel = new RNode[(int)Math.ceil(((double) boxes.length)/boxesPerPage)];
        for(int i = 0; i < newLevel.length; i++) {
            int start = i*boxesPerPage;
            int end = Math.min(start + boxesPerPage, boxes.length);
            newLevel[i] = new RNode(boxes, start, end, false);
        }
        if(newLevel.length == 1) {
            this.root = newLevel[0];
            return;
        }
        this.buildBottomUp(newLevel);
    }

    private void bulkLoad() {
        int tuplesPerPage = PGSIZE/tuples[0].size();
        this.leaves = new RNode[(int)Math.ceil(((double) tuples.length)/tuplesPerPage)];
        for(int i = 0; i < this.leaves.length; i++) {
            int start = i * tuplesPerPage;
            int end = Math.min(start + tuplesPerPage, this.tuples.length);
            this.leaves[i] = new RNode(this.tuples, start, end, true);
        }
        this.buildBottomUp(this.leaves);
    }

    private static String genRandomString(int size) {
        StringBuilder sb = new StringBuilder(size);
        for( int i = 0; i < size; i++ )
            sb.append( AB.charAt(rnd.nextInt(AB.length())));
        return sb.toString();
    }

    private void tuplesFromFile(String filename) throws IOException, FileNotFoundException {
        ArrayList<Tuple> tuplesList = new ArrayList<Tuple>();
        BufferedReader br = new BufferedReader(new FileReader(filename));
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

    public RTree(String filename) throws IOException {
        this.tuplesFromFile(filename);
        this.bulkLoad();
    }

}
