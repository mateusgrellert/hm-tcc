# 1 - input, 2 - "FAST", 3 - fast PU, 4 - fast CU, 5 - fast TU
if [ $2 = "FAST" ]; then
	${HOME}/hm-tcc/bin/TAppEncoderStaticd -c cfg/tcc.cfg --InputFile=${HOME}/videos/$1 --DbgFile=$1-FAST.dbg --FastPU=$3 --FastCU=$4 --FastTU=$5 > $1-FAST.out
elif [ $2 = "RDO" ]; then
	${HOME}/hm-tcc/bin/TAppEncoderStaticd -c cfg/tcc.cfg --InputFile=${HOME}/videos/$1 --DbgFile=$1-RDO.dbg > $1-RDO.out
fi
