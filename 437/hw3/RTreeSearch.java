import java.io.IOException;
import java.util.Scanner;
import java.util.Arrays;
import java.io.FileNotFoundException;
import java.util.InputMismatchException;

public class RTreeSearch {

    private static final String dataset = "project3dataset30K.txt";
    private static Scanner in = new Scanner(System.in);

    public static int readInt() {
        while(true) {
            try {
                int i = in.nextInt();
                return i;
            } catch(InputMismatchException e) {
                System.out.println("Bad int. Enter again");
            }
        }
    }

    public static double readDouble() {
        while(true) {
            try {
                double i = in.nextDouble();
                return i;
            } catch(InputMismatchException e) {
                System.out.println("Bad double. Enter again");
            }
        }
    }

    public static void printArr(Tuple[] arr) {
        for(Tuple t : arr) {
            System.out.println(t);
        }
    }

    public static void pointQuery(RTree r) {
        int x, y;
        System.out.println("Enter x coordinate");
        x = readInt();
        System.out.println("Enter y coordinate");
        y = readInt();

        Tuple[] result = r.pointSearch(x, y);
        System.out.println("Number of results: " + result.length);
        printArr(result);
    }

    public static void rangeQuery(RTree r) {
        int minX, minY, maxX, maxY;
        do {
            System.out.println("Enter minX, then maxX");
            minX = readInt();
            maxX = readInt();
        } while(minX > maxX);
        do {
            System.out.println("Enter minY, then maxY");
            minY = readInt();
            maxY = readInt();
        } while(minY > maxY);

        Tuple[] result = r.rangeSearch(minX, minY, maxX, maxY);
        System.out.println("Number of results: " + result.length);
        printArr(result);
    }

    public static void maxEquation(RTree r) {
        double a, b;
        do {
            System.out.println("Enter a");
            a = readDouble();
            System.out.println("Enter b");
            b = readDouble();
        } while((a+b != 1) || (a < 0) || (b < 0));

        Tuple[] result = r.maxEquation(a, b);
        System.out.println("Number of results: " + result.length);
        printArr(result);
    }

    public static void main(String[] args) throws IOException {
        //Build tree
        RTree r = null;
        while(r == null) {
            System.out.println("Enter dataset");
            String filename = in.nextLine();
            try {
                r = new RTree(filename);
            } catch(FileNotFoundException e) {
                r = null;
            }
        }

        int option = 0;
        String menu = "Choose an option:\n"
            + "1. Point query\n"
            + "2. Range query\n"
            + "3. Maximize equation\n"
            + "4. Exit";
        do {
            System.out.println(menu);
            option = readInt();
            while(option < 1 || option > 4) {
                System.out.println("Invalid option, choose again.");
                option = readInt();
            }

            switch(option) {
                case(1): pointQuery(r);
                         break;
                case(2): rangeQuery(r);
                         break;
                case(3): maxEquation(r);
                         break;
            }

        } while(option != 4);
    }
}
