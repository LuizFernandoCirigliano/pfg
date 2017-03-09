CFLAGS = -I./remoteApi -I./include -DNON_MATLAB_PARSING \
-DMAX_EXT_API_CONNECTIONS=255 -m64 -stdlib=libc++ -D_GLIBCXX_USE_CXX11_ABI=0
CPPFLAGS = -std=c++11

OS = $(shell uname -s)
ifeq ($(OS), Linux)
    CPPFLAGS += -D__linux
else
    CPPFLAGS += -D__APPLE__
endif

all:
	@rm -f bin/*.*
	@rm -f *.o
	g++ $(CFLAGS) $(CPPFLAGS) -c VRepClass.cpp -o VRepClass.o
	g++ $(CFLAGS) $(CPPFLAGS) -c shape.cpp -o shape.o
	g++ $(CFLAGS) $(CPPFLAGS) -c joint.cpp -o joint.o
	g++ $(CFLAGS) $(CPPFLAGS) -c robot.cpp -o robot.o
	g++ $(INC_DIRS) -lm $(CFLAGS) $(CPPFLAGS) -c main.cpp -o main.o
	gcc $(CFLAGS) -c ./remoteApi/extApi.c -o extApi.o
	gcc $(CFLAGS) -c ./remoteApi/extApiPlatform.c -o extApiPlatform.o
	@mkdir -p bin
	g++ extApi.o extApiPlatform.o VRepClass.o shape.o joint.o robot.o main.o -o bin/main -lpthread $(LIB_DIRS) -lga -lm
