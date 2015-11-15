package CIS2430A2;

import java.util.Objects;

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
        str += ("type = \"book\"" + System.getProperty("line.separator")
                + "callnumber = \"" + this.getCallNum() + "\"" + System.getProperty("line.separator"));
        if(!this.getAuthor().equals("")){
            str += ("authors = \"" + this.getAuthor() + "\"" + System.getProperty("line.separator"));
        }
        str += ("title = \"" + this.getTitle() + "\"" + System.getProperty("line.separator"));
        if(!this.getPublisher().equals("")){
            str += ("publisher = \"" + this.getPublisher() + "\"" + System.getProperty("line.separator"));
        } 
        str += ("year = \"" + this.getYear() + "\"" + System.getProperty("line.separator"));
        return str;
    }
    
    /**
     * Checks if an object is equal to a given book
     *
     * @param other The object to be checked against a book object.
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
        if (!(other instanceof Book)) {
            return false;
        }
        Book b = (Book) other;
        return b.getTitle().equalsIgnoreCase(this.getTitle())
                && b.getCallNum().equalsIgnoreCase(this.getCallNum())
                && b.getAuthor().equalsIgnoreCase(this.getAuthor())
                && b.getPublisher().equalsIgnoreCase(this.getPublisher())
                && b.getYear() == this.getYear();
    }

     /**
     * Checks the hashcode of a book
     *@return The hashcode of the book
     */
    @Override
    public int hashCode() {
        int hash = 7;
        hash = 83 * hash + Objects.hashCode(this.author);
        hash = 83 * hash + Objects.hashCode(this.publisher);
        return hash;
    }
}
