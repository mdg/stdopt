CC = g++
DBG = -g
LIB_NAME = liboptionpp.a

SRC = *.h *.cpp


all : lib

lib : $(LIB_NAME)

$(LIB_NAME) : compile $(SRC)
	ar r $(LIB_NAME) *.o

compile : configuration.o usage.o

.o : $.cpp $.h
	$(CC) $(DBG) $.cpp

clean :
	rm -f *.o

clobber : clean
	rm -f $(LIB_NAME)

install : lib
	mkdir -p /usr/include/stdopt
	cp configuration.h /usr/include/stdopt
	cp usage.h /usr/include/stdopt
	cp $(LIB_NAME) /usr/lib

compile_test : test/configuration_test.o test/usage_test.o

test/configuration_test.o : configuration.h test/configuration_test.cpp
	$(CC) $(DBG) -o test/configuration_test.o test/configuration_test.cpp

test/usage_test.o : usage.h test/usage_test.cpp
	$(CC) $(DBG) -o test/usage_test.o test/usage_test.cpp

