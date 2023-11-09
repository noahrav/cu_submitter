# CU Submitter
A small application designed to help Collective Unconscious developers submit their updates

# Install required librairies
sudo apt install -y g++ qtcreator qtbase5-dev qt5-qmake cmake

# Build and execute
cmake -B build -S . &&
cd build &&
make &&
./cu_submitter

## CLI

./cu_submitter --help | --usage : prints the usage\
./cu_submitter --chgen <base_path> <modified_path> : generates a changelog text file\
./cu_submitter --transfer <unmodified_copy_path> <modified_copy_path> <destination_path> : transfers the modified files to the destination path
