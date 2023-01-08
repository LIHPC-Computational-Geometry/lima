#! /usr/bin/python

#########################################
# 
# Filtrage des fichiers .h de Lima
#
# #ifdef __INTER_XX
# toto
# #else
# titi
# #endif
#
# est remplace par:
# toto
#
# idem avec # ifndef SWIG. Ne gere pas commentaire pour le moment
#
#########################################

import os, sys, stat


# element of the main stack
class StackElement:
	def __init__(self, a_conserveThen):
		self.inElse = False
		self.conserveThen = a_conserveThen
		self.nbIgnoredIfdef = 0
		
		
# return last element of a_vector (must not be empty)
def last(a_vector):
	return a_vector[len(a_vector) - 1]


# return true if the given line must be kept in the dest file
def mustWriteLine(line, stack, tagList):
	if line.find("#ifdef ") >= 0 or line.find("#ifndef ") >= 0:
		cst = line[:-1].split(" ")[1]
		for tag in tagList:
			if cst == tag["tag"]:
				if line.find("#ifdef ") >= 0:
					stack.append(StackElement(tag["positiv"]))
				else:
					stack.append(StackElement(not tag["positiv"]))
				return False
		
	if line.find("#else") >= 0 and len(stack) > 0 and last(stack).nbIgnoredIfdef == 0:
		last(stack).inElse = True
		return False
	
	if line.find("#endif") >= 0 and len(stack) > 0:
		if last(stack).nbIgnoredIfdef > 0:
			last(stack).nbIgnoredIfdef -= 1
		else:
			del stack[len(stack)-1]
			return False
	if line.find("#if") >= 0 and len(stack) > 0:
		last(stack).nbIgnoredIfdef += 1
		
	return len(stack) == 0 or last(stack).conserveThen != last(stack).inElse


def convertFile(path, tagList):
	# open the files
	filein = open(path, 'r')
	fileout = open(path + "_out", 'w')
	line = "."
	stack = []		# the main stack that contains the info about the current ifdef structures
	
	# for each line of the source file
	while line != "":
		line = filein.readline()
		
		# test if the line must be written to dest file
		if mustWriteLine(line, stack, tagList):
			fileout.write(line)
		
	# close files
	filein.close()
	fileout.close()
	
	# move dest to src
	os.rename(path + "_out", path)
	os.chmod(path, stat.S_IRWXU | stat.S_IRWXG | stat.S_IROTH | stat.S_IXOTH)


# parse the tag_list argument and return the list 
def parseTagList(line):
	s = line.split(" ")
	ret = []
	for t in s:
		ret.append({"tag":t[1:], "positiv":t[0]=='+'})
	return ret


##########
#  main  #
########## 
# check args
if len(sys.argv) != 3:
	print("Usage: " + os.path.basename(sys.argv[0]) + " path tag_list")
	print("   with tag_list = \"+/-tag1 +/-tag2 ...\"")
	sys.exit(1)
path = sys.argv[1]
tagList = parseTagList(sys.argv[2])
print("Filtrage fichiers en-tete du repertoire " + path)
#print tagList

for entry in os.listdir(path):
	if not entry.startswith(".") and os.path.isfile(path + "/" + entry): 
		convertFile(path + "/" + entry, tagList)

