package CIS2430A3;

import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Scanner;

/**
 * A virtual library with addition, file IO, and search functions
 *
 * @author Brandon Chester
 * @version 1.0
 * @since 2015-10-16
 */
public class LibrarySearch {

    private ArrayList<Reference> referenceList = new ArrayList<>();
    private HashMap<String, HashSet<Integer>> titleIndex = new HashMap<String, HashSet<Integer>>();

    public LibrarySearch() {
    }

    
    /**
     * Adds a reference to the library if a matching entry does not already exist
     * @param author The author, if provided for book entries
     * @param title The title of the reference
     * @param callNum The call number of the reference
     * @param publisher The publisher, if provided for book entries
     * @param organization The organization, if provided for journal entries
     * @param type The type of reference being added
     * @param year The publication year of the reference
     * @param print Specifies if the function should output error messages
     */
    public void addReference(String author, String callNum, String title, String publisher, String organization, String type, int year, boolean print) {
        boolean duplicate = Search("", callNum, Integer.toString(year), false);
        if (!duplicate && checkValidEntry(callNum, title, year, print)) {
            if (type.equalsIgnoreCase("book")) {
                referenceList.add(new Book(author, callNum, title, publisher, year));
            } else {
                referenceList.add(new Journal(callNum, title, organization, year));
            }
            addToTitleMap(title);
        } else if (print && duplicate && checkValidEntry(callNum, title, year, print)) {
            System.out.println("An entry with that call number and year already exists.");
        }
    }

    private void addToTitleMap(String title) {
        title = title.toLowerCase();
        String[] splitTitle = title.split(" ");
        for (String token : splitTitle) {
            if (!titleIndex.containsKey(token)) {
                titleIndex.put(token, new HashSet<Integer>());
            }
            titleIndex.get(token).add(referenceList.size());
        }
    }

    private boolean checkIfMatch(Reference check, boolean hasYear, boolean hasCallNum, boolean hasTitle, String title, int year1, int year2, String callNum) {
        String[] splitTitle = title.split(" ");
        String[] splitTitle1 = check.getTitle().split(" ");
        int matchedWords = 0;

        if (hasYear) {
            if (!(check.getYear() >= year1 && check.getYear() <= year2)) {
                return false;
            }
        }

        if (hasCallNum) {
            if (!(check.getCallNum().equalsIgnoreCase(callNum))) {
                return false;
            }
        }

        if (hasTitle) {
            for (String splitTitleL : splitTitle) {
                for (String splitTitle1L : splitTitle1) {
                    if (splitTitle1L.toLowerCase().equals(splitTitleL.toLowerCase())) {
                        matchedWords++;
                        break;
                    }
                }
            }
            if (!(matchedWords == splitTitle.length)) {
                return false;
            }
        }
        return true;
    }

    private boolean checkValidEntry(String callNum, String title, int year, boolean needPrint) {
        boolean ret = true;
        if (callNum.equals("")) {
            if (needPrint) {
                System.out.println("New entries must have a call number.");
            }
            ret = false;
        }
        if (title.equals("")) {
            if (needPrint) {
                System.out.println("New entries must a title.");
            }
            ret = false;
        }
        if (year == 404 || year > 9999 || year < 1000) {
            if (needPrint) {
                System.out.println("New entries must have a valid year between the year 1000 and the year 9999.");
            }
            ret = false;
        }
        return ret;
    }
    
     /**
     * Performs the input of references from a text file to the virtual library
     *
     *
     * @param file The file with the entries to be added to the library.
     */
    public void fileInput(String file) {
        Scanner fileIn;
        String inputStr;
        String type = "";
        String author = "";
        String callNum = "";
        String title = "";
        String publisher = "";
        String organization = "";
        String year = "";
        String[] split;
        try {
            fileIn = new Scanner(new FileInputStream(file));
            while (fileIn.hasNextLine()) {
                inputStr = fileIn.nextLine();
                if (inputStr.equals("")) {
                    addReference(author, callNum, title, publisher, organization, type, tryParse(year), false);
                    continue;
                }
                split = inputStr.split(" = \"");
                if (split[1] == null) {
                    split[1] = "";
                } else { 
                    split[1] = split[1].substring(0, split[1].length() - 1);
                }
                switch (split[0]) {
                    case "type":
                        type = split[1];
                        break;
                    case "callnumber":
                        callNum = split[1];
                        break;
                    case "authors":
                        author = split[1];
                        break;
                    case "title":
                        title = split[1];
                        break;
                    case "publisher":
                        publisher = split[1];
                        break;
                    case "organization":
                        organization = split[1];
                        break;
                    case "year":
                        year = split[1];
                        break;
                    default:
                        break;
                }
            }
            addReference(author, callNum, title, publisher, organization, type, tryParse(year), false);
        } catch (FileNotFoundException ex) {
        }
    }

    /**
     * Searches through the library based on the user's query and outputs all
     * matches. If a title is provided, searching is done using a HashMap. 
     * If no title is provided a standard iterative search is performed.
     *
     * @param title The title of the reference
     * @param callNum The call number of the reference
     * @param year The year range of the reference
     * @param needPrint Specifies if matches should be printed
     * @return true if any matches are found, false if none are found
     */
    public boolean Search(String title, String callNum, String year, boolean needPrint) {
        int numOfMatchedEntries = 0;
        int numJournals = 0;
        int year1 = 0;
        int year2 = 0;
        int matchedWords = 0;

        boolean hasYear = true;
        boolean hasCallNum = true;
        boolean hasTitle = true;

        if (title.length() == 0) {
            hasTitle = false;
        }

        if (callNum.length() == 0) {
            hasCallNum = false;
        }

        if (year.length() == 9) {
            String[] splitYear = year.split("-");
            year1 = tryParse(splitYear[0]);
            year2 = tryParse(splitYear[1]);
        } else if (year.length() == 5) {
            if (year.charAt(0) == '-') {
                year1 = 1000;
                year2 = tryParse(year.replaceAll("-", ""));
            } else {
                year1 = tryParse(year.replaceAll("-", ""));
                year2 = 9999;
            }
        } else if (year.length() == 4) {
            year1 = tryParse(year);
            year2 = year1;
        } else {
            hasYear = false;
        }

        //404 is symbolically used to represent an invalid year
        if (year1 == 404 || year2 == 404) {
            System.out.println("The date range entered was invalid.");
            return false;
        }

        //This function uses the HashMap to search if the user provided a title search term
        if (hasTitle) {
            HashSet<Integer> firstSet;
            String[] splitTitle = title.split(" ");
            if (titleIndex.get(splitTitle[0]) != null) {
                firstSet = titleIndex.get(splitTitle[0]);
            } else {
                return false;
            }
            for (String token : splitTitle) {
                if (titleIndex.get(token) != null) {
                    firstSet.retainAll(titleIndex.get(token));
                } else {
                    return false;
                }
            }

            Iterator<Integer> getMatches = firstSet.iterator();
            while (getMatches.hasNext()) {
                Reference next = referenceList.get(getMatches.next() - 1);
                if (checkIfMatch(next, hasYear, hasCallNum, false, title, year1, year2, callNum)) {
                    if (needPrint) {
                        System.out.println(next);
                    }
                    numOfMatchedEntries++;
                }
            }
            return numOfMatchedEntries != 0;
        }

        //This function searches iteratively if no title term was provided
        Iterator<Reference> findBooks = referenceList.iterator();
        while (findBooks.hasNext()) {
            Reference next = findBooks.next();
            if (checkIfMatch(next, hasYear, hasCallNum, hasTitle, title, year1, year2, callNum)) {
                if (needPrint) {
                    System.out.println(next);
                }
                numOfMatchedEntries++;
            }
        }
        return numOfMatchedEntries != 0;
    }

     /**
     * Outputs the entries of the virtual library to a text file
     * @param file The text file to be written to. Defaults to "output.txt"
     */
    public void storeCatalog(String file) {

        PrintWriter catalog;
        try {
            PrintWriter pw = new PrintWriter(file);
            pw.close();
            catalog = new PrintWriter(new BufferedWriter(new FileWriter(file, true)));
            Iterator<Reference> findBooks = referenceList.iterator();
            while (findBooks.hasNext()) {
                Reference next = findBooks.next();
                catalog.println(next);
            }
            catalog.close();
        } catch (IOException ex) {
        }

    }

    private Integer tryParse(String yearString) {
        try {
            return Integer.parseInt(yearString);
        } catch (NumberFormatException e) {
            return 404;
        }
    }
}
