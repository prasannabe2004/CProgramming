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
test_list= sorted(["CPU", "MEM", "HDD", "NW"], key=lambda s: s.upper())
# save all hardware test threads in this array(?) or collection(?)
hw_threads = []

# definition of fifo queue
q = Queue.Queue()

#defintion of port status  - as dictionary
hw_status = {}			# Use {} to define a dictionary.  Use [] to access dictionary elements

# global vars
BLACK= None
RED=  None
GREEN=  None
YELLOW=  None
BLUE=  None
MAGENTA=  None
CYAN=  None
WHITE=  None
WHITE_ON_MAGENTA= None
WHITE_ON_BLACK= None
debug_print= True
the_screen= None


HW_TEST_LINE= 7
HW_TEST_COL= 10+1

def dprint(s) :
	if debug_print :
		the_screen.move(HW_TEST_LINE+len(test_list)+5,0)
		the_screen.insdelln(-1)
		the_screen.addstr(HW_TEST_LINE+len(test_list)+13, 0, s, WHITE)

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

def CPU_thread():
	try:
		result= "Pass"
		cmd = subprocess.Popen("dstat -c 1 1 | tail -1", shell=True, stdout=subprocess.PIPE)
		line = cmd.stdout.read()		
		list = line.split()
		usr = list[0]
		sys = list[1]
		s1 = 'usr=' + usr + ' ' + 'sys=' + sys
		q.put(["CPU", s1])
	except  subprocess.CalledProcessError, e:
		q.put(["CPU", "Not implemented"])

def start_CPU_test(arg):
	the_name= "CPU"
	t1 = Thread(target=CPU_thread, name= the_name, args=())
	t1.daemon = True
	hw_threads.append(t1)
	t1.start()

def MEM_thread(arg):
	try:
		result= "Pass"
		cmd = subprocess.Popen("free -mh | grep +", shell=True, stdout=subprocess.PIPE)
		line = cmd.stdout.read()		
		list = line.split()
		used_mem = list[2]
		free_mem = list[3]
		s1 = 'used=' + used_mem + ' ' + 'free=' + free_mem
		q.put(["MEM", s1])
	except  subprocess.CalledProcessError, e:
		q.put(["MEM", "Not implemented"])

def start_MEM_test(arg):
	the_name= "MEM " + str(arg)
	t = Thread(target=MEM_thread, name= the_name, args=(arg,))
	t.daemon = True
	hw_threads.append(t)
	t.start()

def HDD_thread(arg):
	try:
		result= "Pass"
		cmd = subprocess.Popen("iostat | grep sda", shell=True, stdout=subprocess.PIPE)
		line = cmd.stdout.read()		
		list = line.split()
		read_bytes = list[2]
		write_bytes = list[3]
		s1 = 'read=' + read_bytes + 'kBs' + ' ' + 'write=' + write_bytes + 'kBs'
		q.put(["HDD", s1])
	except  subprocess.CalledProcessError, e:
		q.put(["HDD", "Not implemented"])

def start_HDD_test(arg):
	the_name= "HDD " + str(arg)
	t = Thread(target=HDD_thread, name= the_name, args=(arg,))
	t.daemon = True
	hw_threads.append(t)
	t.start()

def NW_thread(arg):
	try:
		result= "Pass"
		cmd = subprocess.Popen("dstat -n 1 1 | tail -1", shell=True, stdout=subprocess.PIPE)
		line = cmd.stdout.read()		
		list = line.split()
		recv_bytes = list[0]
		send_bytes = list[1]
		s1 = 'recv=' + recv_bytes + ' ' + 'send=' + send_bytes
		q.put(["NW", s1])
	except  subprocess.CalledProcessError, e:
		q.put(["NW", "Not implemented"])

def start_NW_test(arg):
	the_name= "NW " + str(arg)
	t = Thread(target=NW_thread, name= the_name, args=(arg,))
	t.daemon = True
	hw_threads.append(t)
	t.start()


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

	dprint( "Testing " + str(tests))
	arg= "[]"
	nlist=[]
	if "CPU" in tests:
		print_test("CPU")
		start_CPU_test(arg)
	if "MEM" in tests:
		print_test("MEM")
		start_MEM_test(arg)
	if "HDD" in tests:
		print_test("HDD")
		start_HDD_test(arg)
	if "NW" in tests:
		print_test("NW")
		start_NW_test(arg)


	if debug_print :
		the_screen.refresh()


# Each HW test runs in its own thread and sends results back to the main thread via a queue.
def read_q():
	status= True
	while not q.empty():
		m= q.get()
		dprint("++ " + str(m))
		if m[0] == "CPU":
			hw_status["CPU"] = m[1]
		elif m[0] == "MEM":
			hw_status["MEM"] = m[1]
		elif m[0] == "HDD":
			hw_status["HDD"] = m[1]
		elif m[0] == "NW":
			hw_status["NW"] = m[1]


		if debug_print :
			print_status(m[0], m[1])
			the_screen.refresh()
	return status

def getsysinfo():
	cmd = subprocess.Popen("cat /proc/cpuinfo | grep name", shell=True, stdout=subprocess.PIPE)
	line = cmd.stdout.read()		
	list = line.split()
	s1 = list[3] + ' ' + list[4] + ' ' + list[5]
	return s1

def run_tests(tests):
	the_screen.move(0,0)
	the_screen.clrtoeol()
	the_screen.addstr(0, 0, "Running tests", WHITE_ON_MAGENTA | curses.A_BOLD)
	the_screen.addstr(HW_TEST_LINE-1, 10, "Test results:", WHITE_ON_BLACK | curses.A_BOLD)
	the_screen.addstr(HW_TEST_LINE-4, 10, "Processor: "+getsysinfo(), WHITE_ON_BLACK | curses.A_BOLD)
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

	the_screen.addstr(0, 0, "Press space key to toggle tests,arrow keys to move,a to select all,r to run,w to while, q to exit", WHITE_ON_MAGENTA | curses.A_BOLD)
	the_screen.refresh()

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

	#screen.addstr(0, 0, "Connect all cables and press enter", WHITE_ON_MAGENTA | curses.A_BOLD)
	#screen.nodelay(0)			# returns on keypresses
	#screen.getch()
	screen.move(0,0)
	screen.clrtoeol()

	curses.textpad.rectangle(screen,HW_TEST_LINE-5,0,HW_TEST_LINE+len(test_list)+3,81)
	for i in range(HW_TEST_LINE-4, HW_TEST_LINE+len(test_list)+3):
		for j in range(1, 80):
			the_screen.addch(i, j, ' ', WHITE_ON_BLACK)

	display_all_tests(screen)		# Not needed
	run_tests(test_list)
	retry_list= []

	#screen.nodelay(0)			# getch returns only when keypresses
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

		elif (key == ord('W')) or (key == ord('w')) :
			while True:
				retry_list=[]
				for item in test_list :
					retry_list.append(item)
				print_status(item, None)
				screen.refresh()
				run_tests(retry_list)
				time.sleep(1)

		elif (key == ord('Q')) or (key == ord('q')):
			break
		else:
			pass


# Begin here
hw_status.clear()
curses.wrapper(my_function)
f = open('./hwtest_results.txt', 'a')
f.write("\n***********************************************************************\n")
f.close()




