#!/usr/bin/python3
import copy
import os
import Cal
import tkinter as tk
from tkinter import *
from tkinter.messagebox import *
from tkinter.filedialog import askopenfilename
from tkinter.filedialog import asksaveasfilename
from tkinter.ttk import * 
import random

class XCalGUI:
    def __init__(self):
    
        self.filename = ""
        self.fileList = []
        self.oldList = []
        self.pcal = 0
        self.root=Tk()
        self.root.title("xcal")
        self.root.minsize(width=700, height=666)
        self.tableSize = 0
        
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
         
        self.showSelButn = Button(self.root,text="Show Selected",command = lambda: self.showSelected())
        self.showSelButn.grid(row=1, column=0, sticky='w')
        
        self.extrEvntBtn = Button(self.root,text="Extract Events",command = lambda: self.clearTable())
        self.extrEvntBtn.grid(row=1, column=0, sticky='e')

        self.extrXPropBtn = Button(self.root,text="Extract X-Props",command = lambda: clearTable(self.CompTable))
        self.extrXPropBtn.grid(row=1, column=0)
        
    # create a Text widget
        self.log = Text(self.root, borderwidth=2, state="disabled")
        self.log.config(font=("consolas", 12), undo=True, wrap='word', state=DISABLED)
        self.log.grid(row=4, column=0, sticky=(E,W,S), padx=2, pady=2)

    # create a Scrollbar and associate it with self.log
        scrollb = Scrollbar(self.root, orient=tk.VERTICAL, command=self.log.yview)
        scrollb.grid(row=4, column=1, sticky='nsew')
        self.log['yscrollcommand'] = scrollb.set
        
        self.clrLogBtn = Button(self.root,text="Clear log",command = self.clearLog)
        self.clrLogBtn.grid(row = 5, column = 0)
       
        #Calls main Window Menu along with a .configure -> Shows the menu
        self.menuBar = Menu()
        #"File"
        self.fileMenu = Menu(self.menuBar, tearoff=0)
        self.fileMenu.add("command", label="Open", command=self.openFile)
        self.fileMenu.add("command", label="Save", command=self.saveFile)
        self.fileMenu.add("command", label="SaveAs", command=self.saveFileAs)
        self.fileMenu.add("command", label="Combine", command=self.combine)
        self.fileMenu.add("command", label="Filter", command=self.filter)
        self.fileMenu.add("command", label="Exit", command=self.quitProg)
        
        #"Help"
        self.todoMenu = Menu(self.menuBar, tearoff=0)
        #Tearoff, run this program, Then drag the dotted line in the help menu. Very cool! =D
        self.todoMenu.add("command", label="To-do List", command=self.todoList)
        self.todoMenu.add("command", label="About xcal", command=self.aboutWindow)
        
        self.helpMenu = Menu(self.menuBar, tearoff=0)
        self.helpMenu.add("command", label="Date Mask", command=self.aboutWindow)
        self.helpMenu.add("command", label="About xcal", command=self.aboutWindow)
        
        #Adds to menu
        self.menuBar.add("cascade",menu=self.fileMenu, label="File")
        self.menuBar.add("cascade",menu=self.todoMenu, label="Todo")
        self.menuBar.add("cascade",menu=self.helpMenu, label="Help")
        self.root.configure(menu=self.menuBar)
        
        self.root.grid_rowconfigure(0, weight = 1)
        self.root.grid_columnconfigure(0, weight = 1)
        
        self.root.bind_all("<Control-o>", self.openSC)
        self.root.bind_all("<Control-s>", self.saveSC)
        self.root.bind_all("<Control-x>", self.quitSC)
        
        self.root.bind_all("<Control-t>", self.todoSC)
       # self.root.bind_all("<Control-z>", self.undoSC)
        
        
        if os.getenv("DATEMSK", "-1") == "-1":
            top = Toplevel(self.root)
            top.attributes("-topmost", True)
            top.lift()
            def inputDateMsk():
              self.getDateMsk()
              top.destroy()
            top.minsize(width=270, height=60)
            top.maxsize(width=270, height=60)
            L1 = Label(top, text="You do not have a Datemsk file specified.\nWould you like to enter it now?")
            L1.grid(row=0, column=0, sticky='n')
            yesBtn = Button(top,text="Yes",command = inputDateMsk)
                
            yesBtn.grid(row=2, column = 0, sticky='e')
            noBtn = Button(top,text="Not now",command = top.destroy)
            noBtn.grid(row=2, column = 0, sticky='w')
            
    
        
    def clearLog(self):
        self.log.config(state = NORMAL)
        self.log.delete('1.0', END)
        self.log.config(state = DISABLED)
        
    def unsavedChanges(self):
        print("yo")
        
    def todoSC(self, event):
        self.todoList()
        
    def todoList(self):
        def reconfigure(canvas):
            canvas.configure(scrollregion=canvas.bbox("all"))
            
        toDo = tk.Toplevel()
        
        def Done():
            removed = 0
            self.oldList = list(self.fileList)
            for box in boxList:
                print("box var")
                print(box.var.get())
                if box.var.get() == 1:
                    print("Rem")
                    del self.fileList[box.index - removed]
                    removed = removed + 1
                    
            self.tableSize = 0
            for i in self.CompTable.get_children():
                self.CompTable.delete(i)
            print(self.fileList)
            print("old")
            print(self.oldList)
            self.drawFVP()
            toDo.destroy() 
            
        def Cancel():
            toDo.destroy()
        
        toDo.minsize(width=240, height=250)
        toDo.maxsize(width=240, height=10000)
        canvas = tk.Canvas(toDo, borderwidth=0, background="#ffffff")
        frame = tk.Frame(canvas, background="#ffffff")
        vsb = tk.Scrollbar(toDo, orient="vertical", command=canvas.yview)
        canvas.configure(yscrollcommand=vsb.set)

        vsb.pack(side="right", fill="y")
        canvas.pack(fill="both", expand=True)
        canvas.create_window(0,0, window=frame, anchor="nw")

        frame.bind("<Configure>", lambda event, canvas=canvas: reconfigure(canvas))
        canvas.bind('<4>', lambda event : canvas.yview('scroll', -1, 'units'))
        canvas.bind('<5>', lambda event : canvas.yview('scroll', 1, 'units'))
        
        i = 0
        
        boxList = []
        j = 0
        for tup in self.fileList:
           
            if (tup[0] == "VTODO"):
                
                Lbl = Label(frame, text=tup[3])
                Lbl.grid(row = i, column = 0)
                Var = IntVar()
                Var.set(0)
                ChkBox = Checkbutton(frame, variable=Var)
                ChkBox.var = Var
                
                ChkBox.grid(row = i, column = 1)
                ChkBox.index = j
                boxList.append(ChkBox)
                i = i + 1
            j = j + 1

        btn = Button(toDo,text="Cancel",command = Cancel)
        btn.pack(fill="both")
        btn2 = Button(toDo,text="Save",command = Done)
        btn2.pack(fill="both")

        mainloop()
        
    def openSC(self, event):
        self.openFile()
        
    def openFile(self):
        self.filename = askopenfilename()
        if (self.filename):
            result = []
            Cal.readFile(self.filename, result)
            self.pcal = result[0]
            self.fileList = result[1]
            self.drawFVP()
            self.root.title("xcal " + os.path.basename(self.filename))
            
    def drawFVP(self):   
        for tup in self.fileList:
            self.tableSize = self.tableSize + 1
            self.CompTable.insert('', 'end', iid = self.tableSize, text=str(self.tableSize), values=(tup[0], tup[1], tup[2], tup[3]))
            
    def saveSC(self, event):
        self.saveFile(self)
        
    def saveFile(self):
        Cal.writeFile(outputFile, self.pcal, self.fileList)
        self.root.title("xcal " + os.path.basename(self.filename))
            
    def saveFileAs(self):
        outputFile = asksaveasfilename(defaultextension=".ics")
        if (outputFile):
            Cal.writeFile(outputFile, self.pcal, self.fileList)
        
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
        datemskloc = askopenfilename()
        if(datemskloc):
            os.environ["DATEMSK"] = datemskloc
       # command = "env DATEMSK={loc}".format(loc=datemskloc)
        #os.system(command)

    def combine():
        combineFile = getFileName
        
    def showSelected(self):
        tempFile = "ShowSelectedTemp.txt"
        item = self.CompTable.focus()
        if (item):
            select = self.CompTable.item(self.CompTable.focus())
            if (select):
                Cal.writeFile(tempFile, self.pcal, int(select['text'])-1)
                f = open(tempFile, "r")
                loadedFile = f.read()
            #  print(loadedFile)
                self.log.config(state=NORMAL)
                self.log.insert("end", loadedFile)
                self.log.config(state=DISABLED)

    def aboutWindow(self):
        #showinfo pops up an entire new window. Alert in Java. 
        showinfo("About", "xcal\nCompatible with iCalendar V2.0\nWritten by Brandon Chester")
        
    def quitSC(self, event):
        self.quitProg(self)
            
    def quitProg(self):
        xCal.quit()
        
xCal = XCalGUI()
    
mainloop()