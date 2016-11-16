

oarsub -I -l nodes=7 -t deploy



kadeploy3 -f $OAR_NODE_FILE -e jessie-x64-std -k



# runs a script remotely
remote_run()
{
  if [[ $# -ge 2 && -f "$2" ]]; then
    ssh -oStrictHostKeyChecking=no root@$1 'bash -s' < "$2"
  fi
}


# Charm++ nodelist file (regenerate it after each oarsub node allocation)
NODELIST=~/nodelist

# Generates Charm++ nodelist file
# each line from $OAR_NODEFILE = 1 cpu entry, usually 8 / node
awk '$0="\thost "$0" ++cpus 8"' $OAR_NODEFILE > $NODELIST
sort -u $NODELIST > tmp
echo "group main" > $NODELIST
cat tmp >> $NODELIST
rm tmp


echo -e "apt-get update\napt-get -y install blender ffmpeg ImageMagick" > task.sh
echo -e "nohup sh ~/task.sh &> out.txt &" > runTask.sh

sort -u $OAR_NODEFILE > nodes

SERVS=$(sort -u $OAR_NODEFILE)
MASTER=$(head -n 1 $OAR_NODEFILE)


# for each node
for SERV in $SERVS; do

  scp ~/.ssh/id_rsa root@$SERV:~/.ssh/id_rsa
  scp ~/.ssh/id_rsa.pub root@$SERV:~/.ssh/id_rsa.pub

  scp $NODELIST root@$SERV:~
  scp nodes root@$SERV:~
  scp task.sh root@$SERV:~

  remote_run $SERV runTask.sh

done



ssh root@$MASTER


cd ~
wget http://charm.cs.illinois.edu/distrib/charm-6.7.1.tar.gz
tar -xvzf charm-6.7.1.tar.gz
cd ~/charm-6.7.1
./build charm++ netlrts-linux-x86_64 --with-production -j8


git clone ssh://lgout@depots.ensimag.fr/depots/2016/BDFG_SDCA/TP_SDCA.git ~/make

cd ~/make/src
make -j8

cp Make ~
cd ~

# for each node
for SERV in $(cat nodes); do

  scp -o StrictHostKeyChecking=no -rp ~/make root@$SERV:~

done


cd ~/make/sujet/makefiles/blender_2.49
../../../src/charmrun ++nodelist ~/nodelist ++ppn 3 ++p 18  ../../../src/Make Makefile

