
cd ~
wget http://charm.cs.illinois.edu/distrib/charm-6.7.1.tar.gz
tar -xvzf charm-6.7.1.tar.gz
cd ~/charm-6.7.1
./build charm++ netlrts-linux-x86_64 --with-production -j8

