/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cis2430laba3;

/**
 *
 * @author Harshavardhan
 */
public interface IAddPainting {
    void addPainting(String fileName, Painting newPainting);
    void catalogItem(String fileName, String itemName, String itemCreatorName);
    void listPaintings();
}
