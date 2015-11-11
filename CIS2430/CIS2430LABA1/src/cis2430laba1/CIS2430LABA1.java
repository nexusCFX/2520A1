package cis2430laba1;

import java.util.Scanner;

public class CIS2430LABA1 {
    
    public static void main(String[] args) {
        Scanner sin = new Scanner(System.in);
        int input = 0;
        String inputString = "";
        String tempString;
        String[] splitString;
        String javaTerrible;
        int stringLength;
        int count = 0;
        
        while (input != 9) {
            
            System.out.println("Menu:");
            System.out.println("1. Input String");
            System.out.println("2. Print String");
            System.out.println("3. Reverse the String");
            System.out.println("4. Tokenize String");
            System.out.println("5. Count spaces in the string");
            System.out.println("6. Check for a number in the string");
            System.out.println("7. Count Vowels");
            System.out.println("8. Count Consonants");
            System.out.println("9. Exit Program");
            
            if (sin.hasNextInt()) {
                input = sin.nextInt();
            }
            javaTerrible = sin.nextLine();
            
            if (input > 1 && input < 9 && inputString.length()==0) {
                System.out.println("Please input a string before trying other commands.");
            }
            
            switch (input) {
                case 1:
                    inputString = sin.nextLine();
                    break;
                case 2:
                    System.out.println(inputString);
                    break;
                case 3:
                    tempString = inputString;
                    inputString = "";
                    for (int i = tempString.length()-1; i >= 0; i--) {
                        inputString += tempString.charAt(i);
                    }
                    System.out.println(inputString);
                    break;
                case 4:
                    splitString = inputString.split(" ");
                    printSplit(splitString);
                    break;
                case 5:
                    stringLength = inputString.length();
                    for(int i = 0; i < stringLength; i++) {
                        if(Character.isWhitespace(inputString.charAt(i)))
                            count++;
                    }
                    System.out.println(count);
                    count = 0;
                    break;
                case 6:
                    boolean isDigit = false;
                    stringLength = inputString.length();
                    for(int i = 0; i < stringLength; i++) {
                        if(Character.isDigit(inputString.charAt(i))) {
                            isDigit = true;
                            break;
                        }
                    }
                    System.out.println(isDigit);
                    break;
                case 7:
                    tempString = inputString;
                    tempString = tempString.replaceAll("[AEIOUaeiou]","");
                    System.out.println((inputString.length()-tempString.length()));
                    break;
                case 8:
                    tempString = inputString;
                    tempString = tempString.replaceAll("[^A-Za-z]","");
                    tempString = tempString.replaceAll("[AEIOUaeiou]","");
                    System.out.println(tempString.length());
                    break;
                case 9:
                    break;
                default:
                    System.out.println("Oops, that isn't a valid input. Please try again.");
                    break;
            }
            System.out.println();
        }
    }
    
    public static void printSplit(String [] printString) {
        int length = printString.length;
 	for (int i = 0 ; i < length ; i++) {
 		System.out.println(printString[i]);
 	}
    }
}
