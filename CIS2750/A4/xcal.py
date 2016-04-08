#!/usr/bin/python3
import time
import getpass
import mysql.connector
from mysql.connector import errorcode
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
        fails = 0
        server = "dursley.socs.uoguelph.ca"
        
        if len(sys.argv) == 3:
            server = sys.argv[2]
            #print(server)
        
        password = getpass.getpass("Password:")
        while(1):
            try:
                 self.cnx = mysql.connector.connect(user=sys.argv[1], password=password, host=server, database=sys.argv[1])
            except mysql.connector.errors.Error as err:
                fails = fails + 1
                if err.errno == errorcode.ER_ACCESS_DENIED_ERROR:
                    print("Something is wrong with your user name or password")
                elif err.errno == errorcode.ER_BAD_DB_ERROR:
                    print("Database does not exist")
                else:
                    print(err)  
                    time.sleep(1)    
            if fails == 0:
                break
            elif fails == 3:
                print("Failure to connect after 3 tries. Exiting")
                exit()  
        
        self.cursor = self.cnx.cursor()
        
        cmd = "CREATE TABLE ORGANIZER( org_id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(60) NOT NULL, contact VARCHAR(60) NOT NULL );"
        try:
            self.cursor.execute(cmd)
            self.cnx.commit()
        except mysql.connector.Error:
            pass
        
        cmd = "CREATE TABLE EVENT( event_id INT AUTO_INCREMENT PRIMARY KEY, summary VARCHAR(60) NOT NULL, start_time DATETIME NOT NULL, location VARCHAR(60), organizer INT, FOREIGN KEY(organizer) REFERENCES ORGANIZER(org_id) ON DELETE CASCADE );"
        try:
            self.cursor.execute(cmd)
            self.cnx.commit()
        except mysql.connector.Error:
            pass
        
        
        cmd = "CREATE TABLE TODO( todo_id INT AUTO_INCREMENT PRIMARY KEY, summary VARCHAR(60) NOT NULL, priority SMALLINT, organizer INT, FOREIGN KEY(organizer) REFERENCES ORGANIZER(org_id) ON DELETE CASCADE );"
        try:
            self.cursor.execute(cmd)
            self.cnx.commit()
        except mysql.connector.Error:
            pass
            
        
        
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
        
        self.dbMenu.entryconfig(0, state=DISABLED)
        self.dbMenu.entryconfig(1, state=DISABLED)
        
        
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
        
        self.tableSize = 0
        
        self.getTableSize()

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
            
    def storeAll(self):
        for i in range(len(self.fileList)):
            storeItem = self.fileList[i]
            if storeItem[0] != "VTIMEZONE":
                
                
                query = "SELECT org_id FROM ORGANIZER WHERE name = '{0}';".format(storeItem[7].replace("'","''"))    
                self.cursor.execute(query)
                orgid = ""
                for thing in self.cursor:
                    orgid = thing[0]
                
                if orgid == "" and storeItem[7] != "" and storeItem[8] != "":
                    query = "INSERT INTO ORGANIZER (name, contact) VALUES('{0}', '{1}');".format(storeItem[7].replace("'","''"), storeItem[8].replace("'","''"))
                    #print(query)
                    self.cursor.execute(query)
                    self.cnx.commit()

                query = "SELECT org_id FROM ORGANIZER WHERE name = '{0}';".format(storeItem[7].replace("'","''"))    
                self.cursor.execute(query)
                orgid = ""
                for thing in self.cursor:
                    orgid = thing[0]
                
                if orgid == "":
                    orgid = "NULL"
                    
                    #          0      1       2         3           4        5          6         7         8
                ####Tuple: compname, nprops, ncomps, summary start_time, location, priority, organizer, contact
                
                if (storeItem[0] == "VEVENT"):
                    structTime = time.strptime(storeItem[4], "%Y%m%dT%H%M%S")
                
                    timeStr = time.strftime("%Y-%m-%d %H-%M-%S", structTime)
                
                    query = "SELECT event_id FROM EVENT WHERE summary = '{0}' AND start_time = '{1}'".format(storeItem[3].replace("'","''"), timeStr)
                    self.cursor.execute(query)
                    eventID = ""
                    for thing in self.cursor:
                        eventID = thing[0]
                    
                    if eventID == "":
                        query = "INSERT INTO EVENT (summary, start_time, location, organizer) VALUES('{0}', '{1}', '{2}', {3});".format(storeItem[3].replace("'","''"), timeStr, storeItem[5].replace("'","''"), orgid)
                        #print(query)
                        self.cursor.execute(query)
                        self.cnx.commit()
                else:
                    query = "SELECT todo_id FROM TODO WHERE summary = '{0}'".format(storeItem[3].replace("'","''"))
                    self.cursor.execute(query)
                    todoID = ""
                    for thing in self.cursor:
                        todoID = thing[0]
                    
                    if todoID == "":
                        query = "INSERT INTO TODO (summary, priority, organizer) VALUES('{0}', {1}, {2});".format(storeItem[3].replace("'","''"), storeItem[6], orgid)
                        #print(query)
                        self.cursor.execute(query)
                        self.cnx.commit()
                        
        self.getTableSize()
        self.statusDB()
        
    def storeSelected(self):
        item = self.CompTable.focus()
        
        storeItem = self.fileList[int(item)-1]
        
        if storeItem[0] != "VTIMEZONE":
            query = "SELECT org_id FROM ORGANIZER WHERE name = '{0}'".format(storeItem[7].replace("'","''"))    
            self.cursor.execute(query)
            orgid = ""
            for thing in self.cursor:
                orgid = thing[0]
            
            if orgid == "" and storeItem[7] != "" and storeItem[8] != "":
                query = "INSERT INTO ORGANIZER (name, contact) VALUES('{0}', '{1}');".format(storeItem[7].replace("'","''"), storeItem[8])
                #print(query)
                self.cursor.execute(query)
                self.cnx.commit()

            query = "SELECT org_id FROM ORGANIZER WHERE name = '{0}'".format(storeItem[7].replace("'","''"))    
            self.cursor.execute(query)
            orgid = ""
            for thing in self.cursor:
                orgid = thing[0]
            
            if orgid == "":
                orgid = "NULL"
                
                #          0      1       2         3           4        5          6         7         8
            ####Tuple: compname, nprops, ncomps, summary start_time, location, priority, organizer, contact
            
            if (storeItem[0] == "VEVENT"):
                structTime = time.strptime(storeItem[4], "%Y%m%dT%H%M%S")
                
                timeStr = time.strftime("%Y-%m-%d %H-%M-%S", structTime)
            
                query = "SELECT event_id FROM EVENT WHERE summary = '{0}' AND start_time = '{1}'".format(storeItem[3].replace("'","''"), timeStr)
                self.cursor.execute(query)
                eventID = ""
                for thing in self.cursor:
                    eventID = thing[0]
                
                if eventID == "":
                    query = "INSERT INTO EVENT (summary, start_time, location, organizer) VALUES('{0}', '{1}', '{2}', {3});".format(storeItem[3].replace("'","''"), timeStr, storeItem[5].replace("'","''"), orgid)
                    #print(query)
                    self.cursor.execute(query)
                    self.cnx.commit()
            else:
                query = "SELECT todo_id FROM TODO WHERE summary = '{0}'".format(storeItem[3].replace("'","''"))
                self.cursor.execute(query)
                todoID = ""
                for thing in self.cursor:
                    todoID = thing[0]
                
                if todoID == "":
                    query = "INSERT INTO TODO (summary, priority, organizer) VALUES('{0}', {1}, {2});".format(storeItem[3].replace("'","''"), storeItem[6], orgid)
                    #print(query)
                    self.cursor.execute(query)
                    self.cnx.commit()
            
            self.getTableSize()
            self.statusDB()
            
    
    def clearDB(self):
        self.getTableSize()
        if self.tableSize > 0:
            self.cursor.execute("SET FOREIGN_KEY_CHECKS = 0;")
            query = "TRUNCATE EVENT;";
            self.cursor.execute(query)
            self.cnx.commit()
            
            query = "TRUNCATE TODO;";
            self.cursor.execute(query)
            self.cnx.commit()
            
            query = "TRUNCATE ORGANIZER;";
            self.cursor.execute(query)
            self.cnx.commit()
            self.cursor.execute("SET FOREIGN_KEY_CHECKS = 1;")
        self.getTableSize()
        self.statusDB()
            
    def getTableSize(self):
        query = "SELECT COUNT(*) FROM ORGANIZER;";
        self.cursor.execute(query)
        
        orgNum = ""
        for thing in self.cursor:
            orgNum = thing[0]
            
        query = "SELECT COUNT(*) FROM EVENT;";
        self.cursor.execute(query)
        
        eventNum = ""
        for thing in self.cursor:
            eventNum = thing[0]
            
        query = "SELECT COUNT(*) FROM TODO;";
        self.cursor.execute(query)
        
        todoNum = ""
        for thing in self.cursor:
            todoNum = thing[0]
            
        self.tableSize = orgNum + eventNum + todoNum
        
        if self.tableSize < 1:
            self.dbMenu.entryconfig(2, state=DISABLED)
        else:
            self.dbMenu.entryconfig(2, state=NORMAL)
            
        
    def statusDB(self):
        query = "SELECT COUNT(*) FROM ORGANIZER;";
        self.cursor.execute(query)
        
        orgNum = ""
        for thing in self.cursor:
            orgNum = thing[0]
            
        query = "SELECT COUNT(*) FROM EVENT;";
        self.cursor.execute(query)
        
        eventNum = ""
        for thing in self.cursor:
            eventNum = thing[0]
            
        query = "SELECT COUNT(*) FROM TODO;";
        self.cursor.execute(query)
        
        todoNum = ""
        for thing in self.cursor:
            todoNum = thing[0]
        
        self.log.config(state = NORMAL)
        
        line = "Database has " + str(orgNum) + " organizers, " + str(eventNum) + " events, " + str(todoNum) + " to-do items."
        self.log.insert("end", line + "\n")
            
        self.log.see("end")
        self.log.config(state = DISABLED)
            
    def queryDB(self):
        queryWin = Toplevel()
        queryWin.title("Query Database")
        self.dbMenu.entryconfig(4, state=DISABLED)
        
        def destroyWin():
            self.dbMenu.entryconfig(4, state=NORMAL)
            queryWin.destroy()
        
        queryWin.protocol("WM_DELETE_WINDOW", destroyWin)
        v = IntVar()
        
        
        def Cancel():
            queryWin.destroy()
            
        def Submit():
           # print(str(v.get()))
            if v.get() == 1:
                try:
                    query = "SELECT summary FROM EVENT WHERE organizer = (SELECT org_id FROM ORGANIZER where name = '{0}');".format((Q1E.get()).replace("'","''"))
                    self.cursor.execute(query)
                    log.config(state = NORMAL)
                    for thing in self.cursor:
                        log.insert(END, thing[0] + "\n")
                    query = "SELECT summary FROM TODO WHERE organizer = (SELECT org_id FROM ORGANIZER where name = '{0}');".format((Q1E.get()).replace("'","''"))
                    self.cursor.execute(query)
                    for thing in self.cursor:
                        log.insert(END, thing[0] + "\n")
                    log.insert(END, "----------------------------\n")
                    log.config(state = DISABLED)
                except mysql.connector.Error:
                    pass
            elif v.get() == 2:
                try:
                    query = "SELECT summary FROM EVENT WHERE location = '{0}';".format((Q2E.get()).replace("'","''"))
                    self.cursor.execute(query)
                    log.config(state = NORMAL)
                    i = 0
                    for thing in self.cursor:
                        i = i + 1
                    log.insert(END, str(i) + "\n")
                    log.insert(END, "----------------------------\n")
                    log.config(state = DISABLED)
                except mysql.connector.Error:
                    pass
            elif v.get() == 3:
                try:
                    sdate = (Q3E.get()).replace("'","''") + "-01-01 00:00:00"
                    edate = (Q3E.get()).replace("'","''") + "-12-31 23:59:59"
                    query = "SELECT summary, start_time FROM EVENT WHERE start_time BETWEEN '{0}' AND '{1}';".format(sdate, edate)
                    #print (query)
                    self.cursor.execute(query)
                    log.config(state = NORMAL)
                    for thing in self.cursor:
                        log.insert(END, "Summary: " + str(thing[0]) + " - Start time: " + str(thing[1]) + "\n")
                    log.insert(END, "----------------------------\n")
                    log.config(state = DISABLED)
                except mysql.connector.Error:
                    pass
            elif v.get() == 4:
                try:
                    log.config(state = NORMAL)
                    query = "SELECT summary, priority FROM TODO WHERE priority <= {0} ORDER BY priority;".format(Q4E.get())
                    self.cursor.execute(query)
                    for thing in self.cursor:
                        log.insert(END, "Summary: " + str(thing[0]) + " - Priority: " + str(thing[1]) + "\n")
                    log.insert(END, "----------------------------\n")
                    log.config(state = DISABLED)
                except mysql.connector.Error:
                    pass
            elif v.get() == 5:
                try:
                    log.config(state = NORMAL)
                    query = "SELECT summary, name, contact FROM (SELECT summary, organizer FROM EVENT UNION SELECT summary, organizer FROM TODO) AS FULL, ORGANIZER WHERE (FULL.organizer = ORGANIZER.org_id) ORDER BY summary;"
                    self.cursor.execute(query)
                    for thing in self.cursor:
                        mail = thing[2][7:]
                        log.insert(END, "Summary: " + str(thing[0]) + " - Organizer: " + str(thing[1]) + " - Email: " + mail + "\n")
                    log.insert(END, "----------------------------\n")
                    log.config(state = DISABLED)
                except mysql.connector.Error:
                    pass
            elif v.get() == 6:
                try:
                    query = (QueryF.get()).replace("'","''")
                    self.cursor.execute(query)
                    log.config(state = NORMAL)
                    for thing in self.cursor:
                        for item in thing:
                            log.insert(END, str(item) + "\n")
                    log.insert(END, "----------------------------\n")
                    log.config(state = DISABLED)
                except mysql.connector.Error:
                    pass
            log.see("end")
        
        def enablebtn():
            btn.config(state=NORMAL)
            return 1
            
            
        def clrLog():
            log.config(state = NORMAL)
        
            log.delete('1.0', END)
            log.config(state = DISABLED)
            
            
        def help():
            helpW = Toplevel()
            helpW.title("Help")
            
            
            helpLog = Text(helpW, borderwidth = 2)
            helpLog.config(font = ("consolas", 12), undo = True, wrap = 'word', state = NORMAL)
            helpLog.pack(fill = "x", side = "left")
            
            scrollbH = Scrollbar(helpW, orient = tk.VERTICAL, command = helpLog.yview)
            scrollbH.pack(fill = "y", side = "right")

            helpLog['yscrollcommand'] = scrollbH.set
            self.cursor.execute("DESCRIBE ORGANIZER;")
            helpLog.insert(END, "Table ORGANIZER:\n")
            for thing in self.cursor:
                helpLog.insert(END, "Column: " + str(thing[0]) + "\n")
                helpLog.insert(END, "Type: " + thing[1] + "\n")
                helpLog.insert(END, "Null: " + thing[2] + "\n")
                helpLog.insert(END, "Key: " + thing[3] + "\n")
                helpLog.insert(END, "Default: " + str(thing[4]) + "\n")
                helpLog.insert(END, "Extra: " + thing[5] + "\n\n")
                
            helpLog.insert(END, "--------------------------\n")
            self.cursor.execute("DESCRIBE EVENT;")
            helpLog.insert(END, "Table EVENT:\n")
            for thing in self.cursor:
                helpLog.insert(END, "Column: " + thing[0] + "\n")
                helpLog.insert(END, "Type: " + thing[1] + "\n")
                helpLog.insert(END, "Null: " + thing[2] + "\n")
                helpLog.insert(END, "Key: " + thing[3] + "\n")
                helpLog.insert(END, "Default: " + str(thing[4]) + "\n")
                helpLog.insert(END, "Extra: " + thing[5] + "\n\n")
            helpLog.insert(END, "--------------------------\n")
            self.cursor.execute("DESCRIBE TODO;")
            helpLog.insert(END, "Table TODO:\n")
            for thing in self.cursor:
                helpLog.insert(END, "Column: " + thing[0] + "\n")
                helpLog.insert(END, "Type: " + thing[1] + "\n")
                helpLog.insert(END, "Null: " + thing[2] + "\n")
                helpLog.insert(END, "Key: " + thing[3] + "\n")
                helpLog.insert(END, "Default: " + str(thing[4]) + "\n")
                helpLog.insert(END, "Extra: " + thing[5] + "\n\n")
                
            helpLog.config(state = DISABLED)

        rb1 = Radiobutton(queryWin, text = "Display items of organizer", variable = v, value = 1, command = enablebtn)
        rb1.grid(row = 0, column = 0, sticky = 'w')
        Q1E = Entry(queryWin)
        Q1E.grid(row = 0, column = 1, sticky = 'w')
        rb2 = Radiobutton(queryWin, text = "How many events take place in", variable = v, value = 2, command = enablebtn)
        rb2.grid(row = 1, column = 0, sticky = 'w')
        Q2E = Entry(queryWin)
        Q2E.grid(row = 1, column = 1, sticky = 'w')
        rb3 = Radiobutton(queryWin, text = "Events taking place in the year", variable = v, value = 3, command = enablebtn)
        rb3.grid(row = 2, column = 0, sticky = 'w')
        Q3E = Entry(queryWin)
        Q3E.grid(row = 2, column = 1, sticky = 'w')
        rb4 = Radiobutton(queryWin, text = "Sorted list of todo items with a minimum priority of", variable = v, value = 4, command = enablebtn)
        rb4.grid(row = 3, column = 0, sticky = 'w')
        Q4E = Entry(queryWin)
        Q4E.grid(row = 3, column = 1, sticky = 'w')
        rb5 = Radiobutton(queryWin, text = "Display summaries for all events and todos that have organizers, sorted alphabetically", variable = v, value = 5, command = enablebtn)
        rb5.grid(row = 4, column = 0, sticky = 'w')
        
        
        rb6 = Radiobutton(queryWin, text = "Custom Query", variable = v, value = 6, command = enablebtn)
        rb6.grid(row = 5, column = 0, sticky = 'w')  
        QueryF = Entry(queryWin)
        QueryF.grid(row = 5, column = 1, sticky = 'w')
        QueryF.insert(END, "SELECT ")
        
        btn = Button(queryWin, text = "Submit", command = Submit)
        btn.grid(row = 6, column = 0)
        btn.config(state = DISABLED)
        
        log = Text(queryWin, borderwidth = 2, state = "disabled")
        log.config(font = ("consolas", 12), undo = True, wrap = 'word', state = DISABLED)
        log.grid(row = 7, column = 0, padx = 2, pady = 2)

        scrollb = Scrollbar(queryWin, orient = tk.VERTICAL, command = log.yview)
        scrollb.grid(row = 7, column = 1, sticky = 'nsew')

        log['yscrollcommand'] = scrollb.set
        clrLogBtn = Button(queryWin, text = "Clear log", command = clrLog)
        clrLogBtn.grid(row = 8, column = 0)
        
        helpBtn = Button(queryWin, text = "Help", command = help)
        helpBtn.grid(row = 8, column = 1)
        
            
        
            
        
        
       # btn2.pack(fill = "both")
       # filter.grab_set()
        queryWin.minsize(width = 765, height = 600)
        queryWin.maxsize(width = 765, height = 600)
        mainloop()
        
       
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
                
                self.dbMenu.entryconfig(0, state=NORMAL)
                
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
            #print( str(tup[0]) + " " + str(tup[1]) + " " +  str(tup[2]) +" " +  str(tup[3]) +" " +  str(tup[4]) +" " +  str(tup[5]) +" " +  str(tup[6]) +" " +  str(tup[7]) +" " +  str(tup[8]) )
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
                    self.dbMenu.entryconfig(1, state=NORMAL)
                else:
                    self.rowSelected = 0
                    self.showSelButn.config(state=DISABLED)
                    self.dbMenu.entryconfig(1, state=DISABLED)
            
        else:
            if iid != "":
                self.rowSelected = 1
                self.CompTable.selection_remove(item)
                self.showSelButn.config(state=NORMAL)
                self.dbMenu.entryconfig(1, state=NORMAL)
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
            self.cnx.close()
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
