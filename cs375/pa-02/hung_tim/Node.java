class Node {
    int level, profit, weight, bound;
    
    protected Node( ) {
        level = 0;
        profit = 0;
        weight = 0;
        bound = 0;
    }
    protected void copyList (Vector v) {
        if ( v == null || v.isEmpty( ) )
            contains = new Vector( );
        else
            contains = new Vector(v);
    }
    protected void add (int index) {
        contains.add(new Integer(index) );
    }
}
