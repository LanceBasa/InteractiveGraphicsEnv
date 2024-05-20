<!-- # CITS3003 Project S1 2024
## Authors: Lance Basa 23420659, Sahil Narula 23313963

##
Built using the following OS

Distributor ID: Ubuntu
Description:    Ubuntu 23.10
Release:        23.10
Codename:       mantic



How to run
Build the following code using VScode terminal on linux or Ubuntu Command Line
Unzip the zipepd file make sure to cd into the project directory cits3003_project.
Then to generate build files, run (these only needs to be re-run if you update CMakeLists.txt)
    cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
    cmake -S . -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
Then to build the debug profile, run:  
    cmake --build cmake-build-debug 
Then to build the release profile, run:
    cmake --build cmake-build-release
Then to run the last built profile, run:  
    ./cits3003_project -->


# CITS3003 Project S1 2024
## Authors: Lance Basa, Sahil Narula

Built using the following OS:
- Distributor ID: Ubuntu
- Description: Ubuntu 23.10
- Release: 23.10
- Codename: mantic

Built in Ubuntu VSCode environment


### How to Run
1. Unzip the zipped file and make sure to navigate to the project directory `cits3003_project`.
2. To generate build files, run the following commands (these only need to be re-run if you update `CMakeLists.txt`):
   ```bash
        cmake -S . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug 
        cmake -S . -B cmake-build-release -DCMAKE_BUILD_TYPE=Release 
   ```
3. To build the debug profile, run:
   ```bash
        cmake --build cmake-build-debug
   ```
4. To build the release profile, run:
   ```bash
        cmake --build cmake-build-release 
   ```
5. To run the last built profile, run:
    ```bash
        ./cits3003_project 
    ```
Note: When running in VScode, ensure that GTK_PATH is set to none for load and save to work.
To check if GTK_PATH is set, run:
    ```bash
        echo $GTK_PATH
    ```
To clear GTK_PATH in VSCode, run:
    ```bash
        export GTK_PATH=
    ```
