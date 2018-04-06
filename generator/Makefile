TARGET=jmsg_generator
JMSG_SRC = ../src/*.cpp
GENERATOR_SRC = ./*.cpp
JSONCPP_SRC = ../jsoncpp/src/lib_json/*.cpp
INCLUDES = -I../src/ \
		   -I./ \
		   -I../jsoncpp/include/

TARGET:$(JMSG_SRC) $(GENERATOR_SRC) $(JSONCPP_SRC)
	g++  $(INCLUDES) -o $(TARGET) $(JMSG_SRC) $(GENERATOR_SRC) $(JSONCPP_SRC)

clean:
	rm -f $(TARGET)