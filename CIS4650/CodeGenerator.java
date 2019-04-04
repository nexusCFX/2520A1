import absyn.*;
import symbol.*;
import java.util.ArrayList;

// This is basically the same as the other visitors
public class CodeGenerator implements AbsynVisitor {

    public String code = "";

    private SymbolTable table;

    private static final int pc = 7;
    private static final int gp = 6;
    private static final int fp = 5;
    private static final int ac = 0;
    private static final int ac1 = 1;

    private static final int inFunAddr = 4;
    private static final int outFunAddr = 7;

    private int emitLoc = 0;
    private int highEmitLoc = 0;
    private int globalOffset = 0;
    private int entry = 0;

    private boolean shouldGetAddressOfVar = false;

    public CodeGenerator() {
        this.table = new SymbolTable(false);
    }

    public void visit(ArrayDec arrayDec, int offset) {
        Symbol s = new Symbol(arrayDec.name, Symbol.ARRAY);
        if (table.isInGlobalScope()) {
            // This is a global variable
            s.offset = globalOffset;
            globalOffset -= Integer.valueOf(arrayDec.size.value);
        } else {
            // This is a local variable or param
            s.offset = offset;
        }
        table.insert(s);
    }

    public void visit(AssignExp assignExp, int offset) {
        emitComment("-> op");
        // Standard variable on left hand side. Look up address.
        if (assignExp.lhs instanceof SimpleVar) {
            shouldGetAddressOfVar = true;
            assignExp.lhs.accept(this, offset);
            shouldGetAddressOfVar = false;
            emitRM("ST", ac, offset, fp, "");
        } else {
            assignExp.lhs.accept(this, offset);
        }

        assignExp.rhs.accept(this, offset - 1);

        emitRM("LD", ac1, offset, fp, "op: load left");
        emitRM("ST", ac, 0, ac1, "assign: store value");
        emitComment("<- op");
    }

    public void visit(CallExp callExp, int offset) {
        emitComment("-> call of function: " + callExp.func);
        int ofpFO = 0;
        int initFO = -2;
        FunctionSymbol f = (FunctionSymbol)table.get(callExp.func);

        while (callExp.args != null){
            callExp.args.head.accept(this, offset);
            emitRM("ST", ac, offset + initFO, fp, "copy arg");
            initFO--;
            callExp.args = callExp.args.tail;
        }

        emitRM("ST", fp, offset + ofpFO, fp, "store current fp");
        emitRM("LDA", fp, offset, fp, "push new frame");
        emitRM("LDA", ac, 1, pc, "save return in ac");
        emitRM_Abs("LDA", pc, f.funaddr, "jump to fun loc");
        emitRM("LD", fp, ofpFO, fp, "pop current frame");
        emitComment("<- call");
    }

    public void visit(CompoundExp compoundExp, int offset) {
        emitComment("-> compound statement");
        if (compoundExp.decs != null) {
            compoundExp.decs.accept(this, offset);
            offset -= sizeInMemory(compoundExp.decs);
        }
        if (compoundExp.exps != null) {
            compoundExp.exps.accept(this, offset);
        }
        emitComment("<- compound statement");
    }

    public void visit(DecList decList, int offset) {
        int retFO = -1;
        table.enterScope();

        // Prelude
        emitComment("Standard prelude:");
        emitRM("LD", gp, 0, ac, "load gp with maxaddr");
        emitRM("LDA", fp, 0, gp, "copy gp to fp");
        emitRM("ST", 0, 0, 0, "clear content at loc 0");
        emitComment("Jump around i/o routines here");
        int savedLoc = emitSkip(1);

        // Input
        emitComment("code for input routine");
        emitRM("ST", ac, retFO, fp, "store return");
        emitRO("IN", ac, 0, 0, "input");
        emitRM("LD", pc, retFO, fp, "return to caller");

        // Output
        emitComment("code for output routine");
        emitRM("ST", ac, retFO, fp, "store return");
        emitRM("LD", 0, -2, fp, "load output value");
        emitRO("OUT", 0, 0, 0, "output");
        emitRM("LD", 7, retFO, fp, "return to caller");

        FunctionSymbol inputSymbol = new FunctionSymbol("input", Symbol.VOID, new ArrayList<>());
        inputSymbol.funaddr = inFunAddr;
        FunctionSymbol outputSymbol = new FunctionSymbol("output", Symbol.INT, new ArrayList<>());
        inputSymbol.funaddr = outFunAddr;
        table.insert(inputSymbol);
        table.insert(outputSymbol);

        int savedLoc2 = emitSkip(0);
        emitBackup(savedLoc);
        emitRM_Abs("LDA", pc, savedLoc2, "jump around i/o code");
        emitRestore();
        emitComment("End of standard prelude.");

        while(decList != null) {
            decList.head.accept(this, offset);
            decList = decList.tail;
        }

        // Finale
        emitRM("ST", fp, globalOffset, fp, "push ofp");
        emitRM("LDA", fp, globalOffset, fp, "push frame");
        emitRM("LDA", ac, 1, pc, "Load ac with ret ptr");
        /* retrieve main function address from symbol table */
        entry = ((FunctionSymbol)table.get("main")).funaddr;
        emitRM_Abs("LDA", pc, entry, "jump to main loc");
        emitRM("LD", fp, 0, fp, "pop frame");
        emitRO("HALT", 0, 0, 0, "");

        table.exitScope();
    }

    public void visit(ExpList expList, int offset) {
        while(expList != null) {
            expList.head.accept(this, offset);
            expList = expList.tail;
        }
    }

    public void visit(FunctionDec functionDec, int offset) {
        int initFO = -2;
        int retFO = -1;
        emitComment("-> fundecl");
        emitComment("processing function: " + functionDec.func);
        emitComment("jump around function body here");

        int savedLoc = emitSkip(1);
        FunctionSymbol symbol = new FunctionSymbol(functionDec.func, Symbol.VOID, new ArrayList<>());
        symbol.funaddr = emitLoc;
        table.insert(symbol);
        table.enterScope();
        emitRM("ST", ac, retFO, fp, "store return");

        if (functionDec.params != null) {
            functionDec.params.accept(this, offset + initFO);
            initFO -= sizeInMemory(functionDec.params);
        }
        functionDec.body.accept(this, offset + initFO);

        emitRM("LD", pc, retFO, fp, "return to caller");
        int savedLoc2 = emitSkip(0);
        emitBackup(savedLoc);
        emitRM_Abs("LDA", pc, savedLoc2, "jump around function body");
        emitRestore();
        emitComment("<- fundecl");
        table.exitScope();
    }

    public void visit(IfExp ifExp, int offset) {
        table.enterScope();
        ifExp.test.accept(this, offset);
        int savedLoc = emitSkip(1);
        ifExp.thenpart.accept(this, offset);
        int savedLoc2 = emitSkip(0);
        emitBackup(savedLoc);
        emitRM_Abs("JEQ", ac, savedLoc2, "goto else if false");
        emitRestore();
        ifExp.elsepart.accept(this, offset);
        table.exitScope();
    }

    public void visit(IndexVar indexVar, int offset) {
        Symbol s = table.get(indexVar.name);
        if (table.containsInScope(indexVar.name) && table.isInGlobalScope()) {
            // Global var
            emitRM("LD", ac, s.offset, gp, "");
            emitRM("ST", ac, offset, gp, "");
        } else {
            // Local var
            emitRM("LD", ac, s.offset, fp, "");
            emitRM("ST", ac, offset, fp, "");
        }
        indexVar.index.accept(this, offset - 1);
    }

    public void visit(IntExp intExp, int offset) {
        emitComment("-> constant");
        emitRM("LDC", ac, Integer.valueOf(intExp.value), 0, "load const");
        emitComment("<- constant");
    }

    public void visit(NameTy nameTy, int offset) {
        // Required by interface
    }

    public void visit(NilExp nilExp, int offset) {
        // Required by interface
    }

    public void visit(OpExp opExp, int offset) {
        emitComment("-> op");
        opExp.left.accept(this, offset);
        // Might need to break down multiple chained expressions
        if((opExp.left instanceof IntExp) ||
                (opExp.left instanceof OpExp) ||
                (opExp.left instanceof VarExp && ((VarExp) opExp.left).variable instanceof SimpleVar)) {
            emitRM("ST", ac, offset, fp, "op: push left");
        }
        opExp.right.accept(this, offset - 1);
        emitRM("LD", ac1, offset, fp, "op: load left");

        // Setting up for comparison expressions
        if (opExp.op != OpExp.PLUS && opExp.op != OpExp.MINUS && opExp.op != OpExp.TIMES && opExp.op != OpExp.OVER && opExp.op != OpExp.EQ) {
            emitRO("SUB", ac, 1, ac, "");
        }

        switch(opExp.op){
            case OpExp.PLUS:
                emitRO("ADD", ac, ac1, ac, "op +");
                break;
            case OpExp.MINUS:
                emitRO("SUB", ac, ac1, ac, "op -");
                break;
            case OpExp.TIMES:
                emitRO("MUL", ac, ac1, ac, "op *");
                break;
            case OpExp.OVER:
                emitRO("DIV", ac, ac1, ac, "op /");
                break;
            case OpExp.EQ:
                emitRO("EQU", ac, ac1, ac, "op ==");
                break;
            case OpExp.LT:
                emitRM("JLT", ac, 2, pc, "op <");
                break;
            case OpExp.GT:
                emitRM("JGT", ac, 2, pc, "op >");
                break;
            case OpExp.NE:
                emitRM("JNE", ac, 2, pc, "op !=");
                break;
            case OpExp.LE:
                emitRM("JLE", ac, 2, pc, "op <=");
                break;
            case OpExp.GE:
                emitRM("JGE", ac, 2, pc, "op >=");
                break;
        }

        // Moving PC for comparison expressions
        if (opExp.op != OpExp.PLUS && opExp.op != OpExp.MINUS && opExp.op != OpExp.TIMES && opExp.op != OpExp.OVER && opExp.op != OpExp.EQ) {
            emitRM("LDC", ac, 0, ac, "false case");
            emitRM("LDA", pc, 1, pc, "unconditional jump");
            emitRM("LDC", ac, 1, ac, "true case");
        }
        emitComment("<- op");
    }

    public void visit(ReturnExp returnExp, int offset) {
        int retFO = -1;
        returnExp.exp.accept(this, offset);
        emitRM("LD", pc, retFO, fp, "return to caller");
    }

    public void visit(SimpleDec simpleDec, int offset) {
        emitComment("processing local var: " + simpleDec.name);

        Symbol s = new Symbol(simpleDec.name, Symbol.INT);
        if (table.isInGlobalScope()) {
            // This is a global variable
            s.offset = globalOffset;
            globalOffset--;
        } else {
            // This is a local variable or param
            s.offset = offset;
        }
        table.insert(s);
    }

    public void visit(SimpleVar simpleVar, int offset) {
        emitComment("-> id");
        emitComment("looking up id: " + simpleVar.name);
        Symbol s = table.get(simpleVar.name);
        if (table.containsInGlobalScope(simpleVar.name)) {
            // Global var
            if (shouldGetAddressOfVar) {
                emitRM("LDA", 0, s.offset, gp, "load id address");
            } else {
                emitRM("LD", 0, s.offset, gp, "load id value");
            }
        } else {
            // Local var
            if (shouldGetAddressOfVar) {
                emitRM("LDA", ac, s.offset, fp, "load id address");
            } else {
                emitRM("LD", ac, s.offset, fp, "load id value");
            }
        }
        emitComment("<- id");
    }

    public void visit(VarDecList varDecList, int offset) {
        while(varDecList != null) {
            varDecList.head.accept(this, offset);
            varDecList = varDecList.tail;
            offset--;
        }
    }

    public void visit(VarExp varExp, int offset) {
        varExp.variable.accept(this, offset);
    }

    public void visit(WhileExp whileExp, int offset) {
        table.enterScope();

        int savedLoc = emitSkip(0);
        whileExp.test.accept(this, offset);

        int savedLoc2 = emitSkip(1);
        whileExp.body.accept(this, offset);
        emitRM_Abs("LDA", pc, savedLoc, "");

        int savedLoc3 = emitSkip(0);
        emitBackup(savedLoc2);
        emitRM_Abs("JEQ", ac, savedLoc3, "exit while loop");
        emitRestore();

        table.exitScope();
    }

    public void visit(ErrorExp errorExp, int offset) {
        // Required by interface
    }

    public void visit(ErrorDec errorDec, int offset) {
        // Required by interface
    }

    public void visit(ErrorVarDec errorVarDec, int offset) {
        // Required by interface
    }

    public int emitSkip(int distance) {
        int i = emitLoc;
        emitLoc += distance;
        if (highEmitLoc < emitLoc) {
            highEmitLoc = emitLoc;
        }
        return i;
    }

    public void emitBackup(int loc) {
        if (loc > highEmitLoc) {
            emitComment("BUG in emitBackup.");
        }
        emitLoc = loc;
    }

    public void emitRestore() {
        emitLoc = highEmitLoc;
    }

    public void emitRO(String op, int r, int s, int t, String c){
        String instruction;
        if (op.length() == 2) {
            instruction = "  " + emitLoc + ":     " + op + "  " + r + "," + s + "," + t + "\t" + c + "\n";
        } else {
            instruction = "  " + emitLoc + ":    " + op + "  " + r + "," + s + "," + t + "\t" + c + "\n";
        }
        emitLoc++;
        output(instruction);
        if (highEmitLoc < emitLoc) {
            highEmitLoc = emitLoc;
        }
    }

    public void emitRM(String op, int r, int d, int s, String c) {
        String instruction;
        if (op.length() == 2) {
            instruction = "  " + emitLoc + ":     " + op + "  " + r + "," + d + "(" + s + ")" + "    " + c + "\n";
        } else {
            instruction = "  " + emitLoc + ":    " + op + "  " + r + "," + d + "(" + s + ")" + "    " + c + "\n";
        }
        emitLoc++;
        output(instruction);
        if (highEmitLoc < emitLoc) {
            highEmitLoc = emitLoc;
        }
    }

    public void emitRM_Abs(String op, int r, int a, String c) {
        String instruction;
        if (op.length() == 2) {
            instruction = "  " + emitLoc + ":     " + op + "  " + r + "," + (a - emitLoc - 1) + "(" + pc + ")" + "    " + c + "\n";
        } else {
            instruction = "  " + emitLoc + ":    " + op + "  " + r + "," + (a - emitLoc - 1) + "(" + pc + ")" + "    " + c + "\n";
        }
        emitLoc++;
        output(instruction);
        if (highEmitLoc < emitLoc) {
            highEmitLoc = emitLoc;
        }
    }

    public void emitComment(String c){
        c = "* " + c + "\n";
        output(c);
    }

    public void output(String code) {
        this.code += code;
    }

    public int sizeInMemory(VarDecList varDecList) {
        int size = 0;
        while(varDecList != null) {
            if (varDecList.head instanceof ArrayDec) {
                if (((ArrayDec) varDecList.head).size != null) {
                    size += Integer.valueOf(((ArrayDec) varDecList.head).size.value) - 1;
                }
            }
            size++;
            varDecList = varDecList.tail;
        }
        return size;
    }
}
