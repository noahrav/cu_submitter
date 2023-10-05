# CU Submitter
A small application designed to help Collective Unconscious developers submit their updates

# Install required librairies
sudo apt install -y g++ qtcreator qtbase5-dev qt5-qmake cmake

and build liblcf: https://wiki.easyrpg.org/development/compiling/liblcf/autotools

# Build and execute
cmake -B build -S . &&
cd build &&
make &&
./cu_submitter
