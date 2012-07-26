import os

videos = [
'BQSquare.cfg',
'Traffic.cfg',
'BQTerrace.cfg',
'RaceHorsesC.cfg',
'Vidyo1.cfg',
'ChinaSpeed.cfg'
]

videos = [
'BQSquare.cfg',
]

configs = {
'encoder_lowdelay_P.cfg',
'encoder_randomaccess.cfg'
}

apps = [
'qp22.cfg',
'qp27.cfg',
'qp32.cfg',
'qp37.cfg'
]
modes = [
'RDO',
]
"""modes = [
'SMP-only',
'AMP-low',
'AMP-high',
'RDO'
]"""

def parseHMResults(fp):
	buff = fp.readlines()
	returnable = ''
	for line in buff:
		temp = line.split()
		if temp != []:
			if temp[1] == 'a':
				returnable += temp[2] + ';'
				returnable += temp[3] + ';'
				returnable += temp[4] + ';'
				returnable += temp[5] + ';'
			if temp[0] == 'Total' and temp[1] == 'Time:':
				returnable += temp[2] + '\n'
	return returnable
	
for video in videos:
	fp = open(video[:-4] + '.csv', 'a')
	print 'Video: '+ video[:-4]
	for config in configs:
		fp.write(config + '\n')
		for mode in modes:
			for app in apps:
				print '\t', config + ' ' + mode + ' '+  app
				os.system('./TAppEncoderStatic -c ../cfg/per-sequence/' + video + ' -c ../cfg/' + config + ' -c ../cfg/tcc/' + mode + '/' + app + ' --FrameToBeEncoded=100 > outHM')
				fpHM = open('outHM', 'r')
				fp.write(parseHMResults(fpHM))
				fpHM.close()
	fp.close()

