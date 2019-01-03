CXXFLAGS := -lncurses
#ifdef env
#	ifeq ($(env), mac)
#		CXXFLAGS := -lncurses
#	else
#		CXXFLAGS := -lncursesw
#	endif
#else
#	CXXFLAGS := -lncursesw
#endif

CXX := g++
PROGRAM := prime

SRCS = main.cpp src.cpp hand.cpp board.cpp myio.cpp
OBJS := $(SRCS:.cpp=.o) #SRCSの各ファイルのサフィックスの.cppを.oに変換

$(PROGRAM): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(CXXFLAGS)

.PHONY: clean
clean:
	rm -f $(PROGRAM) $(OBJS)