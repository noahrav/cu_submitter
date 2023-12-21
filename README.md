# CU Submitter
A small application designed to help Collective Unconscious developers submit their updates

# Install required librairies
Cmake > 3.14

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
./cu_submitter [-p <port>] : opens backend server on specific port; 8080 by default\
./cu_submitter --chgen <base_path> <modified_path> : generates a changelog text file\
./cu_submitter --transfer <unmodified_copy_path> <modified_copy_path> <destination_path> : transfers the modified files to the destination path\
./cu_submitter --submit <unmodified_copy_path> <modified_copy_path> [<archive_path>] : copy the modified files to a submission folder
