package symbol;

public class Symbol {
    public static final int INT = 0;
    public static final int VOID = 1;
    public static final int ARRAY = 2;

    public String name;
    public int type;

    public Symbol(String name, int type) {
        this.name = name;
        this.type = type;
    }

    public String type() {
        switch (type) {
            case INT:
                return "int";
            case VOID:
                return "void";
            case ARRAY:
                return "int[]";
            default:
                return "UNKNOWN";
        }
    }
}
