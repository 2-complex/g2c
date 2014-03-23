
import shutil
import sys
import os


def replace_in_file(path, a, b):
	f = open(path, 'r')
	s = f.read().replace(a, b)
	f.close()
	f = open(path, 'w')
	f.write(s)
	f.close()


def replace_in_filename(path, a, b):
	j = len(path)-1
	if path[-1]!='/':
		j-=1
	while j>0 and path[j]!='/':
		j-=1
	base = path[:j]
	filename = path[j:]
	if a in filename:
		newname = filename.replace(a, b)
		os.rename(path, base+newname)
		return base+newname
	return path


def deepreplace(path, a, b):
	l = []
	if path == '':
		path = '.'

	print path

	if os.path.isdir(path):
		l = os.listdir(path)
		for f in l:
			deepreplace(os.path.join(path, f), a, b)
	else:
		replace_in_file(path, a, b)

	replace_in_filename(path, a, b)


def go():
	print "****************************************************************************************"
	print "* Interactive script for creating a project directory based on a direcotry in blanks/. *"
	print "****************************************************************************************"
	print ""

	script_path = os.path.dirname(os.path.realpath(__file__))
	g2c_dir = os.path.realpath("..")

	blanks_dir = os.path.join(g2c_dir, "projects", "blanks")

	while not os.path.isdir(blanks_dir):
		print "Could not find blanks/ directory " + blanks_dir + ".  Where is g2c?"
		g2c_dir = raw_input("Enter path to g2c root directory: ")
		blanks_dir = os.path.join(g2c_dir, "projects", "blanks")
		print ""

	print "At what path would you like to place the new project directory?"
	target_path = raw_input("(default = " + os.getcwd() + ") ? ")
	print ""

	blanks = os.listdir(blanks_dir)
	def visible(s):
		return s[0] != '.'

	blanks = filter(visible, blanks)

	blank_name = ""
	while blank_name not in blanks:
		print "What blank project would you like to copy from? (" + ", ".join(blanks) + ")"
		blank_name = raw_input("(select one of: " + ", ".join(blanks) + ") ? ")
		print ""

	source_path = os.path.join(blanks_dir, blank_name)
	destination_path = os.path.join(os.getcwd(), blank_name)

	print "Will make a project by copying..." 
	print "from: " + source_path
	print "to: " + destination_path
	print ""

	if os.path.exists(destination_path):
		response = ''
		while not response in ['y', 'n', 'yes', 'no']:
			print destination_path + " already exists.  Would you like to replace it?"
			response = raw_input("This will delete destination_path and make a new one (y/n) ? ").lower()
		if not response.startswith('y'):
			print "Goodbye."
			print ""
			return

	if os.path.exists(destination_path):
		try:
			shutil.rmtree(destination_path)
		except:
			print "Exception when trying to remove " + destination_path
			print "Bailing."
			print ""
			return

	print "What shall the name of the project be?"
	project_name = raw_input("New project name ? ")

	shutil.copytree(source_path, destination_path)
	deepreplace(destination_path, "Blank", project_name)

go()


