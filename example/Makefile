TARGET=jmsg_example
JMSG_SRC = ../src/*.cpp
EXAMPLE_SRC = ./*.cpp
JSONCPP_SRC = ../jsoncpp/src/lib_json/*.cpp
INCLUDES = -I../src/ \
		   -I./ \
		   -I../jsoncpp/include/

TARGET:$(JMSG_SRC) $(EXAMPLE_SRC) $(JSONCPP_SRC)
	g++  $(INCLUDES) -o $(TARGET) $(JMSG_SRC) $(EXAMPLE_SRC) $(JSONCPP_SRC)

clean:
	rm -f $(TARGET)