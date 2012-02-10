from sys import *

fin = open(argv[1],"r")
fout = open(argv[2], "w")

lines = fin.readlines()

for line in lines:
	if line [:3] == "POC":
		beg = line.find("[Y ")
		beg = beg + 3
		end = line[beg:].find(" dB")
		num = line[beg:(beg+end)]
		fout.write(num + "\n")

fin.close()
fout.close()
