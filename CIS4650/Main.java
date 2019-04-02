/*
  Created by: Fei Song
  File Name: Main.java
  To Build: 
  After the scanner, tiny.flex, and the parser, tiny.cup, have been created.
    javac Main.java
  
  To Run: 
    java -classpath /usr/share/java/cup.jar:. Main gcd.tiny

  where gcd.tiny is an test input file for the tiny language.
*/

import java.io.*;
import absyn.*;

class Main {
    public static boolean SHOW_TREE = false;
    public static boolean SHOW_TABLE = false;
    public static boolean GENERATE_CODE = false;
    public static String filename = "";
    static public void main(String argv[]) {
        for (String arg : argv) {
            if (arg.equals("-a")) {
                SHOW_TREE = true;
            } else if (arg.equals("-s")) {
                SHOW_TABLE = true;
            } else if (arg.equals("-c")) {
                GENERATE_CODE = true;
            } else {
                filename = arg;
            }
        }
        /* Start the parser */
        try {
            parser p = new parser(new Lexer(new FileReader(argv[0])));
            Absyn result = (Absyn)(p.parse().value);
            if (SHOW_TREE) {
                System.out.println("The abstract syntax tree is:");
                ShowTreeVisitor visitor = new ShowTreeVisitor();
                result.accept(visitor, 0);
            }
            SemanticAnalyzer analyzer = new SemanticAnalyzer(SHOW_TABLE);
            result.accept(analyzer, 0);
            if (GENERATE_CODE) {
                CodeGenerator codeGenerator = new CodeGenerator();
                result.accept(codeGenerator, 0);
                System.out.println(codeGenerator.code);
            }
        } catch (Exception e) {
            /* do cleanup here -- possibly rethrow e */
            e.printStackTrace();
        }
    }
}


