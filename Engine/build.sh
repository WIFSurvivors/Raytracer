#! /bin/bash

build() {
	# mkdir -p build && \
	cmake -S . -B build -G "Unix Makefiles" && \
	cd build || exit && \
	make && \
	./RayTracer
}

debug() {
	# mkdir -p build-debug && \
	cmake -D CMAKE_BUILD_TYPE=DEBUG -D GLFW_BUILD_X11=1 -D GLFW_BUILD_WAYLAND=0 -S . -B build-debug -G "Ninja" && \
	cd build-debug || exit && \
	ninja && \
	./Engine
}

while [[ $# -gt 0 ]];do
	case "$1" in
	-d | --debug)
		debug
		exit 1
		;;
	*)
		build
		exit 1
		;;
	esac
done
