#! /bin/bash

RFOLDER=graphs
DATA=

init_file()
{
  if [ $# -ge 1 ]; then
    > "$1"
  fi;
}

print_separator()
{
  log "+==============================================================================+"
}

non_zero()
{
  if [ $# -ge 1 ]; then
    local VAL=$1

    if [ $VAL -eq 0 ]; then
      VAL=$((1))
    fi

    echo $VAL
  fi;
}

array_reset()
{
  for ((i=0; i < DATA_SIZE; i++)); do
    DATA[$i]=0
  done
}

array_set_size()
{
  DATA_SIZE=$1
  array_reset
  echo "array_set_size : $DATA_SIZE"
}

# usage : array_set <idx> <value>
array_set()
{
  DATA[$1]=$2
}

# usage : array_get <idx>
array_get()
{
  local val=${DATA[$1]}

  if [[ ! ${val} ]]; then
    val=0
  fi

  echo $val
}

data_init()
{
  if [ $# -ge 3 ]; then
    local BASENAME=$(basename $1)

    cat > "${1}".R <<EOF
pdf("$BASENAME.pdf")

nCPU = $3
STEP = $2

#x_axis <- seq.int( nCPU, STEP, -STEP )
#x_axis <- seq.int( STEP, nCPU, STEP )
x_axis=c()
for (i in seq(from=nCPU, to=1, by=-STEP)) {
  x_axis = c( x_axis, i )
}

if ( ! (1 %in% x_axis) ) {
  x_axis = c( x_axis, 1 )
}

x_axis = rev(x_axis)

EOF
    local SIZE=$(nb_iterations $3 $2)

    if [ $2 -ne 1 ]; then
      SIZE=$((SIZE+1))
    fi

    array_set_size $SIZE
  fi;
}

data_add()
{
  if [ $# -ge 2 ]; then
    printf " $2," >> "${1}".R
  fi;
}

data_array()
{
  if [ $# -ge 1 ]; then
    local i
    local val
    local num=$2
    local FILE="${1}".R

    name=data #"$METHOD"
    printf "$name <- c(" >> "${FILE}"
    echo "DATA_SIZE = $DATA_SIZE"

    for ((i=0; i < DATA_SIZE; i++)); do
      val=$(array_get $i)
      printf " $val" >> "${FILE}"

      if ((i < DATA_SIZE-1)); then
        printf ", " >> "${FILE}"
      else
        printf " )\n\n" >> "${FILE}"
      fi
    done

  fi;
}

data_end()
{
  if [ $# -ge 1 ]; then
    local FILE="${1}".R
    # local TITLE="${2}"

    cat >> "${FILE}" <<EOF
# -1 )
#data <- head( data, -1)

data = data / 1000

#sCPU = tail(data, n=1)

xLim = 4 * nCPU / 7
sCPU = head(data, n=1)
yLim = sCPU + sCPU / 100

x_ideal <- c( 1, nCPU )
y_ideal <- c( sCPU, sCPU / nCPU )
y_lims <- c( min(data), yLim )

plot( x_axis, data, pch="+", type="o", col="dark blue", ann=FALSE, ylim=y_lims )
lines( x_ideal, y_ideal, pch="+", type="o", col="cyan", ann=FALSE )

title( main="Durée de compilation", font.main=1, cex.main=2 )
title( xlab="Nombre de machines", cex.lab=1.2 )
title( ylab="Durée en secondes", cex.lab=1.2 )


# places a legend at the appropriate place
legend( xLim, yLim,

 # puts text in the legend
c("Parallélisation idéale", "Temps mesurés"),

# gives the legend appropriate symbols (lines)
lty=c(1,1),

 # gives the legend lines the correct color and width
lwd=c(2.5,2.5),col=c("cyan", "blue"),bg="white")


dev.off()
EOF

      #cp "${FILE}" "${RFOLDER}"
  fi;
}

# Computes the compression rate in millipercents
# compression_rate <plain_size> <zip_size>
compression_rate()
{
  if [ $# -ge 2 ]; then
    local MAX=100000
    local MIN=-100000
    local DIV=$(non_zero $1)
    local CRATE

    CRATE=$(((1000 * 100 * ($DIV - $2)) / $DIV))

    if (( CRATE > MAX )); then
      CRATE=$MAX

    elif (( CRATE < MIN )); then
      CRATE=$MIN
    fi

    echo $CRATE
  fi;
}

# Computes the speed in MB/s given a size in bytes and a time in seconds
# speed <size> <seconds>
speed()
{
  if [ $# -ge 2 ]; then
    local DIV=$(non_zero $2)
    local SPEED

    SPEED=$(((1000 * $1 / $DIV) / 1024 / 1024))

    echo $SPEED
  fi;
}

percents()
{
  if [ $# -ge 1 ]; then
    echo $(($1/1000))
  fi;
}

mpercents()
{
  if [ $# -ge 1 ]; then
    local PERCENTS=$(percents $1)
    local MPERCENTS=$(($1 % 1000))

    if (( PERCENTS != 0 && PERCENTS != 0 && MPERCENTS < 0 )); then
      MPERCENTS=$((-MPERCENTS))
    fi

    echo $MPERCENTS
  fi;
}

is_cmd()
{
  type "$1" &> /dev/null
}

R_run()
{
  if [ $# -ge 1 ]; then
    R --quiet --vanilla < "${1}" >/dev/null 2>&1
  fi;
}

R_exec()
{
  if [ $# -ge 1 ]; then
    if is_cmd R ; then
      local FILE="$1".R

      cd $RFOLDER
      R_run "${FILE}"
      cd - &> /dev/null
    fi;
  fi;
}

reset_dir()
{
  if [ $# -ge 1 ]; then
    local dpath="./${1}"
    rm -f "./${dpath}"/* &> /dev/null
    rmdir --ignore-fail-on-non-empty "${dpath}" >/dev/null 2>&1
  fi;
}

R_exec_all()
{
  if is_cmd R ; then
    for dir in ./$1 ; do

      cd $dir
      # echo "cd $dir"
      echo "Generating graphs from $dir"

      for f in *.R ; do
          # echo $f
          R_run "${f}"
      done

      cd - &> /dev/null

    done
  fi;
}

dir_count()
{
  if [ $# -ge 1 ]; then
    # echo $(ls "$1" -1 | wc -l)
    echo $(find "$1" -type f | wc -l)
  fi;
}

get_max_cpu()
{
  local nCPU=$(grep "^core id" /proc/cpuinfo | sort -u | wc -l)
  echo $nCPU
}

nb_iterations()
{
  if [ $# -ge 2 ]; then
    local CPUs=$1
    local CSTEP=$2
    local ret=$((CPUs / CSTEP))

    if ! ((CPUs % CSTEP)) ; then
      ret=$ret
    else
      ret=$((ret+1))
    fi;

    echo $ret
  fi;
}


