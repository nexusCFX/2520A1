/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package CIS2430A3;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;

public class AppFrame extends JFrame {
    private JPanel card1;
    
    public AppFrame() {
        super();
        card1 = new JPanel();
        card1.add(new JButton("Button 1"));
        this.add(card1);
        setTitle("Library Search");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);//DO_NOTHING_ON_CLOSE
        setSize(1200, 400); //1370, 730 for fullscreen for laptop
        this.setResizable(true);
        setLocationRelativeTo(null);
        setVisible(true);
    }
    
    public void addPanel(JPanel panel){
        this.add(panel);
    }
}
