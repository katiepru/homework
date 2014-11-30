public class RNode {

    private RNodeContents[] contents;
    private int start, end;
    public int length;
    public boolean isLeaf;

    public RNode(RNodeContents[] contents, int start, int end, boolean isLeaf) {
        this.contents = contents;
        this.start = start;
        this.end = end;
        this.length = end-start;
        this.isLeaf = isLeaf;
    }

    public RNodeContents getItem(int i) {
        if(i < 0 || i >= end-start) {
            return null;
        }
        return this.contents[start + i];
    }

    public int minX() {
        int minX = this.contents[start].minX();
        for(int i = this.start + 1; i < this.end; i++) {
            int thisMin = this.contents[i].minX();
            if(thisMin < minX)
                minX = thisMin;
        }
        return minX;
    }

    public int maxX() {
        int maxX = this.contents[start].maxX();
        for(int i = start + 1; i < end; i++) {
            int thismax = this.contents[i].maxX();
            if(thismax > maxX)
                maxX = thismax;
        }
        return maxX;
    }

    public int minY() {
        int minY = this.contents[start].minY();
        for(int i = start + 1; i < end; i++) {
            int thisMin = this.contents[i].minY();
            if(thisMin < minY)
                minY = thisMin;
        }
        return minY;
    }

    public int maxY() {
        int maxY = this.contents[start].maxY();
        for(int i = start + 1; i < end; i++) {
            int thismax = this.contents[i].maxY();
            if(thismax > maxY)
                maxY = thismax;
        }
        return maxY;
    }
}
