CFLAGS = -I./remoteApi -I./include -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255
CFLAGS += -m64 -stdlib=libc++ -D_GLIBCXX_USE_CXX11_ABI=0

OS = $(shell uname -s)
ifeq ($(OS), Linux)
    CXXFLAGS += -D__linux
else
    CXXFLAGS += -D__APPLE__
endif

all:
	@rm -f bin/*.*
	@rm -f *.o
	g++ $(CFLAGS) -std=c++11 -c main.cpp -o main.o
	gcc $(CFLAGS) -c ./remoteApi/extApi.c -o extApi.o
	gcc $(CFLAGS) -c ./remoteApi/extApiPlatform.c -o extApiPlatform.o
	@mkdir -p bin
	g++ extApi.o extApiPlatform.o main.o -o bin/main -lpthread
