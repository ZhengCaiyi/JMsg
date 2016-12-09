TARGET = libjmsg.a
DEPS = jmsg_idl_parse.o \
	   jmsg_util.o \
	   jmsg_proto.o \
	   jmsg_reader.o \
	   jmsg_writer.o \
	   jmsg_type.o
	   
CXXFLAGS = -g -std=c++11
TARGET: $(DEPS)
	$(AR) -rcs $(TARGET) $(DEPS)

clean:
	rm -f $(TARGET) $(DEPS)