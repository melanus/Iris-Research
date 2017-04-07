CXX=g++

# Linux
CXXFLAGS=-O3 -std=c++11 -Wall -fmessage-length=0 -s
CXXFLAGS=-g -gdwarf-3 -Wall -std=c++11 -Wformat -I /usr/local/opt/opencv/include/ -I /usr/local/boost-1.63.0/include/ -I /Users/mattlanus/Downloads/matio-1.5.10/src/
LINKFLAGS=-L/usr/local/opt/opencv/lib \
		  -lopencv_core \
		  -lopencv_highgui \
		  -lopencv_imgproc \
		  -lopencv_objdetect \
		  -lopencv_contrib \
		  -lopencv_legacy \
		  -lopencv_nonfree \
		  -lopencv_features2d \
		  -lopencv_photo \
		-L/usr/local/boost-1.63.0/lib \
		  -lboost_filesystem \
		  -lboost_system \
		  -lboost_regex \
		-L/usr/local/lib \
		  -lmatio \


ALLTARGETS=bsif  bsifc

%:%.cpp
	$(CXX) -o $@ $(CXXFLAGS) $< $(LINKFLAGS)

all: ${ALLTARGETS}
clean:
	rm ${ALLTARGETS}
