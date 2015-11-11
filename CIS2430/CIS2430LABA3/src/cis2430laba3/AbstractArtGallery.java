/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cis2430laba3;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Harshavardhan
 * This is the AbstractArtGallery class that implements the IAddPainting interface.
 * An Abstract class is used to act as a reference for classes that extend it.
 * Unlike an interface, an abstract class can provide partial implementation, i.e
 * it can implement some methods that are common to all classes that extend it.
 * We have 3 common methods here.
 */
public abstract class AbstractArtGallery implements IAddPainting {
    
    protected ArrayList<Painting> paintings = new ArrayList<>();
    protected ArrayList<Sculpture> sculptures = new ArrayList<>();
    
    protected String name;
    protected int id;

    /*listPaintings() is an abstract method with no implementation here*/
    //both ArtGallery and PaintingGallery list their paintings in a different way.
    @Override
    public abstract void listPaintings(); 
    
       
    @Override
    public void addPainting(String fileName, Painting newPainting) {
        //store a new painting in the ArrayList of paintings
        storePainting(newPainting); 
        //write item to disk (in a .txt file)
        catalogItem(fileName, newPainting.getName(), newPainting.getPainterName());
    }
    
     
    public void storePainting(Painting newPainting) {
        //write code to add newPainting to the ArrayList paintings
    }

    
    @Override
    public void catalogItem(String fileName, String itemName, String itemCreatorName) {

        //Use the PrintWriter to get the familiar println syntax
        //Use FileWriter to write to file
        //Use BufferedWriter to improve write performance by delayed buffered writes
        PrintWriter catalog;
        try {
            catalog = new PrintWriter(new BufferedWriter(new FileWriter(fileName, true)));
            
            //write code to print info to the file in the format:
            //<ItemName> by <itemCreator> for <Price>
            
            catalog.close();
        } catch (IOException ex) {
            Logger.getLogger(AbstractArtGallery.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        
    
    }
    
    
}
