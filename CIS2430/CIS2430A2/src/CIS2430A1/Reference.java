package CIS2430A1;

/**
 * A piece of literature. Subclasses: Book, Journal.
 * @author Brandon Chester
 * @version 1.0
 * @since 2015-10-16
 */
public class Reference {

    private String callNum;
    private String title;
    private int year;

    public Reference() {
    }

    /**
     * Initializes a piece of literature
     * @param title The title of the literature
     * @param callNum The call number of the literature
     * @param year The publication year of the literature
     */
    public Reference(String callNum, String title, int year) {
        this.callNum = callNum;
        this.title = title;
        this.year = year;
    }

    /**
     * Gets a piece of literature's call number
     * @return The call number of the piece of literature
     */
    public String getCallNum() {
        return this.callNum;
    }

    /**
     * Gets a piece of literature's title
     * @return The title of the piece of literature
     */
    public String getTitle() {
        return this.title;
    }

    /**
     * Gets a piece of literature's year of publication
     * @return The year of publication of the piece of literature
     */
    public int getYear() {
        return this.year;
    }
}
