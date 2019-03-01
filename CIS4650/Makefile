JAVA=java
JAVAC=javac
JFLEX=jflex
CLASSPATH=-classpath java-cup-11b-runtime.jar
CUP=$(JAVA) -jar java-cup-11b.jar

all: Main.class

Main.class: absyn/*.java parser.java sym.java Lexer.java ShowTreeVisitor.java Scanner.java Main.java

%.class: %.java
	$(JAVAC) $(CLASSPATH)  $^

Lexer.java: tiny.flex
	$(JFLEX) tiny.flex

parser.java: tiny.cup
	$(CUP) -expect 3 tiny.cup

clean:
	rm -f parser.java Lexer.java sym.java *.class absyn/*.class *~
