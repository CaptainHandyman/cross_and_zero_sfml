CXX = g++ -std=c++14 

LIBS = -lsfml-graphics \
	   -lsfml-system \
	   -lsfml-window

SOURCE_FILES = *.cpp

all:
	$(CXX) $(SOURCE_FILES) $(LIBS)