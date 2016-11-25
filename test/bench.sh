#! /bin/bash

DIR=$(cd "$(dirname "$0")" && pwd)
. "$DIR"/utility.sh

SECONDS=0
BIN=src/Make
BENCHDIR=benchmark
MACHINE=$(hostname)
HISTDIR=history
GETTIME="date +%d/%m/%Y-%H:%M:%S"
TIMESTAMP=$(date +%Y-%m-%d_%H:%M:%S)
START=$($GETTIME)
# CSVFILE=$TIMESTAMP.csv
LOGFILE=$TIMESTAMP.log
TMPLOG=pending.log
mTHREADS=8
# mNODES=42
mNODES=$(cat ~/nodes | wc -l)
MCPU=$(get_max_cpu)
CLEVEL=1
THSTEP=4
nSAMPLES=14
NSTEP=$(nb_iterations $mNODES $nSAMPLES)

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


  # Check all possible CPU numbers
  # for ((nTHREADS=$mTHREADS; nTHREADS>0; nTHREADS-=THSTEP)); do
  for ((nTHREADS=8; nTHREADS <= mTHREADS; nTHREADS+=THSTEP)); do

    echo "mNODES = $mNODES"
    echo "NSTEP = $NSTEP"

    RFILE="../../../$RFOLDER"/"$NAME"_nth$nTHREADS

    data_init "${RFILE}" $NSTEP $mNODES
    idx=$((DATA_SIZE-1))

    # Perform both hardware and software compression / decompression
    # for ((nNODES=1; nNODES < mNODES; nNODES++)); do
    for ((nNODES=mNODES; nNODES>0; nNODES-=NSTEP)); do

      nProcs=$((nNODES * nTHREADS))

      echo "nTHREADS : $nTHREADS, nNODES : $nNODES, nProcs : $nProcs"

      # pwd
      # OUT="$nNODES\n777" # test
      OUT=$(../../../src/charmrun ../../../src/Make $mfile ++nodelist ~/nodelist ++ppn ${nTHREADS} ++p ${nProcs} 2>/dev/null)

      # TIME=$(echo -e "${OUT}" | tail -1)
      TIME=$(echo -e "${OUT}" | tail -2 | head -1)
      echo -e "${OUT}" > out_$mfile-$nNODES.txt

      echo "Time : $TIME ms"
      # echo "NN : $((((nProcs / THSTEP)/ NSTEP) - 1))"

      array_set $idx $TIME
      idx=$((idx-1))

      # Force un dernier calcul avec 1 seul noeud
      if ((nNODES > 1 && nNODES <= NSTEP)); then
      	nNODES=$((NSTEP + 1))
      fi

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

mkdir -p $FOLDER

# mv ${TMPLOG} $FOLDER/${LOGFILE} &> /dev/null
mv ${RFOLDER}/*.{R,pdf} $FOLDER &> /dev/null

zip out.zip -r history bench.log sujet/makefiles/*/out_Makefile*.txt ~/nodes &> /dev/null

# FTP settings
FTPU="gouttefarde"
FTPP="password"
FTPS="ftpperso.free.fr"
NCFTP="$(which ncftpput)"

FILE=out.zip

# Send backup via FTP
ncftp -u"$FTPU" -p"$FTPP" $FTPS<<EOF
put $FILE
quit
EOF


