
cd ~/charm-6.7.1/examples/charm++/PUP/HeapPUP
make

# Run Charm++ program on allocated nodes
./charmrun ++nodelist $NODELIST SimplePUP ++ppn 3 ++p 9

# ++p
# Nombre de processus
#
# ++ppn
# Nombre de processus par noeud
