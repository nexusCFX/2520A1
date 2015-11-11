
package cis2430lab2;

public class Employee {
    private int ID;
    private String name;
    
    public Employee(){   
    }
    
    public Employee(int initID, String initName){
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
