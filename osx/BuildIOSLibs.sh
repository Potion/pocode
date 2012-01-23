#!/bin/bash

build_lib() {
	if [ -z $1 ]; then
		set "Release"
	fi
	
	TAIL=""
	if [ $1 = "Debug" ]; then
		TAIL="_d"
	fi

	xcodebuild -target pocode_iOS -configuration $1 -sdk iphonesimulator
	xcodebuild -target pocode_iOS -configuration $1 -sdk iphoneos

	lipo -create build/$1-iphoneos/libpocode_iOS$TAIL.a build/$1-iphonesimulator/libpocode_iOS$TAIL.a -output ./libpocode_iOS$TAIL.a
	mv libpocode_iOS$TAIL.a /pocode/libraries/pocode
}

help() {
	echo "Usage: ./BuildIOSLibs.sh [Debug|Release|All|Clean]"
}

if [[ -n "$1" ]]; then
	if [ $1 = "-h" -a $1 = "--help" ]; then
		help
	elif [[ $1 = "Debug" ]]; then
		build_lib "Debug"
	elif [[ $1 = "Release" ]]; then
		build_lib 
	elif [[ $1 = "All" ]]; then
		build_lib "Debug"
		build_lib
	elif [[ $1 = "Clean" ]]; then
		xcodebuild -target pocode_iOS -configuration Debug -sdk iphonesimulator clean
		xcodebuild -target pocode_iOS -configuration Debug -sdk iphoneos clean
		xcodebuild -target pocode_iOS -configuration Release -sdk iphonesimulator clean
		xcodebuild -target pocode_iOS -configuration Release -sdk iphoneos clean
		
		DIR=/pocode/libraries/pocode
		if [ -a $DIR/libpocode_iOS_d.a ]; then rm $DIR/libpocode_iOS_d.a; fi
		if [ -a $DIR/libpocode_iOS.a ]; then rm $DIR/libpocode_iOS.a; fi
	else
		help
	fi
else
	help
fi