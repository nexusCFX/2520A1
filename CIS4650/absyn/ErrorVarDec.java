package absyn;

// Needed to allow parsing to continue
public class ErrorVarDec extends VarDec {

    public ErrorVarDec(int row, int col) {
        this.row = row;
        this.col = col;
    }

    public void accept( AbsynVisitor visitor, int level ) {
        visitor.visit( this, level );
    }
}