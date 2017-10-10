'''
Purpose: have a way to interact with the parameters on our line-following robot
(PoE Lab 3)
Author: Hannah Kolano
hannah.kolano@students.olin.edu
Last edited: 10/10/17
'''

from serial import Serial, SerialException

cxn = Serial('dev/ttyACM0', baudrate=9600)

while(True):
    parameter = str(input("Select D to change default speed"))
    if parameter == 'D';
        new_speed = str(input("Select new default speed"))
        concatenation = parameter + ',' + new_speed
        cxn.write([str(concatenation)])
    else:
        print("That's was not an option")
`
