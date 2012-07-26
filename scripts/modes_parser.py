import sys
import os

def parseFile(fin,fouts):
	lines = fin.readlines()
	modes_acc = {'INTRA': 0, 'INTER': 0,'SKIP': 0}
	sizes_acc = {'0': 0, '1': 0, '2': 0, '3': 0}
	partitions_acc = {'0': 0,'1': 0, '2': 0, '3': 0,'4': 0, '5': 0, '6': 0, '7': 0}

	CUParts_acc = []
	for i in range(4):
		line = []
		for j in range(8):
			line.append(0)
		CUParts_acc.append(line)
	total = 0
	for line in lines:
		line = line[:-1]
		line = line.split(' ')

		modes_acc[line[0]] += 1

		if line[0] == 'INTER':
			CUParts_acc[int(line[1])][int(line[2])] += 1
			sizes_acc[line[1]] += 1
			partitions_acc[line[2]] += 1
		total += 1
	i = 0
	print >> fouts[0], ";"
	for line in CUParts_acc:
		print >> fouts[0], i,
		for c in line:
			print >> fouts[0],c,
		print >> fouts[0],"\n",
		i += 1
	print >> fouts[1], modes_acc['INTRA'], ";",modes_acc['INTER'],";",modes_acc['SKIP']
	print >> fouts[2], sizes_acc['0'], ";",sizes_acc['1'],";",sizes_acc['2'], ";",sizes_acc['3']
	print >> fouts[3], partitions_acc['0'], ";",partitions_acc['1'],";",partitions_acc['2'], partitions_acc['3'],";", partitions_acc['4'],";",partitions_acc['5'],";",partitions_acc['6'],";",partitions_acc['7']



qps = ['22', '27', '32', '37']
decisions = ['RDO']
#decisions = ['AMP-high','AMP-low','SMP-only','RDO']
videos = ['BQSquare','BQTerrace','ChinaSpeed','RaceHorsesC','Vidyo1','Traffic']
modes = {
'Traffic':['encoder_randomaccess','encoder_randomaccess_loco','encoder_randomaccess_10'],
'BQTerrace': ['encoder_randomaccess','encoder_randomaccess_loco','encoder_randomaccess_10','encoder_lowdelay','encoder_lowdelay_loco'],
'RaceHorsesC': ['encoder_randomaccess','encoder_randomaccess_loco','encoder_lowdelay','encoder_lowdelay_loco'],
'BQSquare': ['encoder_randomaccess','encoder_randomaccess_loco','encoder_lowdelay','encoder_lowdelay_loco'],
'Vidyo1': ['encoder_lowdelay','encoder_lowdelay_loco'],
'ChinaSpeed': ['encoder_randomaccess','encoder_randomaccess_loco','encoder_lowdelay','encoder_lowdelay_loco']
}

os.system("rm *-modes*")
os.system("rm *-parts*")
os.system("rm *-sizes*")
os.system("rm *-CUParts*")

for v in videos:
	for m in modes[v]:
		for d in decisions:
			if m in ['encoder_randomaccess_loco','encoder_lowdelay_loco'] and d in ['AMP-high','AMP-low']:
				continue
			fouts = [open(m + "_" + d +'-CUParts.csv','a'), open(m + "_" + d +'-modes.csv','a'), open(m + "_" + d +'-sizes.csv','a'), open(m + "_" + d +'-parts.csv','a')]
			for f in fouts:
				print >> f, v  + "_" +  m  + "_" +  d
			print >> fouts[0], ';2Nx2N;2NxN;Nx2N;NxN;2NxnU;2NxnD;nLx2n;nRx2N;'
			print >> fouts[1], ';INTRA;INTER;SKIP;'
			print >> fouts[2], ';64;32;16;8;'
			print >> fouts[3], ';2Nx2N;2NxN;Nx2N;NxN;2NxnU;2NxnD;nLx2n;nRx2N;'
			for qp in qps:
				for f in fouts:
					print >> f,qp

				fin = open(v  + "_" +  m  + "_" +  d  + "_qp" +  qp + '.csv','r')
				parseFile(fin,fouts)
				fin.close()
			for f in fouts:
				f.close()
