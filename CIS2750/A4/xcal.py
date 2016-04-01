#!/usr/bin/python3
import time
import getpass
import mysql.connector
import copy
import os
import os.path
import Cal
import sys
import tkinter as tk
from tkinter import *
from tkinter.messagebox import *
from tkinter.filedialog import askopenfilename
from tkinter.filedialog import asksaveasfilename
from tkinter.ttk import *
import random

#############################
#xcal.py
#Python program that provides a GUI interface for
#functions of caltool command line program
#
#Author: Brandon Chester : 0877477
#Contact: bchester@mail.uoguelph.ca
#Created: Mar 11, 2016
#Last modified: Mar 18, 2016
#############################

class XCalGUI:
    def __init__(self): 
        self.filename = ""
        self.unsavedChanges = 0
        self.fileList = []
        self.oldList = []
        self.pcal = 0
        self.root = Tk()
        self.root.title("xcal")
        self.root.minsize(width = 700, height = 666)
        self.tableSize = 0
        self.rowSelected = 0
        self.numBoxes = 0
        
        self.CompTable = Treeview(self.root, selectmode = "none")
        self.CompTable.bind("<Button-3>", self.popup)
        self.CompTable.configure(columns = ('Name', 'Props', 'Subs', 'Summary'))
        self.CompTable.heading('#0', text = 'No.', anchor = 'w')
        self.CompTable.column('#0', anchor = 'w', width = 5)
        self.CompTable.heading('Name', text = 'Name')
        self.CompTable.column('Name', anchor = 'center', width = 100)
        self.CompTable.heading('Props', text = 'Props')
        self.CompTable.column('Props', anchor = 'center', width = 10)
        self.CompTable.heading('Subs', text = 'Subs')
        self.CompTable.column('Subs', anchor = 'center', width = 10)
        self.CompTable.heading('Summary', text = 'Summary')
        self.CompTable.column('Summary', anchor = 'center', width = 200)
        self.CompTable.grid(sticky = (N, E, W, S))

        self.root.treeview = self.CompTable
        
        scrollbT = Scrollbar(self.root, command = self.CompTable.yview)
        scrollbT.grid(row = 0, column = 1, sticky = 'nsew')
        
        self.CompTable['yscrollcommand'] = scrollbT.set
        self.CompTable.bind("<Button-1>",self.popup)
        
        self.showSelButn = Button(self.root, text = "Show Selected", state=DISABLED, command = self.showSelected)
        self.showSelButn.grid(row = 1, column = 0, sticky = 'w')
        
        self.extrEvntBtn = Button(self.root, text = "Extract Events", state=DISABLED, command = self.extractEvents)
        self.extrEvntBtn.grid(row = 1, column = 0, sticky = 'e')
        
        self.extrXPropBtn = Button(self.root, text = "Extract X-Props", state=DISABLED, command = self.extractXProps)
        self.extrXPropBtn.grid(row = 1, column = 0)

        # create a Text widget
        self.log = Text(self.root, borderwidth = 2, state = "disabled")
        self.log.config(font = ("consolas", 12), undo = True, wrap = 'word', state = DISABLED)
        self.log.grid(row = 4, column = 0, sticky = (E, W, S), padx = 2, pady = 2)

        # create a Scrollbar and associate it with self.log 
        scrollb = Scrollbar(self.root, orient = tk.VERTICAL, command = self.log.yview)
        scrollb.grid(row = 4, column = 1, sticky = 'nsew')

        self.log['yscrollcommand'] = scrollb.set
        self.clrLogBtn = Button(self.root, text = "Clear log", command = self.clearLog)
        self.clrLogBtn.grid(row = 5, column = 0)

        self.menuBar = Menu()

        self.fileMenu = Menu(self.menuBar, tearoff = 0)
        self.fileMenu.add_command(label = "Open", command = self.openFileDLG)
        self.fileMenu.add_command( label = "Save", command = self.saveFile)
        self.fileMenu.add_command( label = "Save As", command = self.saveFileAs)
        self.fileMenu.add_command( label = "Combine", command = self.combine)
        self.fileMenu.add_command( label = "Filter", command = self.filter)
        self.fileMenu.add_command( label = "Exit", command = self.quitProg)
        
        self.fileMenu.entryconfig(1, state=DISABLED)
        self.fileMenu.entryconfig(2, state=DISABLED)
        self.fileMenu.entryconfig(3, state=DISABLED)
        self.fileMenu.entryconfig(4, state=DISABLED)
        
        
        self.todoMenu = Menu(self.menuBar, tearoff = 0)
        self.todoMenu.add_command(label = "To-do List", command = self.todoList)
        self.todoMenu.add_command( label = "Undo", command = self.undoCHK)
        
        self.todoMenu.entryconfig(0, state=DISABLED)
        self.todoMenu.entryconfig(1, state=DISABLED)

        self.helpMenu = Menu(self.menuBar, tearoff = 0)
        self.helpMenu.add_command( label = "Date Mask", command = self.getDateMsk)
        self.helpMenu.add_command( label = "About xcal", command = self.aboutWindow)
        
        
        self.dbMenu = Menu(self.menuBar, tearoff = 0)
        self.dbMenu.add_command(label = "Store All", command = self.storeAll)
        self.dbMenu.add_command(label = "Store Selected", command = self.storeSelected)
        self.dbMenu.add_command(label = "Clear", command = self.clearDB)
        self.dbMenu.add_command(label = "Status", command = self.statusDB)
        self.dbMenu.add_command(label = "Query", command = self.queryDB)

        self.menuBar.add_cascade( menu = self.fileMenu, label = "File")
        self.menuBar.add_cascade( menu = self.todoMenu, label = "Todo")
        self.menuBar.add_cascade( menu = self.helpMenu, label = "Help")
        self.menuBar.add_cascade( menu = self.dbMenu, label = "Database")
        self.root.configure(menu = self.menuBar) 
        self.root.grid_rowconfigure(0, weight = 1)
        self.root.grid_columnconfigure(0, weight = 1)

        self.root.bind_all("<Control-o>", self.openSC)
        self.root.bind_all("<Control-s>", self.saveSC)
        self.root.bind_all("<Control-x>", self.quitSC)
        self.root.bind_all("<Control-t>", self.todoSC)
        self.root.bind_all("<Control-z>", self.undoSC)
       
        self.root.protocol("WM_DELETE_WINDOW", self.quitProg)

        if os.getenv("DATEMSK", "-1") == "-1":
            top = Toplevel()
            top.attributes("-topmost", True)
            top.lift()
           # top.grab_set()
            
            def inputDateMsk():
                top.destroy()
                self.getDateMsk()
                
            def Cancel2(event):
                top.destroy()
        
        
            top.bind_all("<Escape>", Cancel2)
                
            top.minsize(width = 270, height = 60)
            top.maxsize(width = 270, height = 60)
            L1 = Label(top, text = "You do not have a Datemsk file specified.\nWould you like to enter it now?")
            L1.grid(row = 0, column = 0, columnspan = 2, sticky = 'n')
            yesBtn = Button(top, text = "Yes", command = inputDateMsk)
            yesBtn.grid(row = 2, column = 0, columnspan = 1)
            noBtn = Button(top, text = "Not now", command = top.destroy)
            noBtn.grid(row = 2, column = 1, columnspan = 1)
            
            
        #if len(sys.argv) == 2:
        password = getpass.getpass('Password:')    
            
    def storeAll(self):
        print("ok")
        
    def storeSelected(self):
        print("ok")
          
    def clearDB(self):
        print("ok")
        
    def statusDB(self):
        print("ok")
            
    def queryDB(self):
        print("ok")
        
       
    def clearLog(self):
        self.log.config(state = NORMAL)
        
        self.log.delete('1.0', END)
        self.log.config(state = DISABLED)
        
    def todoSC(self, event):
        if (self.filename != ""):
            self.todoList()
        
    def todoList(self):
        def reconfigure(canvas):
            canvas.configure(scrollregion=canvas.bbox("all"))
            
        toDo = tk.Toplevel()
        toDo.title("Todo List")
        
        def Cancel2(event):
            toDo.destroy()
        
        
        toDo.bind_all("<Escape>", Cancel2)
        
        def Done():
            removed = 0
            self.oldList = list(self.fileList)
            for box in boxList:
                if box.var.get() == 1:
                    del self.fileList[box.index - removed]
                    removed = removed + 1
            if removed > 0:        
                self.root.title("xcal " + os.path.basename(self.filename) + "*")
                self.unsavedChanges = 1
                self.todoMenu.entryconfig(1, state=NORMAL)
                self.drawFVP()
            toDo.destroy() 
            
        def Cancel():
            toDo.destroy()
        
        toDo.minsize(width=240, height=350)
        toDo.maxsize(width=240, height=10000)
        canvas = tk.Canvas(toDo, borderwidth=0)
        frame = tk.Frame(canvas)
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
        
        btn2 = Button(toDo,text="Done",command = Done)
        btn2.config(state=DISABLED)
        def check():
            self.numBoxes = 0
            for box in boxList:
                if box.var.get() == 1:
                    self.numBoxes = self.numBoxes + 1
            
            if self.numBoxes > 0:
                btn2.config(state=NORMAL)
            else:
                btn2.config(state=DISABLED)
        
        
        for tup in self.fileList:
           
            if (tup[0] == "VTODO"):
                
                Lbl = Label(frame, text=tup[3])
                Lbl.grid(row = i, column = 0, sticky = W)
                Var = IntVar()
                Var.set(0)
                ChkBox = Checkbutton(frame, variable=Var, command = check)
                ChkBox.var = Var
                
                ChkBox.grid(row = i, column = 1, sticky = E)
                ChkBox.index = j
                boxList.append(ChkBox)
                i = i + 1
            j = j + 1

        btn = Button(toDo,text="Cancel",command = Cancel)
        btn.pack(fill="both")
        
        btn2.pack(fill="both")
        
        #btn2.config(state=DISABLED)
        mainloop()
        
    def openSC(self, event):
        self.openFileDLG()
       
        
    def undoCHK(self):
        if (self.unsavedChanges == 1):
            self.undo()
        
    def undoSC(self, event):
        if (self.unsavedChanges == 1):
            self.undo()
        
    def undo(self):
        dialog = Toplevel()
        
        dialog.title("Undo")
        dialog.attributes("-topmost", True)
        dialog.lift()
        dialog.grab_set()
        
        def Cancel():
            dialog.destroy()
            
        def Cancel2(event):
            dialog.destroy()
        
        
        dialog.bind_all("<Escape>", Cancel2)
            
        def Undo():
            dialog.destroy()
            self.fileList = self.oldList
            self.oldList = []
            self.drawFVP()
            self.unsavedChanges = 0
            self.todoMenu.entryconfig(1, state=DISABLED)
            self.root.title("xcal " + os.path.basename(self.filename))
            
        
        Label(dialog, text = "All changes since the last save will be undone").pack(fill = "x")
        Label(dialog, text = "Do you want to proceed?").pack(fill = "x")
        btn = Button(dialog, text = "Cancel", command = Cancel)
        btn2 = Button(dialog, text = "Undo", command = Undo)
        
        dialog.minsize(width = 300, height = 85)
        dialog.maxsize(width = 300, height = 85)

        btn.pack(fill = "both")
        btn2.pack(fill = "both") 
        
        
        
    def openFileDLG(self):
        if self.unsavedChanges == 1:
            dialog = Toplevel()
            dialog.title("Unsaved Changes")
            dialog.attributes("-topmost", True)
            dialog.lift()
            dialog.grab_set()
            
            def Cancel2(event):
                dialog.destroy()
        
            dialog.bind_all("<Escape>", Cancel2)
            
            def Cancel():
                dialog.destroy()
                
            def Continue():
                dialog.destroy()
                self.openFile()
                
            
            Label(dialog, text = "You have unsaved changes.").pack(fill = "x")
            Label(dialog, text = "Do you want to proceed?").pack(fill = "x")
            btn = Button(dialog, text = "Cancel", command = Cancel)
            btn2 = Button(dialog, text = "Continue", command = Continue)
            
            dialog.minsize(width = 250, height = 85)
            dialog.maxsize(width = 250, height = 85)

            btn.pack(fill = "both")
            btn2.pack(fill = "both")
            dialog.grab_set()
        else:
            self.openFile()
          
    def openFile(self):
        self.filename = ""
        self.root.title("xcal")
        if self.pcal != 0:
            Cal.freeFile(self.pcal)
            self.pcal = 0
        self.tableSize = 0
        for i in self.CompTable.get_children():
            self.CompTable.delete(i)
        self.oldList = []
        self.fileList = []
        self.fileMenu.entryconfig(1, state=DISABLED)
        self.fileMenu.entryconfig(2, state=DISABLED)
        self.fileMenu.entryconfig(3, state=DISABLED)
        self.fileMenu.entryconfig(4, state=DISABLED)
        self.todoMenu.entryconfig(0, state=DISABLED)
        self.extrEvntBtn.config(state=DISABLED)
        self.extrXPropBtn.config(state=DISABLED)
        self.showSelButn.config(state=DISABLED)
        self.filename = askopenfilename()
               
        if (self.filename):
            self.unsavedChanges = 0
            result = []
            error = Cal.readFile(self.filename, result)
            if error == "OK":
                if (self.pcal != 0):
                    Cal.freeFile(self.pcal)
                self.unsavedChanges = 0
                self.todoMenu.entryconfig(1, state=DISABLED)
                self.pcal = result[0]
                self.fileList = result[1]
                self.drawFVP()
                self.root.title("xcal " + os.path.basename(self.filename))
                command = "./caltool -info < {file} > InfoTempOutput.txt 2>> error.txt".format(file = self.filename)
                os.system(command)
                f = open("InfoTempOutput.txt", "r")
                loadedFile = f.read()
                
                f2 = open("error.txt", "r")
                stderr = f2.read()
                os.remove("error.txt")
                self.log.config(state = NORMAL)
                self.log.insert("end", stderr + "\n")
                self.log.see("end")
                self.log.config(state = DISABLED)
                
                self.log.config(state = NORMAL)
                self.log.insert("end", loadedFile + "\n")
                self.log.see("end")
                self.log.config(state = DISABLED)
                
                self.fileMenu.entryconfig(1, state=NORMAL)
                self.fileMenu.entryconfig(2, state=NORMAL)
                self.fileMenu.entryconfig(3, state=NORMAL)
                self.fileMenu.entryconfig(4, state=NORMAL)
                self.todoMenu.entryconfig(0, state=NORMAL)
                self.extrEvntBtn.config(state=NORMAL)
                self.extrXPropBtn.config(state=NORMAL)
                
            else :
                self.log.config(state = NORMAL)
                self.log.insert("end", error + "\n")
                self.log.see("end")
                self.log.config(state = DISABLED)
                self.filename = ""

    def drawFVP(self):
        self.tableSize = 0
        for i in self.CompTable.get_children():
            self.CompTable.delete(i)
        for tup in self.fileList:
            self.tableSize = self.tableSize + 1
            self.CompTable.insert('', 'end', iid = self.tableSize, text = str(self.tableSize), values = (tup[0], tup[1], tup[2], tup[3]))
    
    def combineSC(self, event):
        if self.filename != "":
            self.combine()
            
    def combine(self):
        combineFile = askopenfilename()
        if combineFile:
            Cal.writeFile("CombineTemp.txt", self.pcal, self.fileList)
            command = "./caltool -combine {0} < CombineTemp.txt > CombineTempOutput.txt 2>> error.txt".format(combineFile)
            os.system(command)
            self.oldList = self.fileList
            result = []
            error = Cal.readFile("CombineTempOutput.txt", result)
            
            f = open("error.txt", "r")
            stderr = f.read()
            os.remove("error.txt")
            self.log.config(state = NORMAL)
            self.log.insert("end", stderr + "\n")
            self.log.see("end")
            self.log.config(state = DISABLED)
            
            if error == "OK":
                Cal.freeFile(self.pcal)
                self.pcal = result[0]
                self.fileList = result[1]
                self.unsavedChanges = 1
                self.todoMenu.entryconfig(1, state=NORMAL)
                self.root.title("xcal " + os.path.basename(self.filename) + "*")
                self.drawFVP()
            else :
                self.log.config(state = NORMAL)
                self.log.insert("end", error + "\n")
                self.log.see("end")
                self.log.config(state = DISABLED) 
            
    def saveSC(self, event):
        self.saveFile()
        
    def saveFile(self):
        error = Cal.writeFile(self.filename, self.pcal, self.fileList)
        if error == "OK":
            with open(self.filename) as savedFile:
                linesWritten = sum(1 for line in savedFile)
                self.log.config(state = NORMAL)
                self.log.insert("end", "Lines written: " + str(linesWritten) + "\n")
                self.log.see("end")
                self.log.config(state = DISABLED) 
            
            self.unsavedChanges = 0
            self.todoMenu.entryconfig(1, state=DISABLED)
            self.oldList = []
            self.root.title("xcal " + os.path.basename(self.filename))
        else:
            self.log.config(state = NORMAL)
            self.log.insert("end", error + "\n")
            self.log.see("end")
            self.log.config(state = DISABLED)
        
    def saveFileAs(self): 
        outputFile = asksaveasfilename(defaultextension = ".ics")
        if (outputFile):
            
            error = Cal.writeFile(outputFile, self.pcal, self.fileList)
            with open(outputFile) as savedFile:
                linesWritten = sum(1 for line in savedFile)
                self.log.config(state = NORMAL)
                self.log.insert("end", "Lines written: " + str(linesWritten) + "\n")
                self.log.see("end")
                self.log.config(state = DISABLED) 
            self.filename = outputFile
            self.unsavedChanges = 0
            self.todoMenu.entryconfig(1, state=DISABLED)
            self.oldList = []
            self.root.title("xcal " + os.path.basename(self.filename))
        else:
            self.log.config(state = NORMAL)
            self.log.insert("end", error + "\n")
            self.log.see("end")
            self.log.config(state = DISABLED)
            
    def filter(self):
        filter = Toplevel()
        filter.title("Filter components")
        filter.attributes("-topmost", True)
        v = IntVar()
        
        
        def Cancel():
            filter.destroy()
            
        def Cancel2(event):
            filter.destroy()
        
        
        filter.bind_all("<Escape>", Cancel2)
            
        def Filter():
            fromstr = ""
            if FromDate.get() != fromstr:
                fromstr = ("from " + '"' + FromDate.get() + '"')
                
            tostr = ""
            if ToDate.get() != tostr:
                tostr = ("to " + '"' + ToDate.get() + '"')
                
            if v.get() == 1:
                Cal.writeFile("FilterTemp.txt", self.pcal, self.fileList)
                command = "./caltool -filter t {0} {1} < FilterTemp.txt > FilterTempOutput.txt 2>> error.txt".format(fromstr, tostr)
                os.system(command)
            elif v.get() == 2:
                Cal.writeFile("FilterTemp.txt", self.pcal, self.fileList)
                command = "./caltool -filter e {0} {1} < FilterTemp.txt > FilterTempOutput.txt 2>> error.txt" .format(fromstr, tostr)
                os.system(command)
                
            result = []
            error = Cal.readFile("FilterTempOutput.txt", result)
            
            f = open("error.txt", "r")
            stderr = f.read()
            os.remove("error.txt")
            self.log.config(state = NORMAL)
            self.log.insert("end", stderr + "\n")
            self.log.see("end")
            self.log.config(state = DISABLED)
            
            if error == "OK":
                Cal.freeFile(self.pcal)
                self.pcal = result[0]
                self.fileList = result[1]
                self.unsavedChanges = 1
                self.todoMenu.entryconfig(1, state=NORMAL)
                self.root.title("xcal " + os.path.basename(self.filename) + "*")
                self.drawFVP()
            else :
                self.log.config(state = NORMAL)
                self.log.insert("end", error + "\n")
                self.log.see("end")
                self.log.config(state = DISABLED) 
            filter.destroy()
        
        btn2 = Button(filter, text = "Filter", state=DISABLED, command = Filter)
        
        def enablebtn():
            btn2.config(state=NORMAL)

        rb1 = Radiobutton(filter, text = "Todos", variable = v, value = 1, command = enablebtn)
        rb1.pack(fill = "x")
        rb2 = Radiobutton(filter, text = "Events", variable = v, value = 2, command = enablebtn)
        rb2.pack(fill = "x")
            
        Label(filter, text = "From Date:").pack(fill = "x")
        FromDate = Entry(filter)
        FromDate.pack(fill = "x")
            
        Label(filter, text = "To Date:").pack(fill = "x")
        ToDate = Entry(filter)
        ToDate.pack(fill = "x")
            
        filter.minsize(width = 250, height = 183)
        filter.maxsize(width = 250, height = 183)
            
        btn = Button(filter, text = "Cancel", command = Cancel)
        btn.pack(fill = "both")
        
        btn2.pack(fill = "both")
       # filter.grab_set()
        mainloop()

    def popup(self, event):
        self.CompTable.config(selectmode="browse")
        iid = self.CompTable.identify_row(event.y)
        item = self.CompTable.focus()
        
        if item == iid:
            if iid != "":
                self.CompTable.selection_toggle(iid)
                if self.rowSelected == 0:
                    self.rowSelected = 1
                    self.showSelButn.config(state=NORMAL)
                else:
                    self.rowSelected = 0
                    self.showSelButn.config(state=DISABLED)
            
        else:
            if iid != "":
                self.rowSelected = 1
                self.CompTable.selection_remove(item)
                self.showSelButn.config(state=NORMAL)
                self.CompTable.selection_toggle(iid)
        self.CompTable.config(selectmode="none")

    def extractEvents(self):
        Cal.writeFile("ExtractEventsTemp.txt", self.pcal, self.fileList)
        command = "./caltool -extract e < ExtractEventsTemp.txt > ExtractEventsTempOutput.txt 2>> error.txt"
        os.system(command)

        f = open("ExtractEventsTempOutput.txt", "r")
        loadedFile = f.read()
        self.log.config(state = NORMAL)
        self.log.insert("end", loadedFile + "\n")
        self.log.see("end")
        self.log.config(state = DISABLED)

    def extractXProps(self):
        Cal.writeFile("ExtractXPropsTemp.txt", self.pcal, self.fileList)
        command = "./caltool -extract x < ExtractXPropsTemp.txt > ExtractXPropsTempOutput.txt 2>> error.txt"
        os.system(command)

        f = open("ExtractXPropsTempOutput.txt", "r")
        loadedFile = f.read()
        
        f2 = open("error.txt", "r")
        stderr = f2.read()
        os.remove("error.txt")
            
        self.log.config(state = NORMAL)
        self.log.insert("end", stderr + "\n")
        self.log.see("end")
        self.log.config(state = DISABLED)
        
        self.log.config(state = NORMAL)
        self.log.insert("end", loadedFile + "\n")
        self.log.see("end")
        self.log.config(state = DISABLED)

    def dateMskStartup(self, top):
        self.getDateMsk
        top.destroy

    def getDateMsk(self):
        datemskloc = askopenfilename()
        if (datemskloc):
            os.environ["DATEMSK"] = datemskloc

    def showSelected(self):
        tempFile = "ShowSelectedTemp.txt"
        item = self.CompTable.focus()
        if (item):
            select = self.CompTable.item(self.CompTable.focus())
        if (select):
            Cal.writeFile(tempFile, self.pcal, int(select['text']) - 1)
        f = open(tempFile, "r")
        loadedFile = f.read()# print(loadedFile)
        self.log.config(state = NORMAL)
        self.log.insert("end", loadedFile + "\n")
        self.log.see("end")
        self.log.config(state = DISABLED)

    def aboutWindow(self): #showinfo pops up an entire new window.Alert in Java.
        showinfo("About", "xcal\nCompatible with iCalendar V2.0\nWritten by Brandon Chester")

    def quitSC(self, event):
        self.quitProg()

    def quitProg(self):
    
        def trueExit():
            if (self.pcal != 0):
                Cal.freeFile(self.pcal)
                self.pcal = 0
            try:
                os.remove("ShowSelectedTemp.txt")
            except OSError:
                pass
            try:   
                os.remove("FilterTemp.txt")
            except OSError:
                pass
                
            try:   
                os.remove("InfoTempOutput.txt")
            except OSError:
                pass
            try:   
                os.remove("FilterTempOutput.txt")
            except OSError:
                pass
                
            try:   
                os.remove("ExtractXPropsTempOutput.txt")
            except OSError:
                pass
                
            try:   
                os.remove("ExtractEventsTempOutput.txt")
            except OSError:
                pass
                
            try:   
                os.remove("ExtractXPropsTemp.txt")
            except OSError:
                pass
    
            try:
                os.remove("ExtractEventsTemp.txt")
            except OSError:
                pass
            
            try:
                os.remove("error.txt")
            except OSError:
                pass  
            try:   
                os.remove("CombineTempOutput.txt")
            except OSError:
                pass  
            try:   
                os.remove("CombineTemp.txt")
            except OSError:
                pass  
            xCal.root.destroy()
            xCal.root.quit()
    
        dialog = Toplevel()
        dialog.title("Quit confirmation")
        dialog.attributes("-topmost", True)
        dialog.lift()
        dialog.grab_set()
            
        def Cancel2(event):
            dialog.destroy()
        
        dialog.bind_all("<Escape>", Cancel2)
            
        def Cancel():
            dialog.destroy()
                
        def Continue():
            dialog.destroy()
            self.openFile()
                
            
        Label(dialog, text = "Are you sure you want to quit?").pack(fill = "x")
        btn = Button(dialog, text = "Cancel", command = Cancel)
        btn2 = Button(dialog, text = "Quit", command = trueExit)
            
        dialog.minsize(width = 250, height = 65)
        dialog.maxsize(width = 250, height = 65)

        btn.pack(fill = "both")
        btn2.pack(fill = "both")
        dialog.grab_set()

xCal = XCalGUI()

mainloop()