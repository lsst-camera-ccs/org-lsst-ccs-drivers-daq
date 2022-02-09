ARCH = lib64
TGT = ../../../target/classes
OBJ = $(TGT)/$(ARCH)/obj
LIBTGT = $(TGT)/$(ARCH)
CC = g++ -std=c++0x -fPIC -Wno-deprecated
INC = $(TGT)/headers

DAQ_HOME ?= ../../../lib64/daq/R5-V1.0

all: $(LIBTGT)/libccs_daq_ims.so

$(OBJ)/Store.o: Store.cpp $(INC)/org_lsst_ccs_daq_ims_StoreNativeImplementation.h
	mkdir -p $(OBJ)
	$(CC) -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -o $@ -c $<

$(OBJ)/MyFolders.o: MyFolders.cpp MyFolders.h
	mkdir -p $(OBJ)
	$(CC) -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -o $@ -c $<

$(OBJ)/MyProcessor.o: MyProcessor.cpp MyProcessor.h
	mkdir -p $(OBJ)
	$(CC) -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -o $@ -c $<

$(OBJ)/MyHarvester.o: MyHarvester.cpp MyHarvester.h
	mkdir -p $(OBJ)
	$(CC) -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -o $@ -c $<

$(OBJ)/MyBarrier.o: MyBarrier.cpp MyBarrier.h
	mkdir -p $(OBJ)
	$(CC) -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -o $@ -c $<

$(OBJ)/Channel.o: Channel.cpp Channel.h $(INC)/org_lsst_ccs_daq_ims_DAQSourceChannel.h
	mkdir -p $(OBJ)
	$(CC) -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -o $@ -c $<

$(OBJ)/Statistics.o: Statistics.cpp $(INC)/org_lsst_ccs_daq_ims_Stats.h
	mkdir -p $(OBJ)
	$(CC) -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -o $@ -c $<

$(OBJ)/Playlist.o: Playlist.cpp $(INC)/org_lsst_ccs_daq_ims_Emulator.h
	mkdir -p $(OBJ)
	$(CC) -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -o $@ -c $<

$(LIBTGT)/libccs_daq_ims.so: $(OBJ)/Store.o $(OBJ)/MyFolders.o $(OBJ)/MyProcessor.o $(OBJ)/Channel.o $(OBJ)/MyBarrier.o $(OBJ)/Statistics.o $(OBJ)/Playlist.o $(OBJ)/MyHarvester.o
	mkdir -p ${LIBTGT}
	$(CC) -shared -Wl,-rpath=$(DAQ_HOME)/x86/lib -L$(DAQ_HOME)/x86/lib -ldaq_ims -ldaq_dsm -ldaq_rms -ldaq_rds -ldaq_cms -ldaq_rms -o $@ $^
