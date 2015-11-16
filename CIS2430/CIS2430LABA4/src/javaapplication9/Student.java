/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javaapplication9;

import java.util.Objects;

/**
 *
 * @author Brandy
 */
public class Student {
    private String firstName;
    private String lastName;
    private String address;
    
    public Student() {
    }
    
    public Student(String firstName, String lastName, String address) {
        this.firstName = firstName;
        this.lastName = lastName;
        this.address = address;
    }
    
    public String getFirstName(){
        return this.firstName;
    }
    
    public String getLastName(){
        return this.lastName;
    }
    
    public String getAddress(){
        return this.address;
    }
    
    @Override
    public String toString(){
        return (this.getFirstName() + " " + this.getLastName() + " " + this.getAddress() + System.getProperty("line.separator"));
    }
    
    @Override
    public boolean equals(Object other) {
        if (other == null) {
            return false;
        }
        if (other == this) {
            return true;
        }
        if (!(other instanceof Student)) {
            return false;
        }
        return this.toString().equals(((Student)other).toString());
    }

    @Override
    public int hashCode() {
        int hash = 5;
        hash = 31 * hash + Objects.hashCode(this.firstName);
        hash = 31 * hash + Objects.hashCode(this.lastName);
        hash = 31 * hash + Objects.hashCode(this.address);
        return hash;
    }
}
