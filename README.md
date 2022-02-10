# TreePlanter (RePlantProject)

This project was developed under the guidance and tutelage of professor Armando Sousa and engineer Rui Coutinho has a semester project applied in the curricular unit of engineering systems at the faculty of engeneering at univerisity of Porto.

The final objective of this project is the creation of a smart autonomous planting machine capable of planting a differently sized seedlings amongs harsh and changable terrain.

The scope of the devolpment portion achived throught this semester resulted in the following prototype.

## Directory Organization 

The code gathered in this directory was organized as follows:

|
Marlin 2.0 (Firmware used)

  |...
  
  |--Config_Adv.h
  
  |--Config.h
  
  |--Marlin.ino
  
 |I2C Testing (I2C and GCODE testing code)
 
MainStateMachine

  |...
  
  |--DCMotorSpeedControl.ino
  
  |--BarrierSens.ino
  
  |--DistanceSens.ino
  
  |--I2CTest.ino
  
  |--LCDLib.h
  
  |--LCDLib.cpp
  
  |--MainStateMachine.ino
  
SerialInterface

  |...
  
  |Interface.py
  
  
  Thusly, the code present in the Marlin folder discribes the firmware used to control the manipulator that moves the drill. This firmware was corrected and altered for the porpouses of this project and to control and combine with the shield RAMPS 1.4 used.
  
  Next the code sections presented in MainStateMachine gather the hardware test code for the different sensores used and a liberarly for the LCD display. This folder also includes the main code used to control this project.
  The folder named SerialInterface presents the code used to create the serial interface, this code was developed using PySide2 and QtCreator.
  
  ## Connections Diagram
  
  
  ## State Machine Diagram
  
  
