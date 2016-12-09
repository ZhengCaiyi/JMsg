TAGRET = jmsg_generator
DEPS = jmsg_generator.o \
	   jmsg_code_writer.o
	   
INCLUDES = -I../src/
CFLAGS = $(INCLUDES) -g
CXXFLAGS = $(INCLUDES) -std=c++11 -g

$(TAGRET): $(DEPS) LIBMSG
	g++ -o $(TAGRET) $(DEPS) ../src/libjmsg.a

LIBMSG:
	make -C ../src

clean:
	make clean -C  ../src
	rm -f $(TAGRET) $(DEPS)