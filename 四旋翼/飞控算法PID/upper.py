#!/usr/bin/env python
# FileName: main.py

import wx
import serial
import threading
import time

ser = serial.Serial()
ser.baudrate = 115200
X_FLAG = 0

class WorkerThread(threading.Thread):

    def __init__(self,window):
        threading.Thread.__init__(self)
        self.window = window
        self.timeToQuit = threading.Event()
        self.timeToQuit.clear()

    def run(self):
        if(ser.isOpen()):
            while 1:
                msg = ser.readline()
                print msg
                wx.CallAfter(self.window.LogMessage, msg)
                
class helloThread(threading.Thread):

    def __init__(self,window):
        threading.Thread.__init__(self)
        self.window = window
        self.timeToQuit = threading.Event()
        self.timeToQuit.clear()

    def run(self):
        if(ser.isOpen()):
            while 1:
                ser.write('S')
                time.sleep(1)
          

class MyFrame( wx.Frame ):

    def __init__( self, parent, id, title ):
        self.threads = []
        self.count = 0
        self.Dtype = 'text'
        wx.Frame.__init__(self,parent,id,title,wx.DefaultPosition,wx.Size(920, 250))
        menubar = wx.MenuBar()
        file = wx.Menu()
        file.Append( 22, '&Quit', 'Exit' )        
        menubar.Append( file, '&File' )
        self.SetMenuBar( menubar )
        panel = wx.Panel(self)
        font = wx.SystemSettings_GetFont(wx.SYS_SYSTEM_FONT)
        font.SetPointSize(15)
        
        main = wx.BoxSizer( wx.HORIZONTAL )
        left = wx.BoxSizer( wx.VERTICAL )
        right = wx.BoxSizer( wx.HORIZONTAL )
        main.Add(left,flag=wx.EXPAND|wx.LEFT|wx.TOP,border = 10)
        main.Add(right,flag=wx.EXPAND|wx.RIGHT|wx.TOP,border = 10)
        SUB1 = wx.BoxSizer( wx.HORIZONTAL )
        btnstart = wx.Button(panel, 1, label = 'start')
        btnstop = wx.Button(panel, 2, label = 'stop')
        btnx = wx.Button(panel, 12, label = 'x')
        SUB1.Add(btnstart, proportion = 0, flag = wx.LEFT, border = 0)
        SUB1.Add(btnstop, proportion = 0, flag = wx.LEFT, border = 0)
        SUB1.Add(btnx, proportion = 0, flag = wx.LEFT, border = 0)
        left.Add(SUB1, flag = wx.ALIGN_LEFT|wx.LEFT|wx.TOP, border = 0)
        
        SUB2 = wx.BoxSizer( wx.HORIZONTAL )
        str1 = wx.StaticText(panel, label='com:')
        str1.SetFont(font)
        SUB2.Add(str1,proportion = 0, flag=wx.LEFT, border = 8)
        self.tc1 = wx.TextCtrl(panel)
        SUB2.Add(self.tc1, proportion = 1,flag = wx.LEFT|wx.EXPAND)
        btnget = wx.Button(panel, 3, label = 'get')
        SUB2.Add(btnget, flag = wx.RIGHT, border = 0)
        left.Add(SUB2, flag = wx.ALIGN_LEFT|wx.LEFT|wx.EXPAND, border = 0)
        
        SUB3 = wx.BoxSizer( wx.HORIZONTAL )
        self.tc2 = wx.TextCtrl(panel)
        SUB3.Add(self.tc2, proportion = 0,flag = wx.LEFT)
        btnget = wx.Button(panel, 4, label = 'P')
        SUB3.Add(btnget, flag = wx.RIGHT, border = 0)
        self.tc3 = wx.TextCtrl(panel)
        SUB3.Add(self.tc3, proportion = 0,flag = wx.LEFT)
        btnget = wx.Button(panel, 5, label = 'I')
        SUB3.Add(btnget, flag = wx.RIGHT, border = 0)
        self.tc4 = wx.TextCtrl(panel)
        SUB3.Add(self.tc4, proportion = 0,flag = wx.LEFT)
        btnget = wx.Button(panel, 6, label = 'D')
        SUB3.Add(btnget, flag = wx.RIGHT, border = 0)
        
        left.Add(SUB3, flag = wx.ALIGN_LEFT|wx.LEFT, border = 0)
        
        SUB4 = wx.BoxSizer( wx.HORIZONTAL )
        self.tc5 = wx.TextCtrl(panel)
        SUB4.Add(self.tc5, proportion = 0,flag = wx.LEFT)
        btnget = wx.Button(panel, 7, label = 'throttle')
        SUB4.Add(btnget, flag = wx.RIGHT, border = 0)
        self.tc6 = wx.TextCtrl(panel)
        SUB4.Add(self.tc6, proportion = 0,flag = wx.LEFT)
        btnget = wx.Button(panel, 8, label = 'FRONT')
        SUB4.Add(btnget, flag = wx.RIGHT, border = 0)
        self.tc7 = wx.TextCtrl(panel)
        SUB4.Add(self.tc7, proportion = 0,flag = wx.LEFT)
        btnget = wx.Button(panel, 9, label = 'BACK')
        SUB4.Add(btnget, flag = wx.RIGHT, border = 0)
        
        left.Add(SUB4, flag = wx.ALIGN_LEFT|wx.LEFT, border = 0)
        
        SUB5 = wx.BoxSizer( wx.HORIZONTAL )
        self.tc8 = wx.TextCtrl(panel)
        SUB5.Add(self.tc8, proportion = 0,flag = wx.LEFT)
        btnget = wx.Button(panel, 10, label = 'LEFT')
        SUB5.Add(btnget, flag = wx.RIGHT, border = 0)
        self.tc9 = wx.TextCtrl(panel)
        SUB5.Add(self.tc9, proportion = 0,flag = wx.LEFT)
        btnget = wx.Button(panel, 11, label = 'RIGTH')
        SUB5.Add(btnget, flag = wx.RIGHT, border = 0)
        
        left.Add(SUB5, flag = wx.ALIGN_LEFT|wx.LEFT, border = 0)
        
        RIGTHSUB1 = wx.BoxSizer( wx.VERTICAL )
        RIGTHSUB2 = wx.BoxSizer( wx.VERTICAL )
        
        right.Add(RIGTHSUB1, flag = wx.ALIGN_LEFT|wx.LEFT, border = 0)
        right.Add(RIGTHSUB2, flag = wx.ALIGN_LEFT|wx.LEFT, border = 0)
        
        SUB6 = wx.BoxSizer( wx.HORIZONTAL )
        str1 = wx.StaticText(panel, label='P:')
        str1.SetFont(font)
        SUB6.Add(str1,proportion = 0, flag=wx.LEFT, border = 8)
        self.tc10 = wx.TextCtrl(panel)
        SUB6.Add(self.tc10, proportion = 0,flag = wx.LEFT)
        RIGTHSUB1.Add(SUB6, flag = wx.ALIGN_LEFT|wx.LEFT|wx.TOP, border = 0)
        
        SUB7 = wx.BoxSizer( wx.HORIZONTAL )
        str1 = wx.StaticText(panel, label='I:')
        str1.SetFont(font)
        SUB7.Add(str1,proportion = 0, flag=wx.LEFT, border = 8)
        self.tc11 = wx.TextCtrl(panel)
        SUB7.Add(self.tc11, proportion = 0,flag = wx.LEFT)
        RIGTHSUB1.Add(SUB7, flag = wx.ALIGN_LEFT|wx.LEFT, border = 0)
        
        SUB8 = wx.BoxSizer( wx.HORIZONTAL )
        str1 = wx.StaticText(panel, label='D:')
        str1.SetFont(font)
        SUB8.Add(str1,proportion = 0, flag=wx.LEFT, border = 8)
        self.tc12 = wx.TextCtrl(panel)
        SUB8.Add(self.tc12, proportion = 0,flag = wx.LEFT)
        RIGTHSUB1.Add(SUB8, flag = wx.ALIGN_LEFT|wx.LEFT, border = 0)
        
        SUB9 = wx.BoxSizer( wx.HORIZONTAL )
        str1 = wx.StaticText(panel, label='Cycle_Time:')
        str1.SetFont(font)
        SUB9.Add(str1,proportion = 0, flag=wx.LEFT, border = 8)
        self.tc13 = wx.TextCtrl(panel)
        SUB9.Add(self.tc13, proportion = 0,flag = wx.LEFT)
        RIGTHSUB1.Add(SUB9, flag = wx.ALIGN_LEFT|wx.LEFT, border = 0)
        
        SUBA = wx.BoxSizer( wx.HORIZONTAL )
        str1 = wx.StaticText(panel, label='FRONT:')
        str1.SetFont(font)
        SUBA.Add(str1,proportion = 0, flag=wx.LEFT, border = 8)
        self.tc14 = wx.TextCtrl(panel)
        SUBA.Add(self.tc14, proportion = 0,flag = wx.LEFT)
        RIGTHSUB1.Add(SUBA, flag = wx.ALIGN_LEFT|wx.LEFT, border = 0)
        
        SUBB = wx.BoxSizer( wx.HORIZONTAL )
        str1 = wx.StaticText(panel, label='BACK:')
        str1.SetFont(font)
        SUBB.Add(str1,proportion = 0, flag=wx.LEFT, border = 8)
        self.tc15 = wx.TextCtrl(panel)
        SUBB.Add(self.tc15, proportion = 0,flag = wx.LEFT)
        RIGTHSUB1.Add(SUBB, flag = wx.ALIGN_LEFT|wx.LEFT, border = 0)
        
        SUBC = wx.BoxSizer( wx.HORIZONTAL )
        str1 = wx.StaticText(panel, label='LEFT:')
        str1.SetFont(font)
        SUBC.Add(str1,proportion = 0, flag=wx.LEFT, border = 8)
        self.tc16 = wx.TextCtrl(panel)
        SUBC.Add(self.tc16, proportion = 0,flag = wx.LEFT)
        RIGTHSUB2.Add(SUBC, flag = wx.ALIGN_LEFT|wx.LEFT, border = 0)
        
        SUBD = wx.BoxSizer( wx.HORIZONTAL )
        str1 = wx.StaticText(panel, label='RIGHT:')
        str1.SetFont(font)
        SUBD.Add(str1,proportion = 0, flag=wx.LEFT, border = 8)
        self.tc17 = wx.TextCtrl(panel)
        SUBD.Add(self.tc17, proportion = 0,flag = wx.LEFT)
        RIGTHSUB2.Add(SUBD, flag = wx.ALIGN_LEFT|wx.LEFT, border = 0)
       
        SUBE = wx.BoxSizer( wx.HORIZONTAL )
        str1 = wx.StaticText(panel, label='fit_throttle:')
        str1.SetFont(font)
        SUBE.Add(str1,proportion = 0, flag=wx.LEFT, border = 8)
        self.tc18 = wx.TextCtrl(panel)
        SUBE.Add(self.tc18, proportion = 0,flag = wx.LEFT)
        RIGTHSUB2.Add(SUBE, flag = wx.ALIGN_LEFT|wx.LEFT, border = 0)
        
        SUBF = wx.BoxSizer( wx.HORIZONTAL )
        str1 = wx.StaticText(panel, label='error:')
        str1.SetFont(font)
        SUBF.Add(str1,proportion = 0, flag=wx.LEFT, border = 8)
        self.tc19 = wx.TextCtrl(panel)
        SUBF.Add(self.tc19, proportion = 1,flag = wx.LEFT|wx.EXPAND)
        RIGTHSUB2.Add(SUBF, flag = wx.ALIGN_LEFT|wx.LEFT|wx.EXPAND, border = 0)
        
        SUBG = wx.BoxSizer( wx.HORIZONTAL )
        str1 = wx.StaticText(panel, label='speed:')
        str1.SetFont(font)
        SUBG.Add(str1,proportion = 0, flag=wx.LEFT, border = 8)
        self.tc20 = wx.TextCtrl(panel)
        SUBG.Add(self.tc20, proportion = 1,flag = wx.LEFT|wx.EXPAND)
        RIGTHSUB2.Add(SUBG, flag = wx.ALIGN_LEFT|wx.LEFT|wx.EXPAND, border = 0)
        
        SUBG = wx.BoxSizer( wx.HORIZONTAL )
        str1 = wx.StaticText(panel, label='rpy_c')
        str1.SetFont(font)
        SUBG.Add(str1,proportion = 0, flag=wx.LEFT, border = 8)
        self.tc22 = wx.TextCtrl(panel)
        SUBG.Add(self.tc22, proportion = 1,flag = wx.LEFT|wx.EXPAND)
        RIGTHSUB2.Add(SUBG, flag = wx.ALIGN_LEFT|wx.LEFT|wx.EXPAND, border = 0)

        SUBG = wx.BoxSizer( wx.HORIZONTAL )
        str1 = wx.StaticText(panel, label='rpy_e')
        str1.SetFont(font)
        SUBG.Add(str1,proportion = 0, flag=wx.LEFT, border = 8)
        self.tc23 = wx.TextCtrl(panel)
        SUBG.Add(self.tc23, proportion = 1,flag = wx.LEFT|wx.EXPAND)
        RIGTHSUB2.Add(SUBG, flag = wx.ALIGN_LEFT|wx.LEFT|wx.EXPAND, border = 0)

                
        panel.SetSizer(main)
        self.Centre()
        wx.EVT_BUTTON(self, 1, self.start)
        wx.EVT_BUTTON(self, 2, self.stop)
        wx.EVT_BUTTON(self, 3, self.get)
        wx.EVT_BUTTON(self, 4, self.setP)
        wx.EVT_BUTTON(self, 5, self.setI)
        wx.EVT_BUTTON(self, 6, self.setD)
        wx.EVT_BUTTON(self, 7, self.setT)
        wx.EVT_BUTTON(self, 8, self.setA)
        wx.EVT_BUTTON(self, 9, self.setE)
        wx.EVT_BUTTON(self, 10, self.setN)
        wx.EVT_BUTTON(self, 11, self.setR)
        wx.EVT_BUTTON(self, 12, self.startx)
        
    def start(self,event):
        if(ser.isOpen()):
            ser.write('s')

    def stop(self,event):
        if(ser.isOpen()):
            ser.write('q')
            
    def startx(self,event):
        if(ser.isOpen()):
            ser.write('x')
            self.count += 1
            thread = helloThread(self)
            self.threads.append(thread)
            thread.start()
            print "write hello"
            
    def setP(self,event):
        if(ser.isOpen()):
            ser.write('p'+self.tc2.GetValue())   
        
    def setI(self,event):
        if(ser.isOpen()):
            ser.write('i'+self.tc3.GetValue())
             
    def setD(self,event):  
        if(ser.isOpen()):
            ser.write('d'+self.tc4.GetValue())   
    
    def setT(self,event):
        if(ser.isOpen()):
            ser.write('t'+self.tc5.GetValue())  
    
    def setA(self,event):
        if(ser.isOpen()):
            ser.write('a'+self.tc6.GetValue()) 
    
    def setE(self,event):
        if(ser.isOpen()):
            ser.write('e'+self.tc7.GetValue()) 
    
    def setN(self,event):
        if(ser.isOpen()):
            ser.write('n'+self.tc8.GetValue()) 
    
    def setR(self,event):
        if(ser.isOpen()):
            ser.write('r'+self.tc9.GetValue()) 
    
    def get(self,event):
        ser.port = self.tc1.GetValue()
        ser.open()
        if(ser.isOpen()):
            print "get port"
            self.count += 1
            thread = WorkerThread(self)
            self.threads.append(thread)
            thread.start()
    
    def LogMessage(self, msg):
        if(msg[0] == 's'):
            m = msg[1:].split('%')
            pid = m[0].split(' ')
            self.tc10.SetValue(pid[0])
            self.tc11.SetValue(pid[1])
            self.tc12.SetValue(pid[2])
            self.tc13.SetValue(m[1])
            angle = m[2].split(' ')
            self.tc14.SetValue(angle[0])
            self.tc15.SetValue(angle[1])
            error = m[3].split(' ')
            self.tc16.SetValue(error[0])
            self.tc17.SetValue(error[1])
            self.tc18.SetValue(m[4])
            self.tc19.SetValue(m[5])
            self.tc20.SetValue(m[6])
            self.tc22.SetValue(m[7])
            self.tc23.SetValue(m[8])
            
    def EXIT(self, event):
        self.Close()
    
        
class MyApp(wx.App):
    def OnInit(self):
        frame = MyFrame(None, -1, "main.py")
        frame.Show(True)
        self.SetTopWindow(frame)
        return True

app = MyApp(0)
app.MainLoop()