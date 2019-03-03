package absyn;

public class ArrayDec extends VarDec {
    public String name;
    public NameTy typ;
    public IntExp size;

    public ArrayDec(int row, int col, NameTy typ, String name, IntExp size) {
        this.row = row;
        this.col = col;
        this.name = name;
        this.typ = typ;
        this.size = size;
    }

    public void accept(AbsynVisitor visitor, int level) {
        visitor.visit(this, level);
    }
}