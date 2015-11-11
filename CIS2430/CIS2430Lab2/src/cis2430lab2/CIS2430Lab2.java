package cis2430lab2;

import java.util.ArrayList;
import java.util.Scanner;

public class CIS2430Lab2 {

    public static void main(String[] args) {
        String inputStr;
        int input = 0;
        int choice = 7;
        Scanner in = new Scanner(System.in);

        //Note, it's better to do List<GoofBasics> storeList = new ArrayList<>();
        ArrayList<GoofBasics> storeList = new ArrayList<>();

        storeList.add(new GoofBasics(1, "Sparrow Road"));
        storeList.add(new GoofBasics(2, "Sinister Road"));
        storeList.add(new GoofBasics(3, "Goblin Road"));

        while (input != 4) {
            if (choice == 7) {
                printStoreMenu();
                inputStr = input(1, 4, 0);
                input = Integer.parseInt(inputStr);
                if (input == 4) {
                    break;
                }
            }
            printChoiceMenu();
            inputStr = input(1, 7, 0);
            choice = Integer.parseInt(inputStr);
            actions(choice, storeList.get(input-1));
        }
    }

    public static void printStoreMenu() {
        System.out.println("Welcome to GoofBasics!\n" + "Select a supermarket or press 4 to exit:\n");
        System.out.println("1. Sparrow Road");
        System.out.println("2. Sinister Road");
        System.out.println("3. Goblin Road");
        System.out.println();
    }

    public static void printChoiceMenu() {
        System.out.println();
        System.out.println("1. List products");
        System.out.println("2. Add a product");
        System.out.println("3. Remove a product");
        System.out.println("4. List Employees");
        System.out.println("5. Add an Employee");
        System.out.println("6. Remove an Employee");
        System.out.println("7. Return to the store selection");
        System.out.println();
    }

    public static void actions(int choice, GoofBasics store) {
        String nameID;
        String name;
        int ID;
        int context;

        nameID = input(Integer.MIN_VALUE, Integer.MAX_VALUE, choice);
        ID = nameID.charAt(nameID.length() - 1) - '0';
        name = nameID.replaceAll("[^A-Za-z]", "");

        switch (choice) {
            case 1:
                store.listProducts();
                break;
            case 2:
                store.addProducts(ID, name);
                break;
            case 3:
                if (!store.removeProducts(ID)) {
                    System.out.println("Product removal failed. List is empty or no such product exists.");
                }
                break;
            case 4:
                store.listEmployees();
                break;
            case 5:
                store.addEmployees(ID, name);
                break;
            case 6:
                if (!store.removeEmployees(ID)) {
                    System.out.println("Employee removal failed. List is empty or no such employee exists.");
                }
                break;
            case 7:
                break;
            default:
                System.out.println("Oops, that isn't a valid input.");
                System.out.println();
                break;
        }
    }

    public static String input(int bound1, int bound2, int choice) {
        String output = "";
        String inputStr = "A";
        int input;
        boolean exitLoop = false;
        Scanner in = new Scanner(System.in);
        while (!exitLoop) {
            if (choice == 2 || choice == 5) {
                System.out.println("Please enter a name and ID");
            } else if (choice == 3 || choice == 6) {
                System.out.println("Please enter an ID");
            } else if (choice != 0) {
                break;
            }

            inputStr = in.nextLine();
            if (inputStr.equals("")) {
                System.out.println("Oops, That isn't a valid input. Try again.");
                continue;
            } else if (Character.isDigit(inputStr.charAt(inputStr.length() - 1)) && (bound2 == 4 || bound2 == 7) && inputStr.length() == 1) {
                input = inputStr.charAt(inputStr.length() - 1) - '0';

            } else if (Character.isDigit(inputStr.charAt(inputStr.length() - 1)) && bound2 == Integer.MAX_VALUE ) {
                input = inputStr.charAt(inputStr.length() - 1) - '0';

            } else {
                System.out.println("Oops, That isn't a valid input. Try again.");
                continue;
            }
            if (input < bound1 || input > bound2) {
                System.out.println("Oops, that isn't a valid input. Try again.");
                continue;
            }
            exitLoop = true;
        }
        return inputStr;
    }

}
