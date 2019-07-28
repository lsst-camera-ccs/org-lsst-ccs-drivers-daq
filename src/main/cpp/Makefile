ARCH = lib64
TGT = ../../../target
OBJ = $(TGT)/$(ARCH)/obj
LIBTGT = $(TGT)/$(ARCH)
CC = g++
INC = $(TGT)/headers

DAQ_HOME ?= $(HOME)/projects/lsst/daq/R4-V1.0-IR2

all: $(LIBTGT)/libccs_daq_ims.so

$(OBJ)/Store.o: Store.cpp $(INC)/org_lsst_ccs_daq_imageapi_Store.h
	mkdir -p $(OBJ)
	$(CC) -std=c++11 -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -fPIC -o $@ -c $<

$(OBJ)/MyFolders.o: MyFolders.cpp MyFolders.h
	mkdir -p $(OBJ)
	$(CC) -std=c++11 -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -fPIC -o $@ -c $<

$(OBJ)/MyProcessor.o: MyProcessor.cpp MyProcessor.h
	mkdir -p $(OBJ)
	$(CC) -std=c++11 -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -fPIC -o $@ -c $<

$(OBJ)/MyBarrier.o: MyBarrier.cpp MyBarrier.h
	mkdir -p $(OBJ)
	$(CC) -std=c++11 -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -fPIC -o $@ -c $<

$(OBJ)/Channel.o: Channel.cpp Channel.h
	mkdir -p $(OBJ)
	$(CC) -std=c++11 -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -fPIC -o $@ -c $<

$(LIBTGT)/libccs_daq_ims.so: $(OBJ)/Store.o $(OBJ)/MyFolders.o $(OBJ)/MyProcessor.o $(OBJ)/Channel.o $(OBJ)/MyBarrier.o
	mkdir -p ${LIBTGT}
	$(CC) -shared -Wl,-rpath=$(DAQ_HOME)/x86/lib -L$(DAQ_HOME)/x86/lib -ldaq_ims -ldaq_dsm -o $@ $^
	
