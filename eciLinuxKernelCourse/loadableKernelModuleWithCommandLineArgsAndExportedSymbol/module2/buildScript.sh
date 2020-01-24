#! /bin/bash

echo "=================="
echo "start build script"
echo "=================="

curr_dir=$(pwd)
echo "current working directory is:${curr_dir}"

build_dir_name="build"
if [ -d ${curr_dir}/${build_dir_name} ]; then
	echo "removing previous directory: ${curr_dir}/${build_dir_name}"
	rm -rf ${curr_dir}/${build_dir_name}/ # && rmdir ${curr_dir}/${build_dir_name}
	echo "creating the new (empty) ${curr_dir}/${build_dir_name} folder"	
fi 

echo "creating the ${curr_dir}/${build_dir_name}"
mkdir ${curr_dir}/${build_dir_name}
echo "copying all C files and the Makefile from folder:${curr_dir} to folder:${build_dir_name}"
cp -a ${curr_dir}/*.c ${curr_dir}/"Makefile" ${curr_dir}/${build_dir_name}
echo "running make"
make -C ${curr_dir}/${build_dir_name}

echo "=================="
echo "end build script"
echo "=================="
