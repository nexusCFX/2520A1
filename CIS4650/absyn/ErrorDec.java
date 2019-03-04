package absyn;

// Needed to allow parsing to continue
public class ErrorDec extends Dec {

    public ErrorDec(int row, int col) {
        this.row = row;
        this.col = col;
    }

    public void accept( AbsynVisitor visitor, int level ) {
        visitor.visit( this, level );
    }
}