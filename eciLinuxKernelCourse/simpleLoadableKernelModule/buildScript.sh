#! /bin/bash

echo "=================="
echo "start build script"
echo "=================="

curr_dir=$(pwd)
echo "current working directory is:${curr_dir}"

build_dir_name="build"
if [ -d ${build_dir_name} ]; then
	echo "removing previous ${build_dir_name}"
	rm -rf ${build_dir_name}/* && rmdir ${build_dir_name}
	echo "creating the new (empty) ${build_dir_name} folder"	
fi 

echo "creating the ${build_dir_name}"
mkdir ${build_dir_name}
echo "copying all C files and the Makefile from folder:${curr_dir} to folder:${build_dir_name}"
cp -a ${curr_dir}/*.c ${curr_dir}/"Makefile" ${build_dir_name}
echo "running make"
make -C ${build_dir_name}

echo "=================="
echo "end build script"
echo "=================="
