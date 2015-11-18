package CIS2430A3;

import java.util.Scanner;

/**
 * A user-facing program for managing a virtual LibrarySearch library.
 *
 * @author Brandon Chester
 * @version 1.0
 * @since 2015-10-16
 */
public class CIS2430A3 {

    public static void main(String[] args) {
        AppFrame appFrame = new AppFrame();
        Scanner in = new Scanner(System.in);
        String inputFile;
        String outputFile = "";
        String inputStr = "";
        String type;
        String author = "";
        String callNum;
        String title;
        String publisher = "";
        String organization = "";
        String year;
        LibrarySearch library = new LibrarySearch();
        if (args.length < 1) {
            outputFile = "output.txt";
        } else if (args.length == 1) {
            outputFile = args[0];
            library.fileInput(outputFile);
        } else if (args.length > 1) {
            inputFile = args[0];
            outputFile = args[1];
            library.fileInput(inputFile);
            library.fileInput(outputFile);
        }
        while (!inputStr.equalsIgnoreCase("quit") && !inputStr.equalsIgnoreCase("q")) {
            System.out.println("Please input 'Add' to add an entry, 'Search' to search the library, or 'Quit' to exit.");
            inputStr = in.nextLine().toLowerCase();

            switch (inputStr) {
                case "add":
                    System.out.println("Please input 'Book' to add a book, or 'Journal' to add a journal");
                    type = in.nextLine().toLowerCase();
                    System.out.println("Enter a call number");
                    callNum = in.nextLine();
                    System.out.println("Enter a title");
                    title = in.nextLine();
                    System.out.println("Enter the year of publication");
                    year = in.nextLine();
                    switch (type) {
                        case "book":
                            System.out.println("Optional: Enter an author");
                            author = in.nextLine();
                            System.out.println("Optional: Enter a publisher");
                            publisher = in.nextLine();
                            library.addReference(author, callNum, title, publisher, organization, type, tryParse(year), true);
                            break;
                        case "journal":
                            System.out.println("Optional: Enter an organization");
                            organization = in.nextLine();
                            library.addReference(author, callNum, title, publisher, organization, type, tryParse(year), true);
                            break;
                        default:
                            System.out.println("Invalid input.");
                            break;
                    }
                    break;
                case "search":
                    System.out.println("Optional: Enter a title");
                    title = in.nextLine();
                    System.out.println("Optional: Enter a call number");
                    callNum = in.nextLine();
                    System.out.println("Optional: Enter a year or year range");
                    year = in.nextLine();
                    System.out.println();
                    if (!library.Search(callNum, title, year, true)) {
                        System.out.println("No entries found in the library");
                    }
                    break;
                default:
                    if (!inputStr.equalsIgnoreCase("quit") && !inputStr.equalsIgnoreCase("q")) {
                        System.out.println("Invalid input.");
                    }
                    break;
            }

            library.storeCatalog(outputFile);

        }
    }

    /**
     * Ensures that input is an integer
     *
     * Use {@link #tryParse(String yearString)} to verify that the user's input
     * is an integer value. Useful for year verification.
     *
     *
     * @param yearString The string input to be verified as containing only an
     * integer
     * @return The valid integer if one exists, 404 otherwise which means
     * invalid input
     */
    public static Integer tryParse(String yearString) {
        try {
            return Integer.parseInt(yearString);
        } catch (NumberFormatException e) {
            return 404;
        }
    }
    
}
