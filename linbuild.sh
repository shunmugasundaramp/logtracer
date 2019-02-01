#!/bin/bash

# ****************************************************************************
# * Description: This build system is used to compile the Test Client source
# * Author(s): Shunmuga (ssundaramp@outlook.com)
# ****************************************************************************

declare -a CPPFILES=(	"Tracer.cpp"
						"usage.cpp")

prepareOutDir() {
	rm -fr objs/*
	mkdir objs
}

cppCompile() {
	OBJS=""
	for i in "${CPPFILES[@]}"
	do
		INFILE=$i
		OUTFILE=${i//.cpp/.o}
		echo -e '\e[96m*** Compiling '$INFILE'\e[0m'
		g++ $INFILE -c -o objs/$OUTFILE -I./ -std=c++11
		OBJS=$OBJS" "objs/$OUTFILE
	done
	g++ -o objs/usage.exe $OBJS
}

prepareOutDir
cppCompile
