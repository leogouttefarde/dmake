# Script de déploiement par réinstallation des noeuds
# et installation des librairies requises

# Step 1
ssh lgouttefarde@access.grid5000.fr
ssh grenoble


# Dure quelques secondes
oarsub -I -l nodes=62,walltime=4:00 -t deploy


# Assez long (2-3 minutes)
kadeploy3 -f $OAR_NODE_FILE -e jessie-x64-std -k



# Step 2

# runs a script remotely
remote_run()
{
  if [[ $# -ge 2 && -f "$2" ]]; then
    ssh -oStrictHostKeyChecking=no root@$1 'bash -s' < "$2"
  fi
}


# Charm++ nodelist file (regenerate it after each oarsub node allocation)
NODELIST=~/nodelist

sort -u $OAR_NODEFILE > nodes

# Generates Charm++ nodelist file
# each line from $OAR_NODEFILE = 1 cpu entry, usually 8 / node
awk '$0="\thost "$0" ++cpus 8"' nodes > tmp
echo "group main" > $NODELIST
cat tmp >> $NODELIST
rm tmp


echo -e "apt-get update\napt-get -y install blender ffmpeg ImageMagick ncftp" > task.sh
echo -e "nohup sh ~/task.sh &> out.txt &" > runTask.sh

SERVS=$(cat nodes)
SSH=$(tail -n 1 nodes)


# for each node
for SERV in $SERVS; do

  scp ~/.ssh/id_rsa root@$SERV:~/.ssh/id_rsa
  scp ~/.ssh/id_rsa.pub root@$SERV:~/.ssh/id_rsa.pub

  scp $NODELIST root@$SERV:~
  scp nodes root@$SERV:~
  scp task.sh root@$SERV:~

  remote_run $SERV runTask.sh

done



ssh root@$SSH


cd ~
wget http://charm.cs.illinois.edu/distrib/charm-6.7.1.tar.gz
tar -xvzf charm-6.7.1.tar.gz
cd ~/charm-6.7.1
./build charm++ netlrts-linux-x86_64 --with-production -j8

# Input password here
git clone ssh://lgout@depots.ensimag.fr/depots/2016/BDFG_SDCA/TP_SDCA.git ~/make


# Step 3
# change ftp password in test/bench.sh
vim ~/make/test/bench.sh



# Step 4
cd ~/make/src

# Fix Charm++ path
tail -n +4 Makefile >> Makefile_
echo "CPATH=~/charm-6.7.1" > Makefile
cat Makefile_ >> Makefile

make -j8


cd ~/make/sujet/makefiles/premier
gcc premier.c -lm -o premier

cd ~




# for each node
for SERV in $(cat ~/nodes); do

  scp -o StrictHostKeyChecking=no -rp ~/make root@$SERV:~

done


## Ready for testing
#cd ~/make/sujet/makefiles/blender_2.49

#../../../src/charmrun ++nodelist ~/nodelist ++ppn 2 ++p 60  ../../../src/Make Makefile

cd ~/make


nohup ./test/bench.sh &> bench.log &

