#! /bin/bash

DIR=$(cd "$(dirname "$0")" && pwd)
. "$DIR"/utility.sh

SECONDS=0
BIN=src/Make
BENCHDIR=benchmark
MACHINE=$(hostname)
HISTDIR=history/$MACHINE
GETTIME="date +%d/%m/%Y-%H:%M:%S"
TIMESTAMP=$(date +%Y-%m-%d_%H:%M:%S)
START=$($GETTIME)
# CSVFILE=$TIMESTAMP.csv
LOGFILE=$TIMESTAMP.log
TMPLOG=pending.log
mTHREADS=1
# mNODES=40
mNODES=$(cat ~/nodes | wc -l)
MCPU=$(get_max_cpu)
CLEVEL=1
THSTEP=1
nSAMPLES=7
NSTEP=$(nb_iterations $mNODES $(nb_iterations $mNODES $nSAMPLES))

log()
{
  if [ $# -ge 1 ]; then
    echo -e "$1" #| tee /dev/tty >> ${TMPLOG} 2>/dev/null
  fi;
}


reset_dir "${RFOLDER}"
mkdir -p $HISTDIR "${RFOLDER}"

# make clean
cd src
make -j$MCPU &> /dev/null
cd - &> /dev/null


for testDir in sujet/makefiles/*/ ; do
# for d in ${BENCHDIR}/*/*/ ; do

  cd $testDir
  echo $testDir

for mfile in Makefile* ; do

  echo "Makefile : $mfile"

  BASE=${testDir}${mfile}
  NAME="${BASE//[\/]/_}"

  nIts=$(nb_iterations $CPUs $CSTEP)


  # Check all possible CPU numbers
  # for ((nTHREADS=$mTHREADS; nTHREADS>0; nTHREADS-=THSTEP)); do
  for ((nTHREADS=1; nTHREADS <= mTHREADS; nTHREADS+=THSTEP)); do

    nIts=$(nb_iterations $mNODES $NSTEP)
    echo "mNODES = $mNODES"
    echo "NSTEP = $NSTEP"

    RFILE="../../../$RFOLDER"/"$NAME"_nth$nTHREADS

    data_init "${RFILE}" $nIts

    # Perform both hardware and software compression / decompression
    # for ((nNODES=1; nNODES < mNODES; nNODES++)); do
    for ((nNODES=$mNODES; nNODES>0; nNODES-=NSTEP)); do

      nProcs=$((nNODES * nTHREADS))

      echo "nTHREADS : $nTHREADS, nNODES : $nNODES, nProcs : $nProcs"

      # pwd
      # OUT="46\n777" # test
      OUT=$(../../../src/charmrun ../../../src/Make $mfile ++nodelist ~/nodelist ++ppn ${nTHREADS} ++p ${nProcs} 2>/dev/null)

      TIME=$(echo "${OUT}" | tail -2 | head -1)
      # TIME=$(echo -e "${OUT}" | tail -1)
      echo -e "${OUT}" > out_$mfile.txt

      echo "Time : $TIME ms"
      # echo "NN : $((((nProcs / THSTEP)/ NSTEP) - 1))"

      array_set $((((nProcs / THSTEP)/ NSTEP) - 1)) $TIME

    done


    # Flush all computed values
    data_array "${RFILE}" $ENC

    # Finish R scripts
    data_end "${RFILE}"

  done

done

  cd - &> /dev/null
done



END=$($GETTIME)

ELAPSED=$(date -d @$SECONDS -u +'%H:%M:%S')
log "\nStart time : $START"
log "Final time : $END"
log "Duration   : $ELAPSED"

FOLDER=$HISTDIR/$TIMESTAMP

GRAPHS=$FOLDER/graphs

mkdir -p $FOLDER $GRAPHS

# mv ${TMPLOG} $FOLDER/${LOGFILE} &> /dev/null
mv ${RFOLDER}/*.{R,pdf} $GRAPHS &> /dev/null

zip history.zip -r history &> /dev/null

# FTP settings
FTPU="gouttefarde"
FTPP="password"
FTPS="ftpperso.free.fr"
NCFTP="$(which ncftpput)"

FILE=history.zip

# Send backup via FTP
ncftp -u"$FTPU" -p"$FTPP" $FTPS<<EOF
put $FILE
quit
EOF


