package absyn;

public class SimpleDec extends VarDec {
    public String name;
    public NameTy typ;

    public SimpleDec(int row, int col, NameTy typ, String name) {
        this.row = row;
        this.col = col;
        this.name = name;
        this.typ = typ;
    }

    public void accept(AbsynVisitor visitor, int level) {
        visitor.visit(this, level);
    }
}