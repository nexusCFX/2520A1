#!/usr/bin/python3
import time
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
        self.unsavedChanges = 0
        self.fileList = []
        self.oldList = []
        self.pcal = 0
        self.root = Tk()
        self.root.title("xcal")
        self.root.minsize(width = 700, height = 666)
        self.tableSize = 0
        
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

        self.menuBar.add_cascade( menu = self.fileMenu, label = "File")
        self.menuBar.add_cascade( menu = self.todoMenu, label = "Todo")
        self.menuBar.add_cascade( menu = self.helpMenu, label = "Help")
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
                
            top.minsize(width = 270, height = 60)
            top.maxsize(width = 270, height = 60)
            L1 = Label(top, text = "You do not have a Datemsk file specified.\nWould you like to enter it now?")
            L1.grid(row = 0, column = 0, columnspan = 2, sticky = 'n')
            yesBtn = Button(top, text = "Yes", command = inputDateMsk)
            yesBtn.grid(row = 2, column = 0, columnspan = 1)
            noBtn = Button(top, text = "Not now", command = top.destroy)
            noBtn.grid(row = 2, column = 1, columnspan = 1)
            
            
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
        
    def saveChangesDialog(self):
        dialog = Toplevel()
        dialog.title("Unsaved Changes")
        
        def Cancel():
            dialog.destroy()
            
        def Continue():
            self.unsavedChanges = 0
            self.todoMenu.entryconfig(1, state=DISABLED)
            dialog.destroy()
        
        Label(dialog, text = "You have unsaved changes.").pack(fill = "x")
        Label(dialog, text = "Do you want to proceed?").pack(fill = "x")
        btn = Button(dialog, text = "Cancel", command = Cancel)
        btn2 = Button(dialog, text = "Continue", command = Continue)
        
        dialog.minsize(width = 250, height = 183)
        dialog.maxsize(width = 250, height = 183)

        btn.pack(fill = "both")
        btn2.pack(fill = "both")
        
    def undoCHK(self):
        if (self.unsavedChanges == 1):
            self.undo()
        
    def undoSC(self, event):
        if (self.unsavedChanges == 1):
            self.undo()
        
    def undo(self):
        dialog = Toplevel()
        dialog.grab_set()
        dialog.title("Undo")
        
        def Cancel():
            dialog.destroy()
            
        def Undo():
            dialog.destroy()
            self.fileList = self.oldList
            self.oldList = []
            self.drawFVP()
            self.unsavedChanges = 0
            self.todoMenu.entryconfig(1, state=DISABLED)
            self.root.title("xcal " + os.path.basename(self.filename))
            
        
        Label(dialog, text = "All Todo items removed since the last save will be restored").pack(fill = "x")
        Label(dialog, text = "Do you want to proceed?").pack(fill = "x")
        btn = Button(dialog, text = "Cancel", command = Cancel)
        btn2 = Button(dialog, text = "Undo", command = Undo)
        
        dialog.minsize(width = 250, height = 183)
        dialog.maxsize(width = 250, height = 183)

        btn.pack(fill = "both")
        btn2.pack(fill = "both") 
        
        
        
    def openFileDLG(self):
        if self.unsavedChanges == 1:
            dialog = Toplevel(self.root)
            
            def Cancel():
                dialog.destroy()
                
            def Continue():
                dialog.destroy()
                self.openFile()
                
            
            Label(dialog, text = "You have unsaved changes.").pack(fill = "x")
            Label(dialog, text = "Do you want to proceed?").pack(fill = "x")
            btn = Button(dialog, text = "Cancel", command = Cancel)
            btn2 = Button(dialog, text = "Continue", command = Continue)
            
            dialog.minsize(width = 250, height = 183)
            dialog.maxsize(width = 250, height = 183)

            btn.pack(fill = "both")
            btn2.pack(fill = "both")
            dialog.grab_set()
        else:
            self.openFile()
          
    def openFile(self):
        self.unsavedChanges = 0
        self.filename = askopenfilename()
        
                
        if (self.filename):
        
            result = []
            error = Cal.readFile(self.filename, result)
            if error == "OK":
                if (self.pcal != 0):
                    Cal.freeFile(self.pcal)
                self.unsavedChanges = 0
                self.pcal = result[0]
                self.fileList = result[1]
                self.drawFVP()
                self.root.title("xcal " + os.path.basename(self.filename))
                command = "./caltool -info < {file} > InfoTempOutput.txt".format(file = self.filename)
                os.system(command)
                f = open("InfoTempOutput.txt", "r")
                loadedFile = f.read()
                self.log.config(state = NORMAL)
                self.log.insert("end", loadedFile + "\n")
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
            command = "./caltool -combine {0} < CombineTemp.txt > CombineTempOutput.txt".format(combineFile)
            os.system(command)

            result = []
            error = Cal.readFile("CombineTempOutput.txt", result)
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
                self.log.config(state = DISABLED) 
            
    def saveSC(self, event):
        self.saveFile()
        
    def saveFile(self):
        Cal.writeFile(self.filename, self.pcal, self.fileList)
        self.unsavedChanges = 0
        self.todoMenu.entryconfig(1, state=DISABLED)
        self.oldList = []
        self.root.title("xcal " + os.path.basename(self.filename))
        
    def saveFileAs(self): 
        outputFile = asksaveasfilename(defaultextension = ".ics")
        if (outputFile):
            Cal.writeFile(outputFile, self.pcal, self.fileList)
            
            self.filename = outputFile
            self.unsavedChanges = 0
            self.todoMenu.entryconfig(1, state=DISABLED)
            self.oldList = []
            self.root.title("xcal " + os.path.basename(self.filename))
            
    def filter(self):
        filter = tk.Toplevel()
        filter.grab_set()
        v = IntVar()
        
        def Cancel():
            filter.destroy()
            
        def Filter():
            fromstr = ""
            if FromDate.get() != fromstr:
                fromstr = ("from " + '"' + FromDate.get() + '"')
                
            tostr = ""
            if ToDate.get() != tostr:
                tostr = ("to " + '"' + ToDate.get() + '"')
                
            if v.get() == 1:
                Cal.writeFile("FilterTemp.txt", self.pcal, self.fileList)
                command = "./caltool -filter t {0} {1} < FilterTemp.txt > FilterTempOutput.txt".format(fromstr, tostr)
                os.system(command)
            elif v.get() == 2:
                Cal.writeFile("FilterTemp.txt", self.pcal, self.fileList)
                command = "./caltool -filter e {0} {1} < FilterTemp.txt > FilterTempOutput.txt".format(fromstr, tostr)
                os.system(command)
                
            result = []
            error = Cal.readFile("FilterTempOutput.txt", result)
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
                self.log.config(state = DISABLED) 
            filter.destroy()

        Radiobutton(filter, text = "Todos", variable = v, value = 1).pack(fill = "x")
        Radiobutton(filter, text = "Events", variable = v, value = 2).pack(fill = "x")
            
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
        btn2 = Button(filter, text = "Filter", command = Filter)
        btn2.pack(fill = "both")
        mainloop()

    def popup(self, event):
        self.CompTable.config(selectmode="browse")
        self.showSelButn.config(state=NORMAL)
        iid = self.CompTable.identify_row(event.y)
        item = self.CompTable.focus()
        self.CompTable.selection_toggle(iid)
        if item == iid:
            self.showSelButn.config(state=DISABLED)
        else:
            self.CompTable.selection_remove(item)
            #self.showSelButn.config(state=NORMAL)
        self.CompTable.config(selectmode="none")

    def extractEvents(self):
        Cal.writeFile("ExtractEventsTemp.txt", self.pcal, self.fileList)
        command = "./caltool -extract e < ExtractEventsTemp.txt > ExtractEventsTempOutput.txt"
        os.system(command)

        f = open("ExtractEventsTempOutput.txt", "r")
        loadedFile = f.read()
        self.log.config(state = NORMAL)
        self.log.insert("end", loadedFile + "\n")
        self.log.config(state = DISABLED)

    def extractXProps(self):
        Cal.writeFile("ExtractXPropsTemp.txt", self.pcal, self.fileList)
        command = "./caltool -extract x < ExtractXPropsTemp.txt > ExtractXPropsTempOutput.txt"
        os.system(command)

        f = open("ExtractXPropsTempOutput.txt", "r")
        loadedFile = f.read()
        self.log.config(state = NORMAL)
        self.log.insert("end", loadedFile + "\n")
        self.log.config(state = DISABLED)

    def dateMskStartup(self, top):
        self.getDateMsk
        top.destroy

    def getDateMsk(self):
        datemskloc = askopenfilename()
        if (datemskloc):
            os.environ["DATEMSK"] = datemskloc
            # command = "env DATEMSK={loc}".format(loc = datemskloc)# os.system(command)
            

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
        self.log.config(state = DISABLED)

    def aboutWindow(self): #showinfo pops up an entire new window.Alert in Java.
        showinfo("About", "xcal\nCompatible with iCalendar V2.0\nWritten by Brandon Chester")

    def quitSC(self, event):
        self.quitProg()

    def quitProg(self):
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
            os.remove("FilterTempOutput.txt")
        except OSError:
            pass
            
        try:   
            os.remove("InfoTempOutput.txt")
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
        xCal.root.destroy()
        xCal.root.quit()
        

xCal = XCalGUI()

mainloop()