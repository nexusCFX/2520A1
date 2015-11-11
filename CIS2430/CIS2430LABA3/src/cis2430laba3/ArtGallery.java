/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cis2430laba3;

import java.util.ArrayList;

/**
 *
 * @author Harshavardhan
 * This concrete class implements methods specific to itself.
 * Since only ArtGalleries in the problem can buy paintings from other ArtGalleries,
 * and since PaintingGalleries cannot, we include that method here since it is 
 * specific to only ArtGalleries.
 * 
 * This class implements IAddSculpture because only the ArtGallery has sculptures.
 */
public class ArtGallery extends AbstractArtGallery implements IAddSculpture{
    
    private int availableFunds;
    
    public ArtGallery(int id, String name, int availableFunds) {
        this.id = id;
        this.name = name;
        this.availableFunds = availableFunds;
    }
    
    public int getId() {
        return id;
    }
    
    public int getAvailableFunds() {
        return availableFunds;
    }
    
    public void setAvailableFunds(int newFunds) {
        availableFunds = newFunds;
    }
    
    
    public String getName() {
        return name;
    }
    
    public ArrayList<Painting> ggetPaintings() {
        return paintings;
    }
    
    public ArrayList<Sculpture> getSculptures() {
        return sculptures;
    }
     
    
    @Override
    public void addSculpture(String fileName, Sculpture newSculpture) {
        storeSculpture();
        catalogItem(fileName, newSculpture.getName(), newSculpture.getSculptorName());
    }
    
    @Override
    public void listPaintings() {
        for (Painting painting : paintings) {
            System.out.println(painting.getName());
        }
    }

    public void storeSculpture() {
       //write codes to add sculpture object to ArrayList of sculptures 
    }
    
    @Override
    public void listSculptures() {
        //write codes to list sculptures in the gallery
    }
    
    public void buy(ArtGallery source, String paintingName) {
        
        //write code to transfer painting from source to destination.
        //Do this by searching the painting in the source list. If found,
        //fetch its price and check if destination has funds available to buy
        //Do defensive programming as usual
        //once painting has been transferred from source to destination
        //decrement destination's funds and increment source's funds
        //by amount equal to the price of the painting
        
        
       //begin for loop 
            
            
        //find painting    
           
                //painting found. now check if destination has funds to buy
        //check if funds are available to buy     
                    
                    //destination has funds. 
                    //now transfer painting from source to destination 
                    
        //add painting to destination
             
        //remove from source gallery          
                    
        //decrement funds of destination by amount equal to price of painting
                    
        //increment funds of destination by amount equal to price of painting
                    
                    
                    //transaction complete
                
            
        
        
    }
    
}
