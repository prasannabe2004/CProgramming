# -*- coding: utf-8 -*-			# recomended but doesn't fully work
import sys
reload(sys)				# not entirely a good practice to make 
sys.setdefaultencoding("utf-8")		# this work without an exception
import subprocess
import os
from threading import Thread
import threading
import Queue
import time
import re
import telnetlib
#import locale
import curses.textpad
import curses.wrapper
import datetime


# start logging function
import datetime
import logging
LOG_FILENAME = 'main.log'
start_time= datetime.datetime.now()
logging.basicConfig(filename=LOG_FILENAME, level=logging.DEBUG, )
logging.debug("Running hwtest at " + str(start_time))

# open file descriptor for /dev/null
FNULL=open(os.devnull,'w')
#FTRACE=open("trace.txt",'w')

# Master list of all HW tests
#test_list= sorted(["Ethernet", "GSM", "Wifi", "oneWire", "AR429", "CFAST", "USB", "Display", "GPIO", "SSD", "Temperature"], key=lambda s: s.upper())
test_list= sorted(["Temperature"], key=lambda s: s.upper())

# save all hardware test threads in this array(?) or collection(?)
hw_threads = []

# definition of fifo queue
q = Queue.Queue()

#defintion of port status  - as dictionary
hw_status = {}			# Use {} to define a dictionary.  Use [] to access dictionary elements



HW_TEST_LINE= 7
HW_TEST_COL= 10+1

def dprint(s) :
	if debug_print :
		the_screen.move(HW_TEST_LINE+len(test_list)+5,0)
		the_screen.insdelln(-1)
		the_screen.addstr(HW_TEST_LINE+len(test_list)+13, 0, s, WHITE)

def Temperature_thread():
	try:
		result= "Pass"
		cmd = subprocess.Popen('mpstat | grep all', shell=True, stdout=subprocess.PIPE)
		s1 = 'hello testing temperature'
		line = cmd.stdout.read()		
		list = line.split()
		usr = list[3]
		sys = list[5]
		s1 = usr + ' ' + sys
		q.put(["Temperature", result + "\t" + s1])
	except  subprocess.CalledProcessError, e:
		q.put(["Temperature", "Not available"])


def start_Temperature_test():
	the_name= "Temperature"
	t1 = Thread(target=Temperature_thread, name= the_name, args=())
	t1.daemon = True
	hw_threads.append(t1)
	t1.start()

#HOST= "192.168.0.1"
HOST= "TestComputer"
USER= "anonymous"
PASSWORD= ""


def run_hw_tests(tests):

	now = datetime.datetime.now()
	now = str(now)
	f = open('./hwtest_results.txt', 'w')
	f.write("***********************************************************************\n\n")
	f.write("Test Report: ")
	f.write(now[0:16])
	f.write('\n')
	f.write("-----------------------------------------------------------------------\n")
	f.write("Test				Result\n")
	f.write("-----------------------------------------------------------------------\n")
	f.close()

	def print_test(test) :
		if test in test_list :
			i= test_list.index(test)
			if (i>=0) and (i <= len(test_list)) :
				the_screen.addstr(HW_TEST_LINE+i, 10, "[ ] " +test, WHITE_ON_BLACK | curses.A_BOLD)

	#dprint( "Testing " + str(tests))
	arg= "[]"
	nlist=[]
	if "Temperature" in tests:
		print_test("Temperature")
		start_Temperature_test()

	if debug_print :
		the_screen.refresh()


# Each HW test runs in its own thread and sends results back to the main thread via a queue.
def read_q():
	status= True
	while not q.empty():
		m= q.get()
		dprint("++ " + str(m))
		if m[0] == "Temperature" :
			hw_status["Temperature"] = m[1]

	return status


def run_tests(tests):
	the_screen.move(0,0)
	the_screen.clrtoeol()
	the_screen.addstr(0, 0, "Running tests", WHITE_ON_MAGENTA | curses.A_BOLD)
	the_screen.addstr(HW_TEST_LINE-2, 10, "Test results:", WHITE_ON_BLACK | curses.A_BOLD)
	# Start hw test threads
	run_hw_tests(tests)
	# Wait for all threads to complete
	timeout= 30				# wait up to 30 seconds
	while threading.activeCount() > 1 :	# wait for all threads (except main) to complete
		time.sleep(0.100)	# sleep 100 ms
		timeout = timeout - 0.100
		read_q()		# read pending results
		if timeout < 0 :		# out of time
			for t in threading.enumerate() :
				if t is threading.currentThread() :
					continue
				dprint("** Error " + t.getName() + " is still running.")
			break;

	the_screen.addstr(0, 0, "Press space key to toggle tests, tab or arrow keys to move, a to select all, r to run, q to exit", WHITE_ON_MAGENTA | curses.A_BOLD)
	the_screen.refresh()


# screen movement functions
def cursor_up(screen):
	row, col= screen.getyx()
	if row <= HW_TEST_LINE or row > (HW_TEST_LINE + len(test_list) -1) :
		row= (HW_TEST_LINE + len(test_list) -1)
	else :
		row = row-1
	col= HW_TEST_COL
	screen.move(row, col)
	screen.refresh()

def cursor_down(screen):
	row, col= screen.getyx()
	if row < HW_TEST_LINE :
		row= HW_TEST_LINE
	elif row >= (HW_TEST_LINE + len(test_list) -1) :
		row= HW_TEST_LINE
	else :
		row= row+1
	col= HW_TEST_COL
	screen.move(row, col)
	screen.refresh()

def cursor_tab(screen):
	row, col= screen.getyx()
	if row < HW_TEST_LINE :
		row= HW_TEST_LINE
	elif row >= (HW_TEST_LINE + len(test_list) -1) :
		row= HW_TEST_LINE
	else :
		row= row+1
	col= HW_TEST_COL
	screen.move(row, col)
	screen.refresh()

def cursor_backtab(screen):
	row, col= screen.getyx()
	if row <= HW_TEST_LINE :
		row= HW_TEST_LINE + len(test_list) -1
	elif row >= HW_TEST_LINE  +len(test_list) :
		row= HW_TEST_LINE+ len(test_list) -1
	else :
		row= row-1
	col= HW_TEST_COL
	screen.move(row, col)
	screen.refresh()


def cursor_left(screen):
	row0, col0= screen.getbegyx()
	row, col= screen.getyx()
	if col > col0 :
		col= col -1
	screen.move(row, col)
	screen.refresh()

def cursor_right(screen):
	nrows, ncols= screen.getmaxyx()
	row, col= screen.getyx()
	if col < (ncols-1) :
		col= col +1
	screen.move(row, col)
	screen.refresh()


def display_all_tests(screen) :
	for item in test_list:
		row= test_list.index(item)
		screen.addstr(row+HW_TEST_LINE, 10, "[ ] " +item,WHITE_ON_BLACK)
		screen.move(row+HW_TEST_LINE, 10+1)
		screen.addch(row+HW_TEST_LINE, 10+1, curses.ACS_DIAMOND, WHITE_ON_BLACK)
	screen.move(HW_TEST_LINE, 10+1)
	screen.refresh()			# redraw the screen with new output


def print_status(test, result) :
	if test not in test_list :
		return
	if result == None :
		color= WHITE_ON_BLACK
	elif result.startswith("Pass") :
		color= GREEN
	elif result.startswith("Fail") :
		color= RED
	else :
		color= WHITE_ON_BLACK
	i= test_list.index(test)
	if (i>=0) and (i <= len(test_list)) :
		the_screen.move(HW_TEST_LINE+i,0)
		the_screen.clrtoeol()
		the_screen.addstr(HW_TEST_LINE+i, 10, "[ ] " +test, color | curses.A_BOLD)
		f = open('./hwtest_results.txt', 'a')
		f.write(test)
		if result != None :
			the_screen.addstr(HW_TEST_LINE+i, 34, result, color | curses.A_BOLD)
			if len(test) >= 8:
				result_output = "\t\t\t"+result+'\n'
			else:
				result_output = "\t\t\t\t"+result+'\n'
			f.write(result_output)
		f.close()
		the_screen.chgat(HW_TEST_LINE+i, 1, 79, color | curses.A_BOLD)
		curses.textpad.rectangle(the_screen,HW_TEST_LINE-5,0,HW_TEST_LINE+len(test_list)+3,81)


def setup_colors(screen) :
	# setup the given color on black background
	global RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, WHITE_ON_MAGENTA, WHITE_ON_BLACK
	if curses.has_colors() :
		for i in xrange(1 ,curses.COLORS):
			curses.init_pair(i, i, curses.COLOR_BLACK)
		curses.init_pair(8, curses.COLOR_WHITE, curses.COLOR_MAGENTA)
		curses.init_pair(9, curses.COLOR_WHITE, curses.COLOR_BLUE)
		#BLACK= curses.color_pair(0)		# color_pair(0) hardwired to white on black
		#WHITE= curses.color_pair(0)
		RED= curses.color_pair(1)
		GREEN= curses.color_pair(2)
		YELLOW= curses.color_pair(3)
		BLUE= curses.color_pair(4)
		MAGENTA= curses.color_pair(5)
		CYAN= curses.color_pair(6)
		WHITE_ON_BLACK= curses.color_pair(7)
		WHITE_ON_MAGENTA= curses.color_pair(8)
		WHITE= curses.color_pair(9)
	else :						# to be complete, almost certainly not needed
		WHITE= curses.color_pair(0)
		RED= curses.color_pair(0)
		GREEN= curses.color_pair(0)
		YELLOW= curses.color_pair(0)
		BLUE= curses.color_pair(0)
		MAGENTA= curses.color_pair(0)
		CYAN= curses.color_pair(0)
		WHITE_ON_BLACK= curses.color_pair(0)
		WHITE_ON_MAGENTA= curses.color_pair(0)


def my_function(stdscr):
	screen = stdscr
	global the_screen
	the_screen = stdscr
	the_screen.attron(curses.A_ALTCHARSET)		# to display curses.ACS_DIAMOND

	curses.flushinp()			# Flush all input buffers.
	setup_colors(screen)
	screen.erase()
	screen.bkgd(' ', WHITE)

	screen.move(0,0)
	screen.clrtoeol()

	curses.textpad.rectangle(screen,HW_TEST_LINE-5,0,HW_TEST_LINE+len(test_list)+3,81)
	for i in range(HW_TEST_LINE-4, HW_TEST_LINE+len(test_list)+3):
		for j in range(1, 80):
			the_screen.addch(i, j, ' ', WHITE_ON_BLACK)

	display_all_tests(screen)		# Not needed
	run_tests(test_list)
	retry_list= []

	screen.nodelay(1)			# getch returns immediately
	while True:
		time.sleep(0.005)		# sleep 50 ms
		read_q()
		key= screen.getch()
		if key == curses.ERR:		# no keypress
			continue
		elif key == curses.KEY_UP:
			cursor_up(screen)
		elif key == curses.KEY_DOWN:
			cursor_down(screen)
		elif (key == ord('\t')) or (key == ord('\n')):
			cursor_tab(screen)
		elif key == curses.KEY_BTAB:
			cursor_backtab(screen)
		elif key == ord(' '):
			row, col= screen.getyx()
			if (row >= HW_TEST_LINE) and (row <= (HW_TEST_LINE+len(test_list))) :
				t= test_list[row-HW_TEST_LINE]
				if t in retry_list :
					retry_list.remove(t)
					screen.addch(row, col, ' ', WHITE_ON_BLACK)
				else :
					retry_list.append(t)
					screen.addch(row, col, curses.ACS_DIAMOND, WHITE_ON_BLACK)
				cursor_left(screen)
				cursor_tab(screen)
		elif (key == ord('A')) or (key == ord('a')) :
			retry_list=[]
			for item in test_list :
				retry_list.append(item)
			for row in range(HW_TEST_LINE, HW_TEST_LINE+len(test_list)) :
				screen.addch(row, HW_TEST_COL, curses.ACS_DIAMOND, WHITE_ON_BLACK)
			screen.move(HW_TEST_LINE, HW_TEST_COL)

		elif (key == ord('R')) or (key == ord('r')) :
			for item in retry_list:
				hw_status[item]= None
				print_status(item, None)
			screen.refresh()
			run_tests(retry_list)
			retry_list=[]

		elif (key == ord('Q')) or (key == ord('q')):
			break
		else:
			pass

hw_status.clear()
curses.wrapper(my_function)
f = open('./hwtest_results.txt', 'a')
f.write("\n***********************************************************************\n")
f.close()




