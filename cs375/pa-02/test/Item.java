import java.util.*;

public class Item {

    public static Comparator<Item> byRatio() {
        return new Comparator<Item>() {
            public int compare(Item i1, Item i2) {
                return Double.compare(i2.getRatio(), i1.getRatio());
            }
        };
    }

    public double value;
    public double weight;

    public double getRatio() {
        return value/ weight;
    }
}
