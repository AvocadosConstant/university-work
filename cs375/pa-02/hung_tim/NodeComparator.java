import java.util.Comparator;

public class NodeComparator implements Comparator<Node> {
    @Override
    public int compare(Node a, Node b) {
        if(a.bound > b.bound) return -1;
        if(a.bound < b.bound) return 1;
        return 0;
    }
}
