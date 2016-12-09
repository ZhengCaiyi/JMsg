TAGRET = jmsg_test
DEPS = main.o
INCLUDES = -I../src/
CFLAGS = $(INCLUDES) -g
CXXFLAGS = $(INCLUDES) -std=c++11

$(TAGRET): $(DEPS) LIBMSG
	g++ -o $(TAGRET) $(DEPS) ../src/libjmsg.a

LIBMSG:
	make -C ../src

clean:
	make clean -C  ../src
	rm -f $(TAGRET) $(DEPS)