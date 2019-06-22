# -*- coding: utf-8 -*-
"""
Created on Mon Sep 19 21:43:51 2016

@author: M.Hewelt
Licence: GPL-3.0
"""
import csv
import serial
import sys
import time
import curses
from curses import wrapper

# Initialisation of the dictionary list. The list will hold information
# about the step paramters
seq = []
# Initiate the out variable for collecing the Arduino reponse
out = ''
# Initiating StepNo
StepNo = 0
# Initiating Delimiter character
Delimiter = ';'

# Open sequence csv file
reader = csv.DictReader(open(sys.argv[1], 'rt', encoding="utf8"))
# Read all lines into the seq list
for line in reader:
    seq.append(line)

# Configure serial connection (the parameters differs idepending
# on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=38400,
    timeout=10)


while 1:
    # Collect the data if ready for collection
    time.sleep(0.01)
    while ser.inWaiting() > 0:
        out = ser.readline()
        print('Received: ' + out.decode('utf-8') +
              'Lenght: ' + str(len(out)) + ' (bytes)')

    # Load step paramters
    dataStr = "<" + str(seq[StepNo]['iTypeC'])
    dataStr += Delimiter + str(seq[StepNo]['fCtr_A'])
    dataStr += Delimiter + str(seq[StepNo]['fCtr_B'])
    dataStr += Delimiter + str(seq[StepNo]['fCtr_C'])
    dataStr += Delimiter + str(seq[StepNo]['fCtr_D'])
    dataStr += Delimiter + str(seq[StepNo]['fCtr_E'])
    dataStr += Delimiter + str(seq[StepNo]['fCtr_H_On'])
    dataStr += Delimiter + str(seq[StepNo]['fCtr_S_On'])
    dataStr += Delimiter + str(seq[StepNo]['fCtr_L_On'])
    dataStr += Delimiter + str(seq[StepNo]['fCtr_H_Off'])
    dataStr += Delimiter + str(seq[StepNo]['fCtr_S_Off'])
    dataStr += Delimiter + str(seq[StepNo]['fCtr_L_Off'])
    dataStr += Delimiter + str(seq[StepNo]['iTypeS'])
    dataStr += Delimiter + str(seq[StepNo]['fStrb_A'])
    dataStr += Delimiter + str(seq[StepNo]['fStrb_B'])
    dataStr += Delimiter + str(seq[StepNo]['fStrb_C'])
    dataStr += Delimiter + str(seq[StepNo]['fStrb_D'])
    dataStr += Delimiter + str(seq[StepNo]['fStrb_E'])
    dataStr += Delimiter + str(seq[StepNo]['lPattern'])
    dataStr += Delimiter + str(seq[StepNo]['fStrb_H_On'])
    dataStr += Delimiter + str(seq[StepNo]['fStrb_S_On'])
    dataStr += Delimiter + str(seq[StepNo]['fStrb_L_On'])
    dataStr += Delimiter + str(seq[StepNo]['fStrb_H_Off'])
    dataStr += Delimiter + str(seq[StepNo]['fStrb_S_Off'])
    dataStr += Delimiter + str(seq[StepNo]['fStrb_L_Off'])
    dataStr += Delimiter + str(seq[StepNo]['lSeqTime'])
    dataStr += ">"

    # Start event from Arduino
    if out[0:4] == b'C000':
        print('Sending step no.: ' + str(StepNo) + '\r\n')
        print(dataStr + '\r\n')
        ser.write(dataStr.encode('utf-8'))
        StepNo += 1

    # Collect next step
    if StepNo >= len(seq):
        print('Last step has been reached. Exit')
        ser.close()
        exit()

    # Exit event from Arduino
    if out[0:4] == b'C999':
        print('Exit')
        ser.close()
        exit()

    out = ''
