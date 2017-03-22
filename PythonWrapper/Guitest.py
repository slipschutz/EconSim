#!/usr/bin/env python2.7

import sys
from PyQt4 import uic, QtGui, QtCore
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from os.path import join
import matplotlib.pyplot as plt
import PyEconSim


from PyQt4.QtCore import QThread


class RunSimThread(QThread):
        def __init__(self):
            QThread.__init__(self)
        def __del__(self):
            self.wait()
        def run(self):
            print "I am gonna RUN"
            while True:
                print "Running"
                self.sleep(2)


# Load the GUI class from the .ui file
(Ui_MainWindow, QMainWindow) = uic.loadUiType('Test.ui')

# Define a class for the main window.
class MainWindow(QtGui.QMainWindow):
    def __init__(self, parent=None):
        # Initialize the GUI itself
        super(MainWindow,self).__init__(parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)


        # Connect a function to be run when a button is pressed.
        self.ui.StartSim.clicked.connect(self.StartSimFunction)
        self.runThread = RunSimThread()
        self.ui.stop.clicked.connect(self.StopBotton)
                
        
    def StartSimFunction(self):
        self.runThread.start()
        self.ui.stop.setEnabled(True)
        
    def StopBotton(self):
        print "Stop"
        self.runThread.terminate()



end__Class_MainWindow=0







if __name__=='__main__':
    # Initialize the qt event loop.
    app = QtGui.QApplication(sys.argv)
    # Initialize and display our main window.
    w = MainWindow()
    w.show()
    # Run the qt event loop, exiting the script when done.
    sys.exit(app.exec_())
