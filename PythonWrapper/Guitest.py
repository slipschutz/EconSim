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
        def __init__(self,MainWin):
            QThread.__init__(self)
            self.EconManager = PyEconSim.EconomicActorManager()
            self.MainWindow=MainWin
        def __del__(self):
            self.wait()


        def run(self):
                self.EconManager.BuildCompleteNetwork(self.MainWindow.NumPeople)
                for i in range(self.MainWindow.NumSteps):
                        self.EconManager.DoAStep()
                        PyEconSim.DoEndOfDay()
                        self.MainWindow.TotalNumberOfSteps=self.MainWindow.TotalNumberOfSteps+1
                        if i % 100 ==0:
                                print "on step",i
                        self.MainWindow.Update()



# Load the GUI class from the .ui file
(Ui_MainWindow, QMainWindow) = uic.loadUiType('Test.ui')

# Define a class for the main window.
class MainWindow(QtGui.QMainWindow):
    def __init__(self, parent=None):
        # Initialize the GUI itself
        super(MainWindow,self).__init__(parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.TotalNumberOfSteps=0
        self.NumPeople=0
        self.NumSteps=0

        self.Update()


        # Connect a function to be run when a button is pressed.
        self.ui.StartSim.clicked.connect(self.StartSimFunction)
        self.runThread = RunSimThread(self)
        self.ui.stop.clicked.connect(self.StopBotton)

        #Connect the input feilds for the number of people and such
        self.ui.numPeopleInput.editingFinished.connect(self.SetNumPeople)
        self.ui.numStepsInput.editingFinished.connect(self.SetNumSteps)
        


        
    def SetNumPeople(self):
            x=self.ui.numPeopleInput.text()
            x=int(x)
            self.NumPeople=x
            print "Updated Num People"

    def SetNumSteps(self):
            x=self.ui.numStepsInput.text()
            x=int(x)
            self.NumSteps=x
            print "Updated Num Steps"
        
    def StartSimFunction(self):
        self.runThread.start()
        self.ui.stop.setEnabled(True)
        
    def StopBotton(self):
        print "Stop"
        self.runThread.terminate()

    def Update(self):
            self.ui.TotalNumStepLabel.setText(QString("Total Steps "+str(self.TotalNumberOfSteps)))



end__Class_MainWindow=0







if __name__=='__main__':
    # Initialize the qt event loop.
    app = QtGui.QApplication(sys.argv)
    # Initialize and display our main window.
    w = MainWindow()
    w.show()
    # Run the qt event loop, exiting the script when done.
    sys.exit(app.exec_())
