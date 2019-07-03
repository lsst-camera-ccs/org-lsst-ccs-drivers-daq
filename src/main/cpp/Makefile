ARCH = lib64
TGT = ../../../target
OBJ = $(TGT)/$(ARCH)/obj
LIBTGT = $(TGT)/$(ARCH)
CC = g++
INC = $(TGT)/headers

all: $(LIBTGT)/libdaq_ims.so

$(OBJ)/Store.o: Store.cpp $(INC)/org_lsst_ccs_daq_imageapi_Store.h
	mkdir -p $(OBJ)
	$(CC) -std=c++11 -fPIC -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -fPIC -o $@ -c $<

$(OBJ)/MyFolders.o: MyFolders.cpp MyFolders.h
	mkdir -p $(OBJ)
	$(CC) -std=c++11 -fPIC -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -fPIC -o $@ -c $<

$(OBJ)/MyProcessor.o: MyProcessor.cpp MyProcessor.h
	mkdir -p $(OBJ)
	$(CC) -std=c++11 -fPIC -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -fPIC -o $@ -c $<

$(OBJ)/MyInspector.o: MyInspector.cpp MyInspector.h
	mkdir -p $(OBJ)
	$(CC) -std=c++11 -fPIC -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -fPIC -o $@ -c $<

$(OBJ)/MyReader.o: MyReader.cpp MyReader.h
	mkdir -p $(OBJ)
	$(CC) -std=c++11 -fPIC -I. -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux/ -I$(TGT)/headers -I$(DAQ_HOME)/include -fPIC -o $@ -c $<


$(LIBTGT)/libdaq_ims.so: $(OBJ)/Store.o $(OBJ)/MyFolders.o $(OBJ)/MyProcessor.o $(OBJ)/MyInspector.o $(OBJ)/MyReader.o
	mkdir -p ${LIBTGT}
	$(CC) -shared -L$(DAQ_HOME)/x86/lib -ldaq_ims -ldaq_dsm -o $@ $^
	


