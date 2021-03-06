#!/bin/bash

# rpms prerequisits needs root
#yum install make gcc-c++ gcc binutils libX11-devel libXpm-devel libXft-devel libXext-devel git 


mkdir ToolDAQ
cd ToolDAQ

git clone https://github.com/ToolDAQ/ToolDAQFramework.git
git clone https://github.com/ToolDAQ/zeromq-4.0.7.git
wget http://downloads.sourceforge.net/project/boost/boost/1.60.0/boost_1_60_0.tar.gz
tar zxvf boost_1_60_0.tar.gz

cd zeromq-4.0.7

./configure --prefix=`pwd`
make
make install

export LD_LIBRARY_PATH=`pwd`/lib:$LD_LIBRARY_PATH

cd ../boost_1_60_0

mkdir install

./bootstrap.sh --prefix=`pwd`/install/ 
./b2 install 

export LD_LIBRARY_PATH=`pwd`/install/lib:$LD_LIBRARY_PATH

cd ../ToolDAQFramework

make clean
make

export LD_LIBRARY_PATH=`pwd`/lib:$LD_LIBRARY_PATH

cd ..

sudo apt-get --force-yes --assume-yes install git dpkg-dev cmake g++ gcc binutils libx11-dev libxpm-dev libxft-dev libxext-dev

wget https://root.cern.ch/download/root_v5.34.34.source.tar.gz
tar zxvf root_v5.34.34.source.tar.gz
cd root

./configure --prefix=`pwd` --enable-rpath --with-x11-libdir=/usr/lib/arm-linux-gnueabihf/ --with-xpm-libdir=/usr/lib/arm-linux-gnueabihf/ --with-xft-libdir=/usr/lib/arm-linux-gnueabihf/ --with-xext-libdir=/usr/lib/arm-linux-gnueabihf/
make
make install

cd ../../

make clean
make

