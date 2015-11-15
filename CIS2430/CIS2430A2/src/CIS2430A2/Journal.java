package CIS2430A2;

import java.util.Objects;

/**
 * A journal. Parent class: Reference.
 *
 * @author Brandon Chester
 * @version 1.0
 * @since 2015-10-16
 */
public class Journal extends Reference {

    private String organization;

    public Journal() {
    }

    /**
     * Initializes a book
     *
     * @param title The title of the journal
     * @param callNum The call number of the journal
     * @param organization The organization that publishes the journal
     * @param year The publication year of the journal
     */
    public Journal(String callNum, String title, String organization, int year) {
        super(callNum, title, year);
        this.organization = organization;
    }

    /**
     * Gets a journal's organization
     *
     * @return The organization that publishes the journal
     */
    public String getOrganization() {
        return this.organization;
    }

    /**
     * Prints the attributes of a journal
     *
     * @return A string with the attributes of the journal
     */
    @Override
    public String toString() {
        String str = "";
        str += ("type = \"journal\"" + System.getProperty("line.separator")
                + "callnumber = \"" + this.getCallNum() + "\"" + System.getProperty("line.separator")
                + "title = \"" + this.getTitle() + "\"" + System.getProperty("line.separator"));
        if (!this.getOrganization().equals("")) {
            str += ("organization = \"" + this.getOrganization() + "\"" + System.getProperty("line.separator"));
        }
        str += ("year = \"" + this.getYear() + "\"" + System.getProperty("line.separator"));
        return str;
    }

    /**
     * Checks if an object is equal to a given journal
     *
     * @param other The object to be checked against a journal object.
     * @return A boolean where true indicates that the objects are equal.
     */
    @Override
    public boolean equals(Object other) {

        if (other == null) {
            return false;
        }
        if (other == this) {
            return true;
        }
        if (!(other instanceof Journal)) {
            return false;
        }
        Journal j = (Journal) other;
        return j.getTitle().equalsIgnoreCase(this.getTitle())
                && j.getCallNum().equalsIgnoreCase(this.getCallNum())
                && j.getOrganization().equalsIgnoreCase(this.getOrganization())
                && j.getYear() == this.getYear();
    }

    
    /**
     * Checks the hashcode of a book
     *@return The hashcode of the book
     */
    @Override
    public int hashCode() {
        int hash = 7;
        hash = 59 * hash + Objects.hashCode(this.organization);
        return hash;
    }
}
