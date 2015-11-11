package CIS2430A1;

/**
 * A book. Parent class: Reference.
 * @author Brandon Chester
 * @version 1.0
 * @since 2015-10-16
 */
public class Book extends Reference {

    private String author;
    private String publisher;

    public Book() {
    }

    /**
     * Initializes a book
     * @param author The author of the book
     * @param title The title of the book
     * @param callNum The call number of the book
     * @param publisher The publisher of the book
     * @param year The publication year of the book
     */
    public Book(String author, String callNum, String title, String publisher, int year) {
        super(callNum, title, year);
        this.author = author;
        this.publisher = publisher;
    }

    /**
     * Gets the author of a book
     * @return The author of the book
     */
    public String getAuthor() {
        return this.author;
    }

    /**
     * Gets the publisher of a book
     * @return The publisher of the book
     */
    public String getPublisher() {
        return this.publisher;
    }
    
    /**
     * Prints the attributes of a book
     * @return A string with the attributes of the book
     */
    @Override
    public String toString(){
        String str = "";
        str += ("type = \"book\"\n"
                + "callnumber = \"" + this.getCallNum() + "\"\n");
        if(!this.getAuthor().equals("")){
            str += ("authors = \"" + this.getAuthor() + "\"\n");
        }
        str += ("title = \"" + this.getTitle() + "\"\n");
        if(!this.getPublisher().equals("")){
            str += ("publisher = \"" + this.getPublisher() + "\"\n");
        } 
        str += ("year = \"" + this.getYear() + "\"\n");
        str = str.replaceAll("\n",System.getProperty("line.separator"));
        return str;
    }
}
