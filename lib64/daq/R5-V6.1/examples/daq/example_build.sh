#!/bin/sh

# Replace this with the path to the DAQ release
RELEASE=../..

g++ -o daq_ls_example.o -c -std=gnu++0x -g -Wall -DSINGLE_TASK -I${RELEASE}/include daq_ls_example.cc
g++ -o daq_ls_example daq_ls_example.o -Wl,-rpath=${RELEASE}/x86/lib -L${RELEASE}/x86/lib -ldaq_daq -ldaq_dvi -ldaq_osa -ldaq_dsi -ldaq_net -ldaq_hash
