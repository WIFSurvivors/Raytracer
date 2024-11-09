#! /bin/bash

build_ninja() {
	mkdir -p build
	cmake -S . -B build -G "Ninja"
	cd build || exit
	ninja
	./RayTracer
}

build_default() {
	mkdir -p build
	cmake -S . -B build -G "Unix Makefiles"
	cd build || exit
	make
	./RayTracer
}

build_debug() {
	mkdir -p build-debug
	cmake -S . -B build-debug -G "Ninja" -D GLFW_BUILD_X11=1 -D GLFW_BUILD_WAYLAND=0
	cd build-debug || exit
	ninja
	./RayTracer
}

while [[ $# -gt 0 ]];do
	case "$1" in
	-n | --ninja)
	build_ninja
		exit 1
		;;
	-d | --debug)
		build_debug
		exit 1
		;;
	*)
		build_default
		exit 1
		;;
	esac
