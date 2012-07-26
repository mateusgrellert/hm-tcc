import os

"""videos = [
'BQSquare.cfg',
'Traffic.cfg',
'BQTerrace.cfg',
'RaceHorsesC.cfg',
'Vidyo1.cfg',
'ChinaSpeed.cfg'
]"""

videos = [
'BQSquare.cfg',
]

configs = {
'Traffic.cfg':['encoder_randomaccess.cfg','encoder_randomaccess_loco.cfg','encoder_randomaccess_10.cfg'],
'BQTerrace.cfg': ['encoder_randomaccess.cfg','encoder_randomaccess_loco.cfg','encoder_randomaccess_10.cfg','encoder_lowdelay.cfg','encoder_lowdelay_loco.cfg'],
'RaceHorsesC.cfg': ['encoder_randomaccess.cfg','encoder_randomaccess_loco.cfg','encoder_lowdelay.cfg','encoder_lowdelay_loco.cfg'],
'BQSquare.cfg': ['encoder_randomaccess.cfg','encoder_randomaccess_loco.cfg','encoder_lowdelay.cfg','encoder_lowdelay_loco.cfg'],
'Vidyo1.cfg': ['encoder_lowdelay.cfg','encoder_lowdelay_loco.cfg'],
'ChinaSpeed.cfg': ['encoder_randomaccess.cfg','encoder_randomaccess_loco.cfg','encoder_lowdelay.cfg','encoder_lowdelay_loco.cfg']
}

apps = [
'qp22.cfg',
'qp27.cfg',
'qp32.cfg',
'qp37.cfg'
]
modes = [
'AMP-high',
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
	for config in configs[video]:
		fp.write(config + '\n')
		for mode in modes:
			if ((config in ['encoder_randomaccess_loco.cfg','encoder_lowdelay_loco.cfg']) and (mode in ['AMP-low','AMP-high'])): #skipping AMP modes for _loco configs
				continue
			for app in apps:
				print '\t', config + ' ' + mode + ' '+  app
				os.system('./TAppEncoderStatic -c ../cfg/per-sequence/' + video + ' -c ../cfg/' + config + ' -c ../cfg/tcc/' + mode + '/' + app + ' --FrameToBeEncoded=60 > outHM')
				fpHM = open('outHM', 'r')
				fp.write(parseHMResults(fpHM))
				fpHM.close()
				os.system('mv *.dbg ../modes_depths/' + video[:-4] + '_' + config[:-4] + '_' + mode + '_' + app[:-4] + '.csv')
	fp.close()

