from os import system

def getNum(line):
	idx = line.find(":")
	words = line[idx+1:].split(" ")
	for word in words:
		if word.find(".") >= 0 or word.isdigit():
			return word

def parseRDFile(v,config,mode,qp):
	inpFp = open("output.ref","r")
	lines = inpFp.readlines()
	inpFp.close()
	outFp = open("parsedRD.csv","a")
	print >> outFp, v + ";" + config + ";" + mode + ";" + qp

	results = [0,0,0,0,0]
	if mode == "JM":
		for line in lines:
			if line.find("Total encoding time") >= 0:
				results[4] = getNum(line)
			elif line.find("Y { PSNR") >= 0:
				results[1] = getNum(line).rstrip(",")
			elif line.find("U { PSNR") >= 0:
				results[2] = getNum(line).rstrip(",")
			elif line.find("V { PSNR") >= 0:
				results[3] = getNum(line).rstrip(",")
			elif line.find("Bit rate (kbit/s)") >= 0:
				results[0] = getNum(line)
	else:
		i = 0
		for line in lines:
			if line.find("SUMMARY") >= 0:
				i = 1
			elif i == 1:
				i = 2
			elif i == 2:
				data = line.split(" ")
				j = 1
				for d in data:
					if d.find(".") >= 0:
						results[j] = d
						j += 1					
				i = 3
			if line.find("Bytes written to file") >= 0:
				idx = line.find("(")
				results[0] = getNum(line[idx+1:]) #bitrate
			if line.find("Total Time") >= 0:
				results[4] = getNum(line)

	for r in results:
		print >> outFp, r,
	print >> outFp, "\n"

"""
	if mode == "JM":
		for line in lines:
			if line.find("Total encoding time") >= 0:
				print >> outFp, "Enc. Time;", getNum(line)
			elif line.find("Total ME time") >= 0:
				print >> outFp, "ME Time;", getNum(line)
			elif line.find("Y { PSNR") >= 0:
				print >> outFp, "Y PSNR;", getNum(line).rstrip(",")
			elif line.find("U { PSNR") >= 0:
				print >> outFp, "U PSNR;", getNum(line).rstrip(",")
			elif line.find("V { PSNR") >= 0:
				print >> outFp, "V PSNR;", getNum(line).rstrip(",")
			elif line.find("Total bits") >= 0:
				print >> outFp, "Total bits;", getNum(line)
			elif line.find("Bit rate (kbit/s)") >= 0:
				print >> outFp, "bitrate (kbit/s);", getNum(line)
	else:
		i = 0
		results = []
		for line in lines:
			if line.find("SUMMARY") >= 0:
				i = 1
			elif i == 1:
				i = 2
			elif i == 2:
				data = line.split(" ")
				for d in data:
					if d.find(".") >= 0:
						results.append(d)
				i = 3
			if line.find("Bytes written to file") >= 0:
				idx = line.find("(")
				results.append(int(getNum(line[:idx]))*8) #total bytes -> bits
				results.append(getNum(line[idx+1:])) #bitrate
			if line.find("Total Time") >= 0:
				print >> outFp, "Enc. Time;", getNum(line)

		print >> outFp, "ME Time; XXX"
		print >> outFp, "Y PSNR;", results[1]
		print >> outFp, "U PSNR;", results[2]
		print >> outFp, "V PSNR;", results[3].rstrip() #tira um newline
		print >> outFp, "Total bits;", results[4]
		print >> outFp, "bitrate (kbit/s);", results[5], "\n"
				
				
"""
def parseGPFile(v,config,mode):
	inpFp = open("output.gprof","r")
	lines = inpFp.readlines()
	inpFp.close()
	outFp = open("complexity" + mode + ".csv","a")
	print >> outFp, v + ";" + config + ";" + mode
	if mode == "JM":		
		string = "BlockMotionSearch" 
	else:
		string = "TEncSearch::predInterSearch"
	for line in lines:	
		if line.find("[") == 0:
			if line.find(string) >= 0:
				idx = line.find(".")
				print line
				print >> outFp, line[idx-2:idx+2]
				return

#videos = ['BQSquare','RaceHorsesC','ChinaSpeed','Vidyo1','BQTerrace','Traffic']
#qps = ['22','27','32','37']
videos = ['BQSquare','RaceHorsesC']
qps = ['22','27']

mode = "HM"
profile = False
nFrames = '16'

if mode == "JM":
	binary = "./lencod.exe"
	confFiles = ["HM-like/encoder_JM_RA_B_HE.cfg","HM-like/encoder_JM_LP_HE.cfg"]
	inpPath =  "./HM-like/per-sequence_JM/"
else:
	binary = "./TAppEncoderStatic"
	confFiles = ["../cfg/encoder_randomaccess.cfg","../cfg/encoder_lowdelay_P.cfg"]
	inpPath = "../cfg/per-sequence/"


system("rm *.csv")
for v in videos:
	inpFile = inpPath + v + ".cfg"
	for config in confFiles:
		print "\tRunning " + v + " in " + config + " mode"
		for qp in qps:
			if mode == "JM":
				execLine = binary + " -d " + config + " -f " + inpFile + " -p FramesToBeEncoded=" + nFrames + " -p QP=" + qp
			else:
				execLine = binary + " -c " + config + " -c " + inpFile + " --FrameToBeEncoded=" + nFrames + " --QP="+ qp

			print "\t\tQP :",qp
		
			system(execLine + " > output.ref") 								#1 - roda e gera gmon.out. saida redirecionada
			if profile:
				system("mv gmon.out " + v + ".sum")   							#2 - gmon.out > [video].sum
				system(execLine)                     							#3 - roda de novo pra gerar novo gmon.out
				system("gprof -s " + binary + " gmon.out " + v + ".sum")  		#4 - agrega os dois resultados
				system("gprof " + binary + " " + v + ".sum > output.gprof")		#5 - gprof pra gerar a saida
				parseGPFile(v,config,mode)
			parseRDFile(v,config,mode,qp) 										#6 - parse das saidas do software e do gprof

system("mv parsedRD.csv ~/Dropbox/parsedRD" + mode + ".csv")
system("rm *.sum")
system("rm *.out")
system("rm output.*")
