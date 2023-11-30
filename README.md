# CU Submitter
A small application designed to help Collective Unconscious developers submit their updates

# Install required librairies
sudo apt install -y git g++ qtcreator qtbase5-dev qt5-qmake cmake zip 

# Build
```
cd
git clone https://github.com/noahrav/cu_submitter.git
cd cu_submitter
cmake -B build
cd build
make
```

The executable is build/cu_submitter

## CLI

./cu_submitter --help | --usage : prints the usage\
./cu_submitter --chgen <base_path> <modified_path> : generates a changelog text file\
./cu_submitter --transfer <unmodified_copy_path> <modified_copy_path> <destination_path> : transfers the modified files to the destination path\
./cu_submitter --submit <unmodified_copy_path> <modified_copy_path> [<archive_path>] : compress the modified files to a submission archive
