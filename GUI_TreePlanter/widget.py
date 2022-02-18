# This Python file uses the following encoding: utf-8
import os
from pathlib import Path
import sys

from PySide2.QtWidgets import QApplication, QPushButton, QSpinBox, QWidget, QLCDNumber, QTextBrowser,QLabel
from PySide2.QtCore import QFile, QTimer
from PySide2.QtUiTools import QUiLoader
from PySide2.QtGui import QPixmap

import serial


class Widget(QWidget):

    def __init__(self):
        super(Widget, self).__init__()
        self.load_ui()
        self.ser = serial.Serial()
        self.tree = "0"
        self.treeCout = 0
        self.re = 0
        self.state = 3
        Connect = self.findChild(QPushButton, "Connect")
        Connect.clicked.connect(self.connect)
        Disconnect = self.findChild(QPushButton, "Disconnect")
        Disconnect.clicked.connect(self.disconnect)
        Disconnect.setEnabled(False)
        Start = self.findChild(QPushButton, "Start")
        Start.clicked.connect(self.start)
        Start.setEnabled(False)
        Stop = self.findChild(QPushButton, "StopBtn")
        Stop.clicked.connect(self.stop)
        Stop.setEnabled(False)
        ReloadBtn = self.findChild(QPushButton, "ReloadBtn")
        ReloadBtn.clicked.connect(self.reload)
        ReloadBtn.setEnabled(False)
        up = self.findChild(QPushButton, "Up")
        up.clicked.connect(self.up)
        up.setEnabled(False)
        down = self.findChild(QPushButton, "Down")
        down.clicked.connect(self.down)
        down.setEnabled(False)
        self.box = self.findChild(QTextBrowser,"textBrowser")
        self.timer = QTimer()
        self.timer.timeout.connect(self.scanSerial)


    def load_ui(self):
        loader = QUiLoader()
        path = os.fspath(Path(__file__).resolve().parent / "form.ui")
        ui_file = QFile(path)
        ui_file.open(QFile.ReadOnly)
        loader.load(ui_file, self)
        ui_file.close()

    def connect(self):
        aux = self.findChild(QSpinBox, "spinBox")
        Port = "COM" + str(aux.value())
        self.ser.port = Port
        self.ser.baudrate = 115200
        self.ser.timeout = 0
        self.ser.open()
        Disconnect = self.findChild(QPushButton, "Disconnect")
        Disconnect.setEnabled(True)
        ReloadBtn = self.findChild(QPushButton, "ReloadBtn")
        ReloadBtn.setEnabled(True)
        Stop = self.findChild(QPushButton, "StopBtn")
        Stop.setEnabled(True)
        Start = self.findChild(QPushButton, "Start")
        Start.setEnabled(True)
        self.timer.start(100)

    def start(self):
        self.ser.write(b"B");

    def stop(self):
        self.ser.write(b"S");

    def reload(self):
        #print(self.re)
        #print(self.state)
        if ((self.state != "003" or self.state != "010") and self.re == 1):
            self.re = 0
            self.ser.write(bytes(self.tree[len(self.tree)-1],'utf-8'))
            up = self.findChild(QPushButton, "Up")
            up.setEnabled(False)
            down = self.findChild(QPushButton, "Down")
            down.setEnabled(False)
        elif((self.state == "003" or self.state == "010") and self.re == 0):
            self.re = 1
            down = self.findChild(QPushButton, "Down")
            down.setEnabled(True)
            up = self.findChild(QPushButton, "Up")
            up.setEnabled(True)

    def up(self):
        if (self.reload and self.treeCout < 6):
            lcd = self.findChild(QLCDNumber, "lcdNumber")
            self.treeCout = 1 + self.treeCout
            self.tree = "00" + str(self.treeCout)
            lcd.display(self.tree)

    def down(self):
        if (self.reload and self.treeCout > 0):
            lcd = self.findChild(QLCDNumber, "lcdNumber")
            self.treeCout = self.treeCout - 1
            self.tree = "00" + str(self.treeCout)
            lcd.display(self.tree)

    def scanSerial(self):
        s = self.ser.read(9)
        if (str(s) != "b''"):
            res = str(s)
            self.box.append(res[2:len(res)-5])
            self.parser(res[2:len(res)-5])

    def disconnect(self):
        self.ser.close()
        self.timer.stop()

    def parser(self, res):
        #print(res)

        if ("St:" in res):
            self.state = res[len(res)-3:len(res)]
            box2 = self.findChild(QTextBrowser,"textBrowser_2")
            str = "Current Machine State:" + self.state
            box2.append(str)
            img = self.findChild(QLabel,"label_4")

            if("001" in self.state or "002" in self.state or "003" in self.state or "010" in self.state or "008" in self.state):
                img.setPixmap(QPixmap("Pos1.jpg"))
                print("here")
            elif("004" in self.state ):
                img.setPixmap(QPixmap("Pos2.jpg"))
            elif("005" in self.state or "006" in self.state):
                img.setPixmap(QPixmap("Pos3.jpg"))
            elif("007" in self.state):
                img.setPixmap(QPixmap("Pos4.jpg"))
        if ("Tr:" in res):
            self.tree = res[len(res)-4:len(res)]
            lcd = self.findChild(QLCDNumber, "lcdNumber")
            lcd.display(self.tree)


if __name__ == "__main__":
    app = QApplication([])
    widget = Widget()
    widget.show()
    sys.exit(app.exec_())
