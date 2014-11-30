public class Tuple implements RNodeContents, Comparable<Tuple> {

    public int field1;
    public int field2;
    public String field3;

    public Tuple(int f1, int f2, String f3) {
        this.field1 = f1;
        this.field2 = f2;
        this.field3 = f3;
    }

    public String toString() {
        return field1 + ", " + field2 + ", " + field3;
    }

    public long getHilbertValue() {
        final int BITS_PER_DIM = 32;
        long res = 0;
        int x1 = this.field1;
        int x2 = this.field2;

        for (int ix = BITS_PER_DIM - 1; ix >= 0; ix--) {
            long h = 0;
            long b1 = (x1 & (1 << ix)) >> ix;
            long b2 = (x2 & (1 << ix)) >> ix;

            if (b1 == 0 && b2 == 0) {
                h = 0;
            } else if (b1 == 0 && b2 == 1) {
                h = 1;
            } else if (b1 == 1 && b2 == 0) {
                h = 3;
            } else if (b1 == 1 && b2 == 1) {
                h = 2;
            }
            res += h << (2 * ix);
        }
        return res;
    }

    @Override
    public int compareTo(Tuple x) {
        long l1 = this.getHilbertValue();
        long l2 = x.getHilbertValue();
        return (int) (l1 - l2);
    }

    @Override
    public int size() {
        return 4+4+this.field3.length();
    }

    @Override
    public int minX() {
        return this.field1;
    }

    @Override
    public int minY() {
        return this.field2;
    }

    @Override
    public int maxX() {
        return this.field1;
    }

    @Override
    public int maxY() {
        return this.field2;
    }
}
