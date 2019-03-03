import absyn.*;

public class ShowTreeVisitor implements AbsynVisitor {

  final static int SPACES = 4;

  private void indent( int level ) {
    for( int i = 0; i < level * SPACES; i++ ) System.out.print( " " );
  }

  public void visit(ArrayDec arrayDec, int level) {
    indent(level);
    System.out.print("ArrayDec: ", arrayDec.name, "[",arrayDec.size.value,"] - ");
    arrayDec.accept.typ(this, level);
  }

  public void visit(AssignExp assignExp, int level) {
    ident(level);
    System.out.println("AssignExp:");
    level++;
    assignExp.lhs.accept(this, level);
    assignExp.rhs.accept(this, level);
  }

  public void visit(CallExp callExp, int level) {
    indent(level);
    System.out.println("CallExp: " + callExp.func);
    level++;
    callExp.args.accept(this, level);
  }

  public void visit(CompoundExp compoundExp, int level) {
    indent(level);
    System.out.println("CompoundExp:");
    level++;
    compoundExp.decs.accept(this, level);
    compoundExp.exps.accept(this, level);
  }

  public void visit(DecList decList, int level) {
    while(decList != null) {
      decList.head.accept(this, level);
      decList = decList.tail;
    }
  }

  public void visit(ExpList expList, int level) {
    while(expList != null) {
      expList.head.accept(this, level);
      expList = expList.tail;
    }
  }

  public void visit(FunctionDec functionDec, int level) {
//    indent(level);
    System.out.print("FunctionDec: " + functionDec.func + " - ");
    functionDec.result.accept(this, level);
    level++;
    functionDec.params.accept(this, level);
    functionDec.body.accept(this, level);
  }

  public void visit(IfExp ifExp, int level) {
    indent(level);
    System.out.println("IfExp:");
    level++;
    ifExp.test.accept(this, level);
    ifExp.thenpart.accept(this, level);
    ifExp.elsepart.accept(this, level);
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
    if (nameTy.typ == 0) {
      System.out.println("INT");
    } else {
      System.out.println("VOID");
    }
  }

  public void visit(NilExp nilExp, int level) {
    indent(level);
    System.out.println("NilExp:");
  }

  public void visit( OpExp opExp, int level ) {
    indent(level);
    System.out.print("OpExp:");

    switch(opExp.op) {
      case OpExp.PLUS:
        System.out.println( " + " );
        break;
      case OpExp.MINUS:
        System.out.println( " - " );
        break;
      case OpExp.TIMES:
        System.out.println( " * " );
        break;
      case OpExp.OVER:
        System.out.println( " / " );
        break;
      case OpExp.EQ:
        System.out.println( " = " );
        break;
      case OpExp.LT:
        System.out.println( " < " );
        break;
      case OpExp.GT:
        System.out.println( " > " );
        break;
      case OpExp.NE:
        System.out.println( " != " );
        break;
      case OpExp.LE:
        System.out.println( " >= " );
        break;
      case OpExp.GE:
        System.out.println( " >= " );
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
    level++;
    returnExp.exp.accept(this, level);
  }

  public void visit(SimpleDec simpleDec, int level) {
    indent(level);
    System.out.print("SimpleDec: " + simpleDec.name + " - ");
    simpleDec.accept.typ(this, level);
  }

  public void visit(SimpleVar simpleVar, int level) {
    indent(level);
    System.out.println("SimpleVar: " + simpleVar.name);
  }

  public void visit(VarDecList varDecList, int level) {
    while(varDecList != null) {
      varDecList.head.accept(this, level);
      varDecList = varDecList.tail;
    }
  }

  public void visit(VarExp varExp, int level) {
    indent(level);
    System.out.println("VarExp:");
    level++;
    varExp.exp.accept(this, level);
  }

  public void visit(WhileExp whileExp, int level) {
    indent(level);
    System.out.println("WhileExp:");
    level++;
    whileExp.test.accept(this, level);
    whileExp.body.accept(this, level);
  }

 /* public void visit( ExpList expList, int level ) {
    while( expList != null ) {
      expList.head.accept( this, level );
      expList = expList.tail;
    } 
  }

  public void visit( AssignExp exp, int level ) {
    indent( level );
    System.out.println( "AssignExp:" );
    level++;
    exp.lhs.accept( this, level );
    exp.rhs.accept( this, level );
  }

  public void visit( IfExp exp, int level ) {
    indent( level );
    System.out.println( "IfExp:" );
    level++;
    exp.test.accept( this, level );
    exp.thenpart.accept( this, level );
    if (exp.elsepart != null )
       exp.elsepart.accept( this, level );
  }

  public void visit( IntExp exp, int level ) {
    indent( level );
    System.out.println( "IntExp: " + exp.value ); 
  }

  public void visit( OpExp exp, int level ) {
    indent( level );
    System.out.print( "OpExp:" ); 
    switch( exp.op ) {
      case OpExp.PLUS:
        System.out.println( " + " );
        break;
      case OpExp.MINUS:
        System.out.println( " - " );
        break;
      case OpExp.TIMES:
        System.out.println( " * " );
        break;
      case OpExp.OVER:
        System.out.println( " / " );
        break;
      case OpExp.EQ:
        System.out.println( " = " );
        break;
      case OpExp.LT:
        System.out.println( " < " );
        break;
      case OpExp.GT:
        System.out.println( " > " );
        break;
      default:
        System.out.println( "Unrecognized operator at line " + exp.row + " and column " + exp.col);
    }
    level++;
    exp.left.accept( this, level );
    exp.right.accept( this, level );
  }

  public void visit( ReadExp exp, int level ) {
    indent( level );
    System.out.println( "ReadExp:" );
    exp.input.accept( this, ++level );
  }

  public void visit( RepeatExp exp, int level ) {
    indent( level );
    System.out.println( "RepeatExp:" );
    level++;
    exp.exps.accept( this, level );
    exp.test.accept( this, level ); 
  }

  public void visit( VarExp exp, int level ) {
    indent( level );
    System.out.println( "VarExp: " + exp.name );
  }

  public void visit( WriteExp exp, int level ) {
    indent( level );
    System.out.println( "WriteExp:" );
    exp.output.accept( this, ++level );
  }
*/
}
