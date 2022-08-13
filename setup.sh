echo "Setting up project"

sudo apt install cmake
sudo apt-get install build-essential

chmod a+rx run.sh
chmod a+rx clean_build.sh
chmod a+rx build.sh

cmake .
make
