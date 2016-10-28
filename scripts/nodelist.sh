
# Charm++ nodelist file (regenerate it after each oarsub node allocation)
NODELIST=~/nodelist
export NODELIST

# Generates Charm++ nodelist file
# each line from $OAR_NODEFILE = 1 cpu entry, usually 8 / node
awk '$0="\thost "$0" ++cpus 8"' $OAR_NODEFILE > $NODELIST
sort -u $NODELIST > tmp
echo "group main" > $NODELIST
cat tmp >> $NODELIST
rm tmp

