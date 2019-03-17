package symbol;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;

public class SymbolTable {

    final static int SPACES = 4;
    private int depth = 0;

    private void indent() {
        for(int i = 0; i < depth * SPACES; i++) System.out.print(" ");
    }

    private class Scope {
        HashMap<String, Symbol> map;
        boolean isFunctionScope;

        Scope(boolean isFunctionScope) {
            this.map = new HashMap<>();
            this.isFunctionScope = isFunctionScope;
        }
    }

    // Use a linked list so we can add and remove the deepest scope to front in constant time
    // This also means we can search deepest -> global nested scope by iterating forward
    // rather than backward in our code, which will be less error-prone and allow for-each loops.
    private LinkedList<Scope> table;
    private boolean shouldDisplay;

    public SymbolTable(boolean shouldDisplay) {
        this.table = new LinkedList<>();
        this.shouldDisplay = shouldDisplay;
    }

    public void enterScope() {
        // Display logic
        if (shouldDisplay) {
            if (table.isEmpty()) {
                depth = 0;
                System.out.println("Entering the global scope:");
            } else {
                depth++;
                indent();
                System.out.println("Entering a new block:");
            }
        }

        table.addFirst(new Scope(false));
        if (table.size() == 1) {
            // Add global functions
            insert(new FunctionSymbol("input", Symbol.INT, new ArrayList<>()));
            ArrayList<Symbol> params = new ArrayList<>();
            params.add(new Symbol("x", Symbol.INT));
            insert(new FunctionSymbol("output", Symbol.VOID, params));
        }
    }

    public void exitScope() {
        Scope scope = table.removeFirst();

        // Display logic
        if (shouldDisplay) {
            // Print all symbols in scope
            scope.map.forEach((k,v) -> {
                depth++;
                indent();
                depth--;
                System.out.println(v.name + ": " + v.type());
            });

            if (table.isEmpty()) {
                System.out.println("Leaving the global scope");
            } else if (scope.isFunctionScope) {
                indent();
                System.out.println("Leaving the function scope");
                depth--;
            } else {
                indent();
                System.out.println("Leaving the block");
                depth--;
            }
        }
    }

    // Special logic to handle insertion of function and proper scoping for its parameters
    public void enterFunctionScope(FunctionSymbol symbol) {
        insert(symbol);
        table.addFirst(new Scope(true));

        // Display logic
        if (shouldDisplay) {
            depth++;
            indent();
            System.out.println("Entering the scope for function " + symbol.name + ":");
        }
    }

    public void insert(Symbol symbol) {
        table.getFirst().map.put(symbol.name, symbol);
    }

    public Symbol get(String name) {
        for (Scope scope : table) {
            if (scope.map.containsKey(name)) {
                return scope.map.get(name);
            }
        }
        return null;
    }

    public boolean contains(String name) {
        return get(name) == null;
    }

    public boolean containsInScope(String name) {
        return table.getFirst().map.containsKey(name);
    }
}

