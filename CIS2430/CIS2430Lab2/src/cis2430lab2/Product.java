
package cis2430lab2;

public class Product {
    private int ID;
    private String name;
    
    public Product(){   
    }
    
    public Product(int initID, String initName){
        ID = initID;
        name = initName;
    }
    
    public String getName () {
        return name;
    }
    
    public int getID () {
        return ID;
    }
}
