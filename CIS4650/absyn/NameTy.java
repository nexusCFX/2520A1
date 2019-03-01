package absyn;

public class NameTy extends Absyn {
    final static int NameTy.INT, NameTy.VOID;
    public int typ;

    public SimpleVar(int row, int col, int typ) {
        this.row = row;
        this.col = col;
        this.typ = typ;
    }

    public void accept(AbsynVisitor visitor, int level) {
        visitor.visit(this, level);
    }
}