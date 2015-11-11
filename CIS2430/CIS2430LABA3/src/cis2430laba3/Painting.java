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
 * Each painting has a name, its painter's name and a price
 */
public class Painting {
    private String name;
    private String painterName;
    private int price;
    
    public Painting(String painterName, String paintingName, int price) {
        
        this.name = paintingName;
        this.painterName = painterName;
        this.price = price;
    }
    
    public String getName() {
        return name;
    }
    
    public int getPrice() {
        return price;
    }
    
    public String getPainterName() {
        return painterName;
    }
}
