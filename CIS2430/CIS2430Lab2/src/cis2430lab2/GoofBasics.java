
package cis2430lab2;

import java.util.ArrayList;
import java.util.Iterator;


public class GoofBasics {
    private String storeName;
    private int storeID;
    private ArrayList<Product> productList = new ArrayList<>();
    private ArrayList<Employee> employeeList = new ArrayList<>();
    
    public GoofBasics(){   
    }
    
    public GoofBasics(int initID, String initName){
        storeID = initID;
        storeName = initName;
    }
    
    public void listProducts () {
        Iterator<Product> printProducts = productList.iterator();
        System.out.println(productList.size() + " products found.");
        while(printProducts.hasNext()) {
            Product print = printProducts.next();
            System.out.println(print.getID() + " : " + print.getName());
            
        }
    }
    
    public void addProducts (int ID, String name) {
        productList.add(new Product(ID, name));
    }
    
    public boolean removeProducts (int ID) {
        Iterator<Product> removeProducts = productList.iterator();
        while(removeProducts.hasNext()) {
            Product next = removeProducts.next();
            if (next.getID() == ID) {
                productList.remove(next);
                return true;
            }
        }
        return false;
    }
    
    public void listEmployees () {
        Iterator<Employee> printEmployees = employeeList.iterator();
        System.out.println(employeeList.size() + " employees found.");
        while(printEmployees.hasNext()) {
            Employee print = printEmployees.next();
            System.out.println(print.getID() + " : " + print.getName());
            
        }
    }
    
    public void addEmployees (int ID, String name) {
        employeeList.add(new Employee(ID, name));
    }
    
    public boolean removeEmployees (int ID) {
        Iterator<Employee> removeEmployees = employeeList.iterator();
        while(removeEmployees.hasNext()) {
            Employee next = removeEmployees.next();
            if (next.getID() == ID) {
                employeeList.remove(next);
                return true;
            }
        }
        return false;
    }
}
