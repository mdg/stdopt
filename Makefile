CC = g++
DBG = -g
LIB_NAME = liboptionpp.a

INC_OPT = -Iinclude
SRC = *.h *.cpp


all : lib

lib : $(LIB_NAME)

$(LIB_NAME) : compile
	ar r $(LIB_NAME) obj/*.o

compile : obj/configuration.o obj/option.o obj/usage.o

clean :
	rm -f obj/*.o obj/test/*.o

clobber : clean
	rm -f $(LIB_NAME)

install : lib
	mkdir -p /usr/include/stdopt
	cp configuration.h /usr/include/stdopt
	cp usage.h /usr/include/stdopt
	cp $(LIB_NAME) /usr/lib

test : compile compile_test
	$(CC) $(DBG) -o run_stdopt_tests obj/*.o obj/test/*.o -ltestpp

compile_test : obj/test/configuration_test.o obj/test/option_test.o \
	obj/test/usage_test.o

obj :
	mkdir -p obj

obj/test :
	mkdir -p obj/test

obj/configuration.o : obj include/stdopt/configuration.h configuration.cpp
	$(CC) $(DBG) $(INC_OPT) -c -o obj/configuration.o configuration.cpp

obj/option.o : obj include/stdopt/option.h option.cpp
	$(CC) $(DBG) $(INC_OPT) -c -o obj/option.o option.cpp

obj/usage.o : obj include/stdopt/usage.h usage.cpp include/stdopt/option.h
	$(CC) $(DBG) $(INC_OPT) -c -o obj/usage.o usage.cpp

obj/test/configuration_test.o : obj/test include/stdopt/configuration.h \
	test/configuration_test.cpp include/stdopt/option.h
	$(CC) $(DBG) $(INC_OPT) -c -o obj/test/configuration_test.o \
		test/configuration_test.cpp

obj/test/option_test.o : obj/test include/stdopt/option.h \
	test/option_test.cpp
	$(CC) $(DBG) $(INC_OPT) -c -o obj/test/option_test.o \
		test/option_test.cpp

obj/test/usage_test.o : obj/test include/stdopt/usage.h test/usage_test.cpp
	$(CC) $(DBG) $(INC_OPT) -c -o obj/test/usage_test.o test/usage_test.cpp

