import absyn.*;

public class ShowTreeVisitor implements AbsynVisitor {

    final static int SPACES = 4;

    private void indent(int level) {
        for (int i = 0; i < level * SPACES; i++) System.out.print(" ");
    }

    public void visit(ArrayDec arrayDec, int level) {
        indent(level);
        if (arrayDec.size != null) {
            System.out.print("ArrayDec: " + arrayDec.name + "[" + arrayDec.size.value + "] - ");
        } else {
            System.out.print("ArrayDec: " + arrayDec.name + "[] - ");
        }
        arrayDec.typ.accept(this, level);
    }

    public void visit(AssignExp assignExp, int level) {
        indent(level);
        System.out.println("AssignExp: ");
        level++;
        assignExp.lhs.accept(this, level);
        assignExp.rhs.accept(this, level);
    }

    public void visit(CallExp callExp, int level) {
        indent(level);
        System.out.println("CallExp: " + callExp.func);
        level++;
        if (callExp.args != null) {
            callExp.args.accept(this, level);
        }
    }

    public void visit(CompoundExp compoundExp, int level) {
        indent(level);
        System.out.println("CompoundExp: ");
        level++;
        if (compoundExp.decs != null) {
            compoundExp.decs.accept(this, level);
        }
        if (compoundExp.exps != null) {
            compoundExp.exps.accept(this, level);
        }
    }

    public void visit(DecList decList, int level) {
        while (decList != null) {
            decList.head.accept(this, level);
            decList = decList.tail;
        }
    }

    public void visit(ExpList expList, int level) {
        while (expList != null) {
            expList.head.accept(this, level);
            expList = expList.tail;
        }
    }

    public void visit(FunctionDec functionDec, int level) {
//    indent(level);
        System.out.print("FunctionDec: " + functionDec.func + " - ");
        functionDec.result.accept(this, level);
        level++;
        if (functionDec.params != null) {
            functionDec.params.accept(this, level);
        }
        functionDec.body.accept(this, level);
    }

    public void visit(IfExp ifExp, int level) {
        indent(level);
        System.out.println("IfExp: ");
        level++;
        ifExp.test.accept(this, level);
        ifExp.thenpart.accept(this, level);
        if (ifExp.elsepart != null) {
            ifExp.elsepart.accept(this, level);
        }
    }

    public void visit(IndexVar indexVar, int level) {
        indent(level);
        System.out.println("IndexVar: " + indexVar.name);
        level++;
        indexVar.index.accept(this, level);
    }

    public void visit(IntExp intExp, int level) {
        indent(level);
        System.out.println("IntExp: " + intExp.value);
    }

    public void visit(NameTy nameTy, int level) {
        if (nameTy.typ == NameTy.INT) {
            System.out.println("INT");
        } else if (nameTy.typ == NameTy.VOID) {
            System.out.println("VOID");
        } else {
            System.out.println("UNRECOGNIZED TYPE");
        }
    }

    public void visit(NilExp nilExp, int level) {
        indent(level);
        System.out.println("NilExp:");
    }

    public void visit(OpExp opExp, int level) {
        indent(level);
        System.out.print("OpExp: ");

        switch (opExp.op) {
            case OpExp.PLUS:
                System.out.println(" + ");
                break;
            case OpExp.MINUS:
                System.out.println(" - ");
                break;
            case OpExp.TIMES:
                System.out.println(" * ");
                break;
            case OpExp.OVER:
                System.out.println(" / ");
                break;
            case OpExp.EQ:
                System.out.println(" == ");
                break;
            case OpExp.LT:
                System.out.println(" < ");
                break;
            case OpExp.GT:
                System.out.println(" > ");
                break;
            case OpExp.NE:
                System.out.println(" != ");
                break;
            case OpExp.LE:
                System.out.println(" >= ");
                break;
            case OpExp.GE:
                System.out.println(" >= ");
                break;
            default:
                System.out.println("Unrecognized operator at line " + opExp.row + " and column " + opExp.col);
        }
        level++;
        opExp.left.accept(this, level);
        opExp.right.accept(this, level);
    }

    public void visit(ReturnExp returnExp, int level) {
        indent(level);
        System.out.println("ReturnExp: ");
        level++;
        if (returnExp.exp != null) {
            returnExp.exp.accept(this, level);
        }
    }

    public void visit(SimpleDec simpleDec, int level) {
        indent(level);
        System.out.print("SimpleDec: " + simpleDec.name + " - ");
        simpleDec.typ.accept(this, level);
    }

    public void visit(SimpleVar simpleVar, int level) {
        indent(level);
        System.out.println("SimpleVar: " + simpleVar.name);
    }

    public void visit(VarDecList varDecList, int level) {
        while (varDecList != null) {
            varDecList.head.accept(this, level);
            varDecList = varDecList.tail;
        }
    }

    public void visit(VarExp varExp, int level) {
        indent(level);
        System.out.println("VarExp: ");
        level++;
        varExp.variable.accept(this, level);
    }

    public void visit(WhileExp whileExp, int level) {
        indent(level);
        System.out.println("WhileExp: ");
        level++;
        whileExp.test.accept(this, level);
        whileExp.body.accept(this, level);
    }

    public void visit(ErrorExp errorExp, int level) {
        indent(level);
        System.out.println("ErrorExp: ");
    }

    public void visit(ErrorDec errorDec, int level) {
        indent(level);
        System.out.println("ErrorDec: ");
    }

    public void visit(ErrorVarDec errorVarDec, int level) {
        indent(level);
        System.out.println("ErrorVarDec: ");
    }
}
