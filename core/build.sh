#! /bin/bash

build_ninja() {
	cmake -S . -B build -G "Ninja" 
	cd build
	ninja
	./RayTracer
}

build_default() {
	cmake -S . -B build -G "Unix Makefiles"
	cd build
	make
	./RayTracer
}

mkdir -p build
while [[ $# -gt 0 ]];
do
	case "$1" in
		-n|--ninja)
			build_ninja 
			exit 1
			;;
		*)
			build_default 
			exit 1
			;;

	esac
done
