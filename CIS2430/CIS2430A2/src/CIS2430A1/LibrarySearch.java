package CIS2430A1;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;

/**
 * A virtual library with addition and Search functions
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
    
    public void addReference(String author, String callNum, String title, String publisher, String organization, String type, int year, boolean print) {
        boolean duplicate = Search("", callNum, Integer.toString(year), false);
        if (!duplicate && checkValidEntry(callNum, title, year)) {
            if (type.equalsIgnoreCase("book")) {
                 referenceList.add(new Book(author, callNum, title, publisher, year));
            } else {
                 referenceList.add(new Journal(callNum, title, organization, year));
            }
            addToTitleMap(title);
        } else if (print && duplicate && checkValidEntry(callNum, title, year)) {
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

    private boolean checkValidEntry(String callNum, String title, int year) {
        boolean ret = true;
        if (callNum.equals("")) {
            System.out.println("New entries must have a call number.");
            ret = false;
        }
        if (title.equals("")) {
            System.out.println("New entries must a title.");
            ret = false;
        }
        if (year == 404 || year > 9999 || year < 1000) {
            System.out.println("New entries must have a valid year between the year 1000 and the year 9999.");
            ret = false;
        }
        return ret;
    }

    /**
     * Searches through the library based on the user's query and outputs all
     * matches
     *
     * @param title The title of the book/journal
     * @param callNum The call number of the book/journal
     * @param year The year range of the book/journal
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

    public void storeCatalog(String arg) {
        
        PrintWriter catalog;
        try {
            PrintWriter pw = new PrintWriter(arg);
            pw.close();
            catalog = new PrintWriter(new BufferedWriter(new FileWriter(arg, true)));
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
