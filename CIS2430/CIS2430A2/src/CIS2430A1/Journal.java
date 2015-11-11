package CIS2430A1;

/**
 * A journal. Parent class: Reference.
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
     * @return The organization that publishes the journal
     */
    public String getOrganization() {
        return this.organization;
    }
    
    /**
     * Prints the attributes of a  journal
     * @return A string with the attributes of the journal
     */
    @Override
    public String toString(){
        String str = "";
        str += ("type = \"journal\"\n"
                + "callnumber = \"" + this.getCallNum() + "\"\n"
                + "title = \"" + this.getTitle() + "\"\n");
        if(!this.getOrganization().equals("")){
            str += ("organization = \"" + this.getOrganization() + "\"\n");
        } 
        str += ("year = \"" + this.getYear() + "\"\n");
        str = str.replaceAll("\n",System.getProperty("line.separator"));
        return str;
    }
}
