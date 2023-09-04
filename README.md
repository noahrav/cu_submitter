# CU Submitter
A small application designed to help Collective Unconscious developers submit their updates

# Install required librairies
sudo apt install -y qtcreator qtbase5-dev qt5-qmake cmake

# Build and execute
cmake -B build -S . &&
cd build &&
make &&
./cu_submitter
