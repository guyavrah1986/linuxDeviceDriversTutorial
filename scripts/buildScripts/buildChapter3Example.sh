#!/bin/bash
# ============================================================================================
# The first argument (which is labled as $1) is the location of the sources and makefiles we  
# wish to copy from there into the build folder in order to "out-of-source-build".
# This location should be providied as a full path. 
# ============================================================================================


# get current directory
echo "===== Start =====" 
echo "Running script `basename $0`"
echo "---------------------------------"

curr_dir=$(dirname $0)
echo "Current directory is:$curr_dir"

# go "up" to the root folder, AKA network-driver
base_folder_name="network-driver"
cd "${PWD/\/$base_folder\/*//$base_folder}"
base_folder=$(pwd)
echo "Moved to:$base_folder"

# check if the build directory exsits
build_dir_name="build"
if [ -d "$build_dir_name" ]; then
	echo "Deleting current build folder"
	sudo rm -r -f $build_dir_name
fi

# create a new empty build folder
mkdir $build_dir_name
echo "Created an empty build folder"
cd $build_dir_name

# copy the sources and Makefile from the given location
echo "Copying files from:$1 to:$build_dir_name"
cp -r $1 .

# TODO: changed it to be dynamically set
cd pomeranzBook/kernelModule
curr_dir=$(pwd)
echo "Running make in folder:$curr_dir"

# run make to build the kernel module
make

# setting the read & write permissions to the device node file
echo "Setting read & write permissions to file:$2"
sudo chmod a+r+w $2

cd ..
cd userspaceApps
curr_dir=$(pwd)
echo "Copying files from:$curr_dir to:$build_dir_name"

# run cmake followed by make to build the user space app(s)
echo "Running cmake to create Makefile for the user space apps"
cmake .

echo "Running make to create the executalbes"
make

echo "===== Done =====" 

