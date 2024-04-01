################## build ##################
cd .. 
#delete "build" folder if it exists
if [ -d "build" ]; then
    rm -rf build
fi
mkdir build
cd build 
# set CommonAPI configuration file 
cp /conf/commonapi.ini .
cmake -DUSE_INSTALLED_COMMONAPI=ON .. 
make -j $(nproc)
#sudo make install