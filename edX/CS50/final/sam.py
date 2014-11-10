import subprocess

def Temperature_thread():
	try:
		result= "Pass"
		cmd = subprocess.Popen("mpstat | grep all", shell=True, stdout=subprocess.PIPE)
		s1 = 'hello testing temperature'
		line = cmd.stdout.read()		
		list = line.split()
		usr = list[3]
		sys = list[5]
		s1 = usr + ' ' + sys
		print s1
				
	except  subprocess.CalledProcessError, e:
		print 'error'



Temperature_thread()
