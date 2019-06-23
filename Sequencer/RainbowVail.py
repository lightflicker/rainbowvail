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


# Define the main window function
def main(stdscr):
    # Initialisation of the dictionary list. The list will hold information
    # about the step paramters
    seq = []
    # Initiate the out variable for collecing the Arduino reponse
    out = ''
    # Initiating StepNo
    StepNo = 0
    # Initiating Delimiter character
    Delimiter = ';'
    # Start command - when True, the sequence starts
    start_cmd = False
    # Sequnce start timestamp
    start_t = 0.0
    # Sequence bookmark
    bookmarks = []
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
    # Make getch non-blocking
    stdscr.nodelay(True)
    # Clear window
    stdscr.clear()

    # Start the loop of checking the keboard input
    # and communicating with Arduino
    while True:
        # Get key value
        k = stdscr.getch()

        # Clear keyboard buffer
        curses.flushinp()

        # Key commands
        # Start sequence
        if k == ord('s'):
            start_cmd = True
            start_t = time.time()
            stdscr.addstr(0,2,'SEQUENCE STARTED')
        elif k == ord('q'):
            break
        elif k == curses.KEY_DOWN:
            bookmarks.append('Elapsed time: '
                             + '{:10.1f}'.format(time.time() - start_t)
                             + '       Step no.: ' + str(StepNo))
            i = 11
            for ln in bookmarks:
                stdscr.addstr(i,2,ln)
                i += 1

        # Wait for Arduino 10ms and collect the data
        # if ready for collection
        time.sleep(0.01)
        while ser.inWaiting() > 0:
            out = ser.readline()
            stdscr.addstr(3,2,'Received: ' + str(out[0:4]))
            stdscr.addstr(4,2,'Lenght: ' + str(len(out)) + ' (bytes)')

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
        if out[0:4] == b'C000' and start_cmd == True:
            stdscr.addstr(5,2,'Sending step no.: ' + str(StepNo))
            stdscr.addstr(6,2,dataStr)
            ser.write(dataStr.encode('utf-8'))
            StepNo += 1
            out = ''

        # Collect next step
        if StepNo >= len(seq):
            stdscr.addstr(6,2,'Last step has been reached. Exit')
            ser.close()
            exit()

        # Exit event from Arduino
        if out[0:4] == b'C999':
            stdscr.addstr(6,2,'Exit')
            ser.close()
            exit()
            out = ''

        stdscr.refresh()

wrapper(main)
