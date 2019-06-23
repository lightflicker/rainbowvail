# -*- coding: utf-8 -*-
"""
Created on Mon Sep 19 21:43:51 2016

@author: M.Hewelt
Licence: GPL-3.0
"""
import curses
from curses import wrapper
import time
def main(stdscr):
    # Make getch non-blockinh
    stdscr.nodelay(True)
    # Create window object that represents the terminal window
    stdscr.clear()
    # Init begining
    start_t = time.time()
    while True:
        # Here store the key value
        c = stdscr.getch()
        curses.flushinp()
        # If user presses 's' start restart timer
        if c == ord('s'):
            start_t = time.time()
            stdscr.addstr(0,0,"Time restarted\n")
            stdscr.refresh()
        # If user presses Arrow UP display current delta
        elif c == curses.KEY_UP:
            stdscr.addstr(1,0,"Current delta: " + str(time.time() - start_t)
                          + "\n")
        elif c == ord('q'):
            stdscr.addstr("Exiting ....\n")
            stdscr.refresh()
            time.sleep(1)
            break
        stdscr.hline(50,10,'|',2)
        time.sleep(0.05)

wrapper(main)

