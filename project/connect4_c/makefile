CC = clang
CFLAGS = -g -Wall -std=c11

c4_objects = connect4.o connect4_engine.o
c4: $(c4_objects)
	clang $(CFLAGS) $(c4_objects) -o $@

c4_test_objects = connect4_engine.o CuTest.o c4_engine_test.o
c4_test: $(c4_test_objects)
	clang $(CFLAGS) $(c4_test_objects) -o $@
