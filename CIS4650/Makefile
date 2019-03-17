JAVA=java
JAVAC=javac
JFLEX=jflex
CLASSPATH=-classpath java-cup-11b-runtime.jar
CUP=$(JAVA) -jar java-cup-11b.jar

all: Main.class

Main.class: absyn/*.java symbol/*.java parser.java sym.java Lexer.java SemanticAnalyzer.java ShowTreeVisitor.java Scanner.java Main.java

%.class: %.java
	$(JAVAC) $(CLASSPATH)  $^

Lexer.java: CM.flex
	$(JFLEX) CM.flex

parser.java: CM.cup
	$(CUP) -expect 3 CM.cup

clean:
	rm -f parser.java Lexer.java sym.java *.class absyn/*.class symbol/*.class *~
