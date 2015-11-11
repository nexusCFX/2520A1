/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cis2430laba3;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayList;

/**
 *
 * @author Harshavardhan
 */
public class CIS2430LABA3 {
    
    public static void main(String args[]) {
        
        ArrayList<ArtGallery> artGalleries = new ArrayList();
        ArrayList<PaintingGallery> paintingGalleries = new ArrayList();
        
        
        //Begin by creating two ArrayLists. 
        //One will hold two ArtGallery objects.
        //Assign them IDs, Names and Funds of 0,NY,2000
        //and 1,Toronto,5000 respectively.
        
         //The second ArrayList will hold two PaintingGallery objects.
        //Assign them IDs and Names of 2, Toronto
        //and 3, Seattle respectively.
        
        BufferedReader scan = new BufferedReader(new InputStreamReader(System.in));
        int galleryChoice = 0;
        int opChoice = 0;
        
        while(galleryChoice != 4) {
            
            opChoice = 0;
            
            //print first menu
            System.out.println("\nSelect an art gallery or press 4 to exit: "
                    + "\n0. NY" 
                    + "\n1. Toronto"
                    + "\n2. Vancouver"
                    + "\n2. Seattle");
            
            try {
                galleryChoice = Integer.parseInt(scan.readLine());
                
                //exit if user selects 4
                if(galleryChoice == 4) {
                    System.exit(0);
                }
                
                //print message if invalid user choice
                
                while(opChoice != 7) {
                    
                    //show second menu
                    System.out.println("\nSelect an operation: "
                        + "\n1. List Paintings"
                        + "\n2. Add Paintings" 
                        + "\n3. List Sculptures"        //show this only for ArtGalleries, not PaintingGalleries 
                        + "\n4. Add Sculptures"         //show this only for ArtGalleries, not PaintingGalleries 
                        + "\n5. Buy Paintings"          //show this only for ArtGalleries, not PaintingGalleries 
                        + "\n6. Back to gallery selection");

                    opChoice = Integer.parseInt(scan.readLine());
            
                    switch(opChoice) {
                        case 1: 
                            
                            //list paintings from selected gallery
                            //for example 
                            artGalleries.get(galleryChoice).listPaintings();
                            break;

                        case 2: 
                            
                            //Scan painter's name, painting's name and price.
                            //add the painting to selected gallery
                           
                            break;

                        case 3: 
                           
                            break;
                            
                        case 5: 
                            
                            //This case is for buying paintings from another ArtGallery
                            
                            //scan source gallery's id (refer to IDs in the menu)
                            
                            //scan painting name
                                                       
                            //execute buy method in the artGallery.
                            //params to buy method are source ArtGallery Object and painting name
                            
                             break;
                            
                        case 7: 
                            break;

                    }

                }
            
            } catch (Exception ex) {
                
            }
            
        }
    }
    
}
