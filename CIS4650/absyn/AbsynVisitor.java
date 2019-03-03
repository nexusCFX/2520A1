package absyn;

public interface AbsynVisitor {

  public void visit(Object obj, int level);

  public void visit(ArrayDec arrayDec, int level);

  public void visit(AssignExp assignExp, int level);

  public void visit(CallExp callExp, int level);

  public void visit(CompoundExp compoundExp, int level);

  public void visit(DecList decList, int level);

  public void visit(ExpList expList, int level);

  public void visit(FunctionDec functionDec, int level);

  public void visit(IfExp ifExp, int level);

 /*public void visit( ExpList exp, int level );

  public void visit( AssignExp exp, int level );

  public void visit( IfExp exp, int level );

  public void visit( IntExp exp, int level );

  public void visit( OpExp exp, int level );

  public void visit( ReadExp exp, int level );

  public void visit( RepeatExp exp, int level );

  public void visit( VarExp exp, int level );

  public void visit( WriteExp exp, int level );
  */

}
