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
                # this if is important.  Makes sure that the netowrk of people is only made once
                if self.MainWindow.TotalNumberOfSteps == 0:
                        self.EconManager.BuildCompleteNetwork(self.MainWindow.NumPeople)
                for i in range(self.MainWindow.NumSteps):
                        self.EconManager.DoAStep()
                        PyEconSim.DoEndOfDay()
                        self.MainWindow.TotalNumberOfSteps=self.MainWindow.TotalNumberOfSteps+1
                        if i % 100 ==0:
                                print "on step",self.MainWindow.TotalNumberOfSteps
                                self.MainWindow.Update()
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
        self.NumPeople=1000
        self.NumSteps=100
        self.ui.numPeopleInput.setText(str(self.NumPeople))
        self.ui.numStepsInput.setText(str(self.NumSteps))

        self.Good2Plot=0
        self.ui.Good2PlotInput.setText(str(self.Good2Plot))

        self.Update()


        # Connect a function to be run when a button is pressed.
        self.ui.StartSim.clicked.connect(self.StartSimFunction)
        self.runThread = RunSimThread(self)
        self.ui.Plot.clicked.connect(self.PlotBotton)

        #Connect the input feilds for the number of people and such
        self.ui.numPeopleInput.editingFinished.connect(self.SetNumPeople)
        self.ui.numStepsInput.editingFinished.connect(self.SetNumSteps)

        self.ui.Good2PlotInput.editingFinished.connect(self.SetGood2Plot)
        


    def SetGood2Plot(self):
            x=self.ui.Good2PlotInput.text()
            x=int(x)
            self.Good2Plot=x
            print "Updated good 2 plot"

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
        self.ui.numPeopleInput.setEnabled(False)
        
    def PlotBotton(self):
        print "Plot"

        price = plt.plot(self.runThread.EconManager.GetPriceData(),"ro",linestyle="-",label="Price")
        people = plt.plot(self.runThread.EconManager.GetPopulation(),label="Population")
        demand = plt.plot(self.runThread.EconManager.GetDemands(self.Good2Plot),"g^",label="demand")
        supply=plt.plot(self.runThread.EconManager.GetSupplies(self.Good2Plot),label="supply")


        plt.yscale('log')
        plt.legend()
        plt.show(block=True)



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
