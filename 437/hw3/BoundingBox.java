public class BoundingBox implements RNodeContents {

    private int maxX, maxY, minX, minY;
    public RNode child;

    public BoundingBox(int minX, int minY, int maxX, int maxY) {
        this.minX = minX;
        this.minY = minY;
        this.maxX = maxX;
        this.maxY = maxY;
    }

    public void setChild(RNode child) {
        this.child = child;
    }

    public boolean contains(int x, int y) {
        if(x >= minX && x <= maxX && y >= minY && y <= maxY)
            return true;
        return false;
    }

    public boolean intersects(BoundingBox b) {
        if (this.maxX() < b.minX()) return false;
        if (this.minX() > b.maxX()) return false;
        if (this.maxY() < b.minY()) return false;
        if (this.minY() > b.maxY()) return false;
        return true;
    }

    @Override
    public int size() {
        return 5*4;
    }

    @Override
    public int minX() {
        return this.minX;
    }

    @Override
    public int minY() {
        return this.minY;
    }

    @Override
    public int maxX() {
        return this.maxX;
    }

    @Override
    public int maxY() {
        return this.maxY;
    }
}
