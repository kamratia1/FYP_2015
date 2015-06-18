from PyQt4 import QtCore, QtGui
import serial

global_vref = 3.3
pwm_high = 3.0

class Meter(QtGui.QProgressBar):
    def __init__(self):
        super(Meter, self).__init__()
        self.setFormat("%v / 4095")
        self.setStyleSheet(" QProgressBar { border: 1px solid #d0d0d0; border-radius: 0px; text-align: center; } QProgressBar::chunk {background-color: #3add36; width: 1px;}")
        self.setRange(0,4095)

    def setValueF(self, value):
        self.setFormat("%v / 4095 ({:.2f} V)".format(value*global_vref/4095))
        self.setValue(value)

class ValueLabel(QtGui.QLabel):
    def __init__(self, maxValue, vRef):
        super(ValueLabel, self).__init__()
        self.maxValue = maxValue
        self.vRef = vRef
        self.setText("0 / {} (0 V)".format(maxValue))
        self.setMinimumSize(100,10)
        self.setSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)

    def updateValue(self, val):
        self.setText("{} / {} ({:.2f} V)".format(val, self.maxValue, val*self.vRef/self.maxValue))

class ValueLabel2(QtGui.QLabel):
    def __init__(self, maxValue):
        super(ValueLabel2, self).__init__()
        self.maxValue = maxValue
        self.setText("0V")
        self.setMinimumSize(100,10)
        self.setSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)

    def updateValue(self, val):
        self.setText("{}V".format(val))

class Window(QtGui.QWidget):
    def __init__(self):
        super(Window, self).__init__()

        self.control()
        self.measurement()

        layout = QtGui.QVBoxLayout()
        layout.addWidget(self.controlGroup)
        layout.addWidget(self.measureGroup)
        self.setLayout(layout)

        self.resize(600,100)
        self.setWindowTitle("Final Year Project - Kishan Amratia")

        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.handler)
        self.timer.start(40)

    def handler(self):
        sin = ser.readline()
        #print sin
        if sin:
            values = sin.split(',')
            values[-1] = values[-1].strip() # Get rid of the newline on the last value
            if values[0] == 'k':
                print values[1]
            else:
                print values
                self.CapacitanceMeter.setValueF(int(values[0]))
                #self.HV_Est.setValueF(int(values[1]))

    def measurement(self):
        self.measureGroup = QtGui.QGroupBox("Measurement")

        self.CapacitanceMeterLabel = QtGui.QLabel("Capacitance")
        self.CapacitanceMeter = Meter()

        mLayout = QtGui.QGridLayout()
        mLayout.setSpacing(5)
        mLayout.addWidget(self.CapacitanceMeterLabel, 0,0)
        mLayout.addWidget(self.CapacitanceMeter, 0,1)

        self.measureGroup.setLayout(mLayout)

    def control(self):
        self.controlGroup = QtGui.QGroupBox("Control")

        self.HVEnable = QtGui.QCheckBox("High Voltage Converter ON/OFF")
        self.HVEnable.clicked.connect(self.updateHV)

        HV_AdjustLabel = QtGui.QLabel("High Voltage Adjust:")
        self.HV_Adjust = QtGui.QSlider(QtCore.Qt.Horizontal)
        self.HV_Adjust.setRange(0, 160)
        self.HV_Adjust.sliderReleased.connect(self.updateControlSettings)
        
        self.HV_AdjustBox = ValueLabel2(160)
        self.HV_Adjust.valueChanged.connect(self.HV_AdjustBox.updateValue)
       
        controlsLayout = QtGui.QGridLayout()
        controlsLayout.setSpacing(5)
        controlsLayout.addWidget(self.HVEnable, 0, 0)

        controlsLayout.addWidget(HV_AdjustLabel, 1,0)
        controlsLayout.addWidget(self.HV_AdjustBox, 1,4)
        controlsLayout.addWidget(self.HV_Adjust, 1,3)

        self.controlGroup.setLayout(controlsLayout)

    def updateHV(self):
        print "HV =", int(self.HVEnable.isChecked())
        ser.write("s,1,{}\n".format(int(self.HVEnable.isChecked())))

    def updateControlSettings(self):
        print "changed slider settings, now:", self.HV_Adjust.sliderPosition()
        #print "set,2,{}.".format(self.HV_Adjust.sliderPosition())
        ser.write("s,2,{}\n".format(self.HV_Adjust.sliderPosition()))
        #ser.write("set, 3,{}\n".format(self.MirrorControl.sliderPosition()))

# Get list of active serial ports
def enumerate_serial_ports():
   available = []
   for i in range(256):
       try:
           s = serial.Serial(i)
           available.append(s.portstr)
           s.close()
       except serial.SerialException:
           pass
   return available

if __name__ == '__main__':

    import sys
    '''
    app = QtGui.QApplication(sys.argv)
    window = Window()
    window.show()
    sys.exit(app.exec_())
    '''
    # Populate list of serial ports
    ports = enumerate_serial_ports()
    if ports:
        print "Connected on", ports[0]
        ser = serial.Serial(ports[0], 115200, timeout=0.6)
        app = QtGui.QApplication(sys.argv)
        window = Window()
        window.show()
        sys.exit(app.exec_())
    else:
        print "No serial ports found."