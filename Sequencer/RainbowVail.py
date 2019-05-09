# -*- coding: utf-8 -*-
"""
Created on Mon Sep 19 21:43:51 2016

@author: mhewelt
"""
import csv
import serial
import sys
import time

#Initialisation of the dictionary list. The list will hold information about the step paramters

seq = []

reader = csv.DictReader(open(sys.argv[1],'rb'))
for line in reader:
    seq.append(line)


#Definition of Step 0
# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=38400,
    timeout=10
)
#ser.isOpen()
#time.sleep(2)
out = ''

print 'Enter your commands below.\r\nInsert "exit" to leave the application.'

input=1
StepNo = 0
Delimiter = ';'
while 1 :
    #Collect the data if ready for collection
    time.sleep(0.01)
    while ser.inWaiting() > 0:
        out = ser.readline()
        print('Received: ' + out + 'Lenght: ' + str(len(out)) + ' (bytes)')


    #Load step paramters
    dataStr =   "<" + str(seq[StepNo]['iTypeC']) 
    dataStr +=  Delimiter + str(seq[StepNo]['fCtr_A'])+Delimiter+str(seq[StepNo]['fCtr_B'])+Delimiter+str(seq[StepNo]['fCtr_C'])+Delimiter+str(seq[StepNo]['fCtr_D'])+Delimiter+str(seq[StepNo]['fCtr_E'])
    dataStr +=  Delimiter + str(seq[StepNo]['fCtr_H_On'])+Delimiter+str(seq[StepNo]['fCtr_S_On'])+Delimiter+str(seq[StepNo]['fCtr_L_On'])
    dataStr +=  Delimiter + str(seq[StepNo]['fCtr_H_Off'])+Delimiter+str(seq[StepNo]['fCtr_S_Off'])+Delimiter+str(seq[StepNo]['fCtr_L_Off'])
    dataStr +=  Delimiter + str(seq[StepNo]['iTypeS'])
    dataStr +=  Delimiter + str(seq[StepNo]['fStrb_A']) + Delimiter + str(seq[StepNo]['fStrb_B']) + Delimiter + str(seq[StepNo]['fStrb_C']) + Delimiter + str(seq[StepNo]['fStrb_D']) + Delimiter + str(seq[StepNo]['fStrb_E'])
    dataStr +=  Delimiter + str(seq[StepNo]['lPattern'])
    dataStr +=  Delimiter + str(seq[StepNo]['fStrb_H_On']) + Delimiter + str(seq[StepNo]['fStrb_S_On']) + Delimiter + str(seq[StepNo]['fStrb_L_On'])
    dataStr +=  Delimiter + str(seq[StepNo]['fStrb_H_Off']) + Delimiter + str(seq[StepNo]['fStrb_S_Off']) + Delimiter + str(seq[StepNo]['fStrb_L_Off'])
    dataStr +=  Delimiter + str(seq[StepNo]['lSeqTime'])
    dataStr +=  ">"
        


        
    #Start event from Arduino
    if out[0:4] == 'C000' :
            print('Sending step no.: ' + str(StepNo) + '\r\n')
            print(dataStr + '\r\n')
            ser.write(dataStr)
            StepNo += 1

    #Collect next step
    if StepNo >= len(seq):
        print('Last step has been reached. Exit')
        ser.close()
        exit()
        
    
    #Exit event from Arduino
    if out[0:4] == 'C999':
        print('Exit')
        ser.close()
        exit()
    
    out = ''
