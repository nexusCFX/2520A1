import absyn.*;
import symbol.*;
import java.util.ArrayList;

// This is basically the same as the visitor for the tree, but with table insertion/scope calls
public class SemanticAnalyzer implements AbsynVisitor {

    private SymbolTable table;
    private FunctionDec currentFunction;
    private boolean didPreviouslyAddScopeForFunction = false;

    // This is a value we use when ensuring an expression resolves to an int, mainly for validating
    // that a function returns an int when it's put inside an array index, assigned to an int var, or as a function arg.
    private int REQUIRE_INT_TYPE = 314;
    public boolean encounteredErrors = false;

    public SemanticAnalyzer(boolean displayScopes) {
        this.table = new SymbolTable(displayScopes);
    }

    public void visit(ArrayDec arrayDec, int typeRequirement) {
        String name = arrayDec.name;
        if (table.containsInScope(name)) {
            // Redeclaration error
            System.err.println("Error " + arrayDec.row + ":" + arrayDec.col + ". Invalid redeclaration of variable " + arrayDec.name + ".");
            return;
        }
        if (arrayDec.typ.typ != NameTy.INT) {
            // Type error
            System.err.println("Error " + arrayDec.row + ":" + arrayDec.col + ". Array type must be int[]");
            return;
        }

        table.insert(new Symbol(name, Symbol.ARRAY));
    }

    public void visit(AssignExp assignExp, int typeRequirement) {
        assignExp.lhs.accept(this, typeRequirement);
        assignExp.rhs.accept(this, REQUIRE_INT_TYPE);
    }

    public void visit(CallExp callExp, int typeRequirement) {
        String name = callExp.func;

        Symbol symbol = table.get(name);
        if (symbol == null) {
            // Undefined function
            System.err.println("Error " + callExp.row + ":" + callExp.col + ". Function " + callExp.func + " is not defined.");
            return;
        }

        if (!(symbol instanceof FunctionSymbol)) {
            // Symbol is not a function
            System.err.println("Error " + callExp.row + ":" + callExp.col + ". " + callExp.func + " is not a function.");
            return;
        }

        if (typeRequirement == REQUIRE_INT_TYPE && symbol.type != Symbol.INT) {
            // Function needs to return an int but it returns a void. This is used for functions assigned to ints, passed as ints, or used as array indices.
            System.err.println("Error " + callExp.row + ":" + callExp.col + ". Expected function of type int but found void.");
        }

        FunctionSymbol functionSymbol = (FunctionSymbol) symbol;

        int numArgs = 0;
        ExpList args = callExp.args;
        while (args != null) {
            numArgs += 1;
            args = args.tail;
        }

        if (functionSymbol.parameters.size() != numArgs) {
            // Error, expected k parameters, got n
            System.err.println("Error " + callExp.row + ":" + callExp.col + ". Expected " + functionSymbol.parameters.size() + " parameters but received " + numArgs + ".");
            return;
        }

        args = callExp.args;
        for (Symbol param : functionSymbol.parameters) {
            Exp arg = args.head;
            if (param.type == Symbol.ARRAY) {
                // If the type of the actual argument isn't an array
                if (arg instanceof VarExp && !(((VarExp) arg).variable instanceof IndexVar)) {
                    String argName = ((SimpleVar) (((VarExp) arg).variable)).name;
                    Symbol s = table.get(argName);
                    if (s == null) {
                        System.err.println("Error " + arg.row + ":" + arg.col + ". Undefined variable \'" + argName + "\' referenced.");
                    } else if (s.type != Symbol.ARRAY) {
                        System.err.println("Error " + arg.row + ":" + arg.col + ". Expected type int[] but found type " + s.type() + ".");
                    }
                }
            } else if (param.type == Symbol.INT) {
                arg.accept(this, REQUIRE_INT_TYPE);
            }
            args = args.tail;
        }
    }

    public void visit(CompoundExp compoundExp, int typeRequirement) {
        // If we are in a function body, new scope is already handled.
        if (didPreviouslyAddScopeForFunction == false) {
            table.enterScope();
        }
        didPreviouslyAddScopeForFunction = false;

        if (compoundExp.decs != null) {
            compoundExp.decs.accept(this, typeRequirement);
        }
        if (compoundExp.exps != null) {
            compoundExp.exps.accept(this, typeRequirement);
        }
        table.exitScope();
    }

    public void visit(DecList decList, int typeRequirement) {
        table.enterScope();
        while(decList != null) {
            decList.head.accept(this, typeRequirement);
            decList = decList.tail;
        }
        table.exitScope();
    }

    public void visit(ExpList expList, int typeRequirement) {
        while(expList != null) {
            expList.head.accept(this, typeRequirement);
            expList = expList.tail;
        }
    }

    public void visit(FunctionDec functionDec, int typeRequirement) {
        Symbol symbol = table.get(functionDec.func);
        if (symbol instanceof FunctionSymbol) {
            System.err.println("Error " + functionDec.row + ":" + functionDec.col + ". Invalid redeclaration of function " + functionDec.func + ".");
            return;
        }

        ArrayList<Symbol> params = new ArrayList<>();
        VarDecList decList = functionDec.params;

        while (decList != null) {
            if (decList.head instanceof ArrayDec) {
                params.add(new Symbol(((ArrayDec)decList.head).name, Symbol.ARRAY));
            } else if (decList.head instanceof SimpleDec) {
                params.add(new Symbol(((SimpleDec)decList.head).name, Symbol.INT));
            }
            decList = decList.tail;
        }


        if (functionDec.result.typ == NameTy.VOID) {
            table.enterFunctionScope(new FunctionSymbol(functionDec.func, Symbol.VOID, params));
        } else {
            table.enterFunctionScope(new FunctionSymbol(functionDec.func, Symbol.INT, params));
        }

        currentFunction = functionDec;
        didPreviouslyAddScopeForFunction = true;

        // Add parameters and body in the new scope
        if (functionDec.params != null) {
            functionDec.params.accept(this, typeRequirement);
        }
        functionDec.body.accept(this, typeRequirement);
    }

    public void visit(IfExp ifExp, int typeRequirement) {
        ifExp.test.accept(this, REQUIRE_INT_TYPE);
        ifExp.thenpart.accept(this, typeRequirement);
        if (ifExp.elsepart != null) {
            ifExp.elsepart.accept(this, typeRequirement);
        }
    }

    public void visit(IndexVar indexVar, int typeRequirement) {
        String name = indexVar.name;
        Symbol symbol = table.get(name);
        if (symbol == null) {
            // Undefined error
            System.err.println("Error " + indexVar.row + ":" + indexVar.col + ". Undefined variable \'" + name + "\' referenced.");
            return;
        } else if (symbol.type != Symbol.ARRAY) {
            // int type used as array
            System.err.println("Error " + indexVar.row + ":" + indexVar.col + ". Expected type int[] but found type " + symbol.type() + ".");
            return;
        }
        indexVar.index.accept(this, REQUIRE_INT_TYPE);
    }

    public void visit(IntExp intExp, int typeRequirement) {
        // Required by interface
    }

    public void visit(NameTy nameTy, int typeRequirement) {
        // Required by interface
    }

    public void visit(NilExp nilExp, int typeRequirement) {
        // Required by interface
    }

    public void visit( OpExp opExp, int typeRequirement ) {
        opExp.left.accept(this, typeRequirement);
        opExp.right.accept(this, typeRequirement);
    }

    public void visit(ReturnExp returnExp, int typeRequirement) {
        if (currentFunction.result.typ == NameTy.INT) {
            if (returnExp.exp == null) {
                System.err.println("Error " + returnExp.row + ":" + returnExp.col + ". Function of type int must return a value.");
            } else {
                returnExp.exp.accept(this, REQUIRE_INT_TYPE);
            }
        } else if (currentFunction.result.typ == NameTy.VOID) {
            if (returnExp.exp != null) {
                // Error, int value returned from void function.
                System.err.println("Error " + returnExp.row + ":" + returnExp.col + ". Value returned from void function.");
            }
        }
    }

    public void visit(SimpleDec simpleDec, int typeRequirement) {
        String name = simpleDec.name;
        if (table.containsInScope(name)) {
            // Redeclaration error
            System.err.println("Error " + simpleDec.row + ":" + simpleDec.col + ". Invalid redeclaration of variable " + simpleDec.name + ".");
            return;
        }
        if (simpleDec.typ.typ != NameTy.INT) {
            // Type error
            System.err.println("Error " + simpleDec.row + ":" + simpleDec.col + ". Expected type int but found type void.");
            return;
        }

        table.insert(new Symbol(name, Symbol.INT));
    }

    public void visit(SimpleVar simpleVar, int typeRequirement) {
        String name = simpleVar.name;
        Symbol symbol = table.get(name);
        if (symbol == null) {
            // Undefined error
            System.err.println("Error " + simpleVar.row + ":" + simpleVar.col + ". Undefined variable \'" + name + "\' referenced.");
        } else if (symbol.type != Symbol.INT) {
            // Type error
            System.err.println("Error " + simpleVar.row + ":" + simpleVar.col + ". Expected type int but found type " + symbol.type() + ".");
        }
    }

    public void visit(VarDecList varDecList, int typeRequirement) {
        while(varDecList != null) {
            varDecList.head.accept(this, typeRequirement);
            varDecList = varDecList.tail;
        }
    }

    public void visit(VarExp varExp, int typeRequirement) {
        varExp.variable.accept(this, typeRequirement);
    }

    public void visit(WhileExp whileExp, int typeRequirement) {
        whileExp.test.accept(this, REQUIRE_INT_TYPE);
        whileExp.body.accept(this, typeRequirement);
    }

    public void visit(ErrorExp errorExp, int typeRequirement) {
        // Required by interface
    }

    public void visit(ErrorDec errorDec, int typeRequirement) {
        // Required by interface
    }

    public void visit(ErrorVarDec errorVarDec, int typeRequirement) {
        // Required by interface
    }
}
