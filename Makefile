########LIBRAIRIES
LIBS_ffmpeg = -lm -lz -lpthread -lavformat -lavcodec -lavutil

LIBS_opencv = -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video -lopencv_objdetect

LIBS_autres = -lpthread -ldl -lm

LIBS = $(LIBS_autres) $(LIBS_ffmpeg) $(LIBS_opencv)

########CONSTANTES
CXXFLAGS = -O2 -g -Wall -fmessage-length=0 -D__STDC_CONSTANT_MACROS 
OBJS = main.o CommonFunc.o IStereoAlg.o BinaryStereo.o
TARGET = main

########REGLES
$(TARGET): $(OBJS) 
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.cpp 
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(LIBS)

all: $(TARGET)

clean: 
	rm -f $(OBJS) $(TARGET)
