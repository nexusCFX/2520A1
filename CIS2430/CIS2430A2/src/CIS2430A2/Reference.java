package CIS2430A2;

/**
 * A library reference. Subclasses: Book, Journal.
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
     * Initializes a reference
     * @param title The title of the reference
     * @param callNum The call number of the reference
     * @param year The publication year of the reference
     */
    public Reference(String callNum, String title, int year) {
        this.callNum = callNum;
        this.title = title;
        this.year = year;
    }

    /**
     * Gets a reference's call number
     * @return The call number of the reference
     */
    public String getCallNum() {
        return this.callNum;
    }

    /**
     * Gets a reference's title
     * @return The title of the reference
     */
    public String getTitle() {
        return this.title;
    }

    /**
     * Gets a reference's year of publication
     * @return The year of publication of the reference
     */
    public int getYear() {
        return this.year;
    }
}
