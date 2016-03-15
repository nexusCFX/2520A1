#!/usr/bin/python3
import os
import Cal
import tkinter as tki
from tkinter import *
from tkinter.messagebox import *
from tkinter.filedialog import askopenfilename
from tkinter.filedialog import asksaveasfilename
from tkinter.ttk import * 
import random

class XCalGUI:
    def __init__(self):
       
        if os.getenv("DATEMSK", "-1") == "-1":
            top = Tk()
            def tyrone():
              self.getDateMsk()
              top.destroy()
            top.minsize(width=270, height=60)
            top.maxsize(width=270, height=60)
            L1 = Label(top, text="You do not have a Datemsk file specified.\nWould you like to enter it now?")
            L1.grid(row=0, column=0, sticky='n')
            showB = Button(top,text="Yes",command = tyrone)
                
            showB.grid(row=2, column = 0, sticky='e')
            kfc = Button(top,text="Not now",command = top.destroy)
            kfc.grid(row=2, column = 0, sticky='w')

                
            mainloop()
        
        self.filename = ""
        self.fileList = []
        self.pcal = 0
        self.root=Tk()
        self.root.title("xcal")
        self.root.minsize(width=700, height=666)
        
        self.CompTable = Treeview(self.root, selectmode="browse")
        self.CompTable.bind("<Button-3>", self.popup)
        
        self.CompTable.configure(columns = ('Name', 'Props', 'Subs', 'Summary'))
        
        self.CompTable.heading('#0', text='No.', anchor='w')
        self.CompTable.column('#0', anchor='w', width=5)
        
        self.CompTable.heading('Name', text='Name')
        self.CompTable.column('Name', anchor='center', width=100)
        
        self.CompTable.heading('Props', text='Props')
        self.CompTable.column('Props', anchor='center', width=10)
        
        self.CompTable.heading('Subs', text='Subs')
        self.CompTable.column('Subs', anchor='center', width=10)
        
        self.CompTable.heading('Summary', text='Summary')
        self.CompTable.column('Summary', anchor='center', width=200)
        
        self.CompTable.grid(sticky = (N,E,W,S))
        self.root.treeview = self.CompTable
        
        scrollbT = Scrollbar(self.root, command=self.CompTable.yview)
        scrollbT.grid(row=0, column=1, sticky='nsew')
        self.CompTable['yscrollcommand'] = scrollbT.set
        
        
        showB = Button(self.root,text="Show Selected",command = lambda: self.showSelected())
        showB.grid(row=1, column=0, sticky='w')
        
        extractEB = Button(self.root,text="Extract Events",command = lambda: self.clearTable())
        extractEB.grid(row=1, column=0, sticky='e')

        extractXB = Button(self.root,text="Extract X-Props",command = lambda: clearTable(self.CompTable))
        extractXB.grid(row=1, column=0)

    # create a Text widget
        self.log = Text(self.root, borderwidth=2, state="disabled")
        self.log.config(font=("consolas", 12), undo=True, wrap='word', state=DISABLED)
        self.log.grid(row=4, column=0, sticky=(E,W,S), padx=2, pady=2)

    # create a Scrollbar and associate it with self.log
        scrollb = Scrollbar(self.root, orient=tki.VERTICAL, command=self.log.yview)
        scrollb.grid(row=4, column=1, sticky='nsew')
        self.log['yscrollcommand'] = scrollb.set
       
        #Calls main Window Menu along with a .configure -> Shows the menu
        m = Menu()
        #"File"
        self.fileMenu = Menu(m, tearoff=0)
        self.fileMenu.add("command", label="Open", command=self.openFile)
        self.fileMenu.add("command", label="Save", command=self.saveCurrent)
        self.fileMenu.add("command", label="SaveAs", command=self.saveFileAs)
        self.fileMenu.add("command", label="Combine", command=self.combine)
        self.fileMenu.add("command", label="Filter", command=self.filter)
        self.fileMenu.add("command", label="Exit", command=self.quitProg)
        
        #"Help"
        self.todoMenu = Menu(m, tearoff=0)
        #Tearoff, run this program, Then drag the dotted line in the help menu. Very cool! =D
        self.todoMenu.add("command", label="To-do List", command=self.aboutWindow)
        self.todoMenu.add("command", label="About xcal", command=self.aboutWindow)
        self.helpMenu = Menu(m, tearoff=0)
        self.helpMenu.add("command", label="Date Mask", command=self.aboutWindow)
        self.helpMenu.add("command", label="About xcal", command=self.aboutWindow)
        
        #Adds to menu
        m.add("cascade",menu=self.fileMenu, label="File")
        m.add("cascade",menu=self.todoMenu, label="Todo")
        m.add("cascade",menu=self.helpMenu, label="Help")
        self.root.configure(menu=m)
        
        self.root.grid_rowconfigure(0, weight = 1)
        self.root.grid_columnconfigure(0, weight = 1)
        
    def filter(self):
        result = []
        
    def popup(self, event):
        self.CompTable.selection_toggle(self.CompTable.focus)
    # select row under mouse
       # test = self.CompTable.focus()
        #iid = self.CompTable.identify_row(event.y)
        #if iid:
         #   if iid != test:
          #      print("kfcs")
                # mouse pointer over item
                #self.CompTable.selection_set(iid)         
          #  else:
           #     print("kfc")
            #    self.CompTable.focus(None)
                
        
    def dateMskStartup(self, top):
        self.getDateMsk   
        top.destroy
        
    def getDateMsk(self):
        #print()
        datemskloc = askopenfilename()
        os.environ["DATEMSK"] = datemskloc
       # command = "env DATEMSK={loc}".format(loc=datemskloc)
        #os.system(command)

    def combine():
        combineFile = getFileName
    
    def openFile(self):
        self.filename = askopenfilename()
        result = []
        Cal.readFile(self.filename, result)
        self.pcal = result[0]
        self.fileList = result[1]
        i = 1
        for tup in self.fileList:
            self.CompTable.insert('', 'end', text=str(i), values=(tup[0], tup[1], tup[2], tup[3]))
            i = i + 1
        
    def saveCurrent():
        quitProg
        
    def saveFile(self):
        print ()
        
    def showSelected(self):
        tempFile = "ShowSelectedTemp.txt"
        select = self.CompTable.item(self.CompTable.focus())
        Cal.writeFile(tempFile, self.pcal, int(select['text'])-1)
        f = open(tempFile, "r")
        loadedFile = f.read()
      #  print(loadedFile)
        self.log.config(state=NORMAL)
        self.log.insert("end", loadedFile)
        self.log.config(state=DISABLED)

    def saveFileAs(self):
        print (self.pcal)
        outputFile = asksaveasfilename(defaultextension=".ics")
        Cal.writeFile(outputFile, self.pcal, self.fileList)

    def aboutWindow():
            #showinfo pops up an entire new window. Alert in Java. 
            showinfo("About", "xcal\nCompatible with iCalendar V2.0\nWritten by Brandon Chester")
            
    def clearTable(self):
        command = "./caltool -info < {nameofile} > pytest.self.log".format(nameofile=self.filename)
        os.system(command)
        #print (command)
    # for i in self.CompTable.get_children():
    #     self.CompTable.delete(i)
            
    #Quit program function
    def quitProg():
        print ("Goodbye!")
        xCal.quit()
        

xCal = XCalGUI()
    
mainloop()