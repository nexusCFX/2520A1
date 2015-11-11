/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cis2430laba3;

/**
 *
 * @author Harshavardhan
 * Painting class for the paintings
 * Each sculpture has a name and its sculptor's name
 */
public class Sculpture {
    
    private String name;
    private String sculptorName;
    
    public Sculpture(String name, String sculptorName) {
        
        this.name = name;
        this.sculptorName = sculptorName;
    } 
    
    public String getName() {
        return name;
    }
    
    public String getSculptorName() {
        return sculptorName;
    }
}
