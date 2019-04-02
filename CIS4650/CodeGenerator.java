import absyn.*;
import symbol.*;
import java.util.ArrayList;

// This is basically the same as the other visitors
public class CodeGenerator implements AbsynVisitor {

    public String code;

    private SymbolTable table;

    private static final int pc = 7;
    private static final int gp = 6;
    private static final int fp = 5;
    private static final int ac = 0;
    private static final int ac1 = 1;

    private int emitLoc = 0;
    private int highEmitLoc = 0;
    private int globalOffset = 0;
    private int entry = 0;

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
        // Standard variable on left hand side
        if (assignExp.lhs instanceof SimpleVar) {
            emitRM("ST", ac, offset, fp, "");
        }

        assignExp.rhs.accept(this, offset - 1);

        emitRM("LD", ac1, offset, fp, "");
        emitRM("ST", ac, 0, ac1, "");
    }

    public void visit(CallExp callExp, int offset) {
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
        emitRM_Abs("LDA", pc, f.funaddr, "relative jump to function entry");
        emitRM("LD", fp, ofpFO, fp, "pop current frame");
    }

    public void visit(CompoundExp compoundExp, int offset) {
        compoundExp.decs.accept(this, offset);
        offset -= sizeInMemory(compoundExp.decs);
        compoundExp.exps.accept(this, offset);
    }

    public void visit(DecList decList, int offset) {
        int retFO = -1;
        table.enterScope();

        // Prelude
        emitRM("LD", gp, 0, ac, "load gp with maxaddr");
        emitRM("LDA", fp, 0, gp, "copy gp to fp");
        emitRM("ST", 0, 0, 0, "clear content at loc 0");
        int savedLoc = emitSkip(1);

        // Input
        emitRM("ST", ac, retFO, fp, "store return");
        emitRO("IN", ac, 0, 0, "input");
        emitRM("LD", pc, retFO, fp, "return to caller");

        // Output
        emitRM("ST", ac, retFO, fp, "store return");
        emitRM("LD", 0, -2, fp, "load output value");
        emitRO("OUT", 0, 0, 0, "output");
        emitRM("LD", 7, retFO, fp, "return to caller");

        int savedLoc2 = emitSkip(0);
        emitBackup(savedLoc);
        emitRM_Abs("LDA", pc, savedLoc2, "");
        emitRestore();

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
        int savedLoc = emitSkip(1);
        FunctionSymbol symbol = new FunctionSymbol(functionDec.func, Symbol.VOID, null);
        symbol.funaddr = emitLoc;
        table.insert(symbol);
        table.enterScope();
        emitRM("ST", ac, retFO, fp, "store return");

        functionDec.params.accept(this, offset + initFO);
        initFO -= sizeInMemory(functionDec.params);
        functionDec.body.accept(this, offset + initFO);

        emitRM("LD", pc, retFO, fp, "return to caller");
        int savedLoc2 = emitSkip(0);
        emitBackup(savedLoc);
        emitRM_Abs("LDA", pc, savedLoc2, "jump around function body");
        emitRestore();
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
        emitRM("LDC", ac, Integer.valueOf(intExp.value), 0, "copy int");
    }

    public void visit(NameTy nameTy, int offset) {
        // Required by interface
    }

    public void visit(NilExp nilExp, int offset) {
        // Required by interface
    }

    public void visit(OpExp opExp, int offset) {
        opExp.left.accept(this, offset);
        // Might need to break down multiple chained expressions
        if((opExp.left instanceof IntExp) ||
                (opExp.left instanceof OpExp) ||
                (opExp.left instanceof VarExp && ((VarExp) opExp.left).variable instanceof SimpleVar)) {
            emitRM("ST", ac, offset, fp, "");
        }
        opExp.right.accept(this, offset - 1);
        emitRM("LD", ac1, offset, fp, "");

        // Setting up for comparison expressions
        if (opExp.op != OpExp.PLUS && opExp.op != OpExp.MINUS && opExp.op != OpExp.TIMES && opExp.op != OpExp.OVER && opExp.op != OpExp.EQ) {
            emitRO("SUB", ac, 1, ac, "");
        }

        switch(opExp.op){
            case OpExp.PLUS:
                emitRO("ADD", ac, ac1, ac, "PLUS");
                break;
            case OpExp.MINUS:
                emitRO("SUB", ac, ac1, ac, "MINUS");
                break;
            case OpExp.TIMES:
                emitRO("MUL", ac, ac1, ac, "MULTIPLY");
                break;
            case OpExp.OVER:
                emitRO("DIV", ac, ac1, ac, "DIVIDE");
                break;
            case OpExp.EQ:
                emitRO("EQU", ac, ac1, ac, "EQUALITY");
                break;
            case OpExp.LT:
                emitRM("JLT", ac, 2, pc, "LESS THAN");
                break;
            case OpExp.GT:
                emitRM("JGT", ac, 2, ac, "GREATER THAN");
                break;
            case OpExp.NE:
                emitRM("JNE", ac, 2, pc, "NOT EQUAL");
                break;
            case OpExp.LE:
                emitRM("JLE", ac, 2, pc, "LESS OR EQUAL");
                break;
            case OpExp.GE:
                emitRM("JGE", ac, 2, pc, "GREATER OR EQUAL");
                break;
        }

        // Moving PC for comparison expressions
        if (opExp.op != OpExp.PLUS && opExp.op != OpExp.MINUS && opExp.op != OpExp.TIMES && opExp.op != OpExp.OVER && opExp.op != OpExp.EQ) {
            emitRM("LDC", ac, 0, ac, "");
            emitRM("LDA", pc, 1, pc, "");
            emitRM("LDC", ac, 1, ac, "");
        }
    }

    public void visit(ReturnExp returnExp, int offset) {
        int retFO = -1;
        returnExp.exp.accept(this, offset);
        emitRM("LD", pc, retFO, fp, "return to caller");
    }

    public void visit(SimpleDec simpleDec, int offset) {
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
        Symbol s = table.get(simpleVar.name);
        if (table.containsInScope(simpleVar.name) && table.isInGlobalScope()) {
            // Global var
            emitRM("LD", 0, s.offset, gp, "");
        } else {
            // Local var
            emitRM("LD", ac, s.offset, fp, "");
        }
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
        String instruction = emitLoc + ":  " + op + " " + r + "," + s + "," + t + "\t" + c + "\n";
        emitLoc++;
        output(instruction);
        if (highEmitLoc < emitLoc) {
            highEmitLoc = emitLoc;
        }
    }

    public void emitRM(String op, int r, int d, int s, String c) {
        String instruction = emitLoc + ":  " + op + "  " + r + "," + d + "(" + s + ")" + "\t" + c + "\n";
        emitLoc++;
        output(instruction);
        if (highEmitLoc < emitLoc) {
            highEmitLoc = emitLoc;
        }
    }

    public void emitRM_Abs(String op, int r, int a, String c) {
        String instruction = emitLoc + ":  " + op + "  " + r + "," + (a - emitLoc + 1) + "(" + pc + ")" + "\t" + c + "\n";
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
