package symbol;

import java.util.ArrayList;

public class FunctionSymbol extends Symbol {
    public ArrayList<Symbol> parameters;

    public FunctionSymbol(String name, int returnType, ArrayList<Symbol> parameters) {
        super(name, returnType);
        this.parameters = parameters;
    }

    public String type() {
        StringBuilder output = new StringBuilder("(");
        for (Symbol param : parameters) {
            switch (param.type) {
                case INT:
                    output.append("int, ");
                    break;
                case VOID:
                    output.append("void, ");
                    break;
                case ARRAY:
                    output.append("int[], ");
                    break;
                default:
                    break;
            }
        }
        if (parameters.size() == 0) {
            output.append("void, ");
        }
        output.delete(output.lastIndexOf(","), output.length());
        output.append(") -> ");
        switch (type) {
            case INT:
                output.append("int");
                break;
            case VOID:
                output.append("void");
                break;
            case ARRAY:
                output.append("int[]");
                break;
            default:
                output.append("UNKNOWN");
                break;
        }
        return output.toString();
    }
}
