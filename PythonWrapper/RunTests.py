


import sys
from PyQt4 import uic, QtGui, QtCore
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from os.path import join
import matplotlib.pyplot as plt
import PyEconSim


def Tests():
    PyEconSim.UnitTests()


if __name__=='__main__':
    Tests()
