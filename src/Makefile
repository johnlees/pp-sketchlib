
CXXFLAGS=-Wall -Wextra -O2 -std=c++14
ifdef DEBUG
  CXXFLAGS=-Wall -Wextra -O0 -g -std=c++14
endif


CPPFLAGS=-I../dlib -I../rollinghashcpp -I../HighFive/include -I/Users/jlees/miniconda3/envs/pp_sketch/include -I/Users/jlees/miniconda3/envs/pp_sketch/include/eigen3 -D DLIB_NO_GUI_SUPPORT=1 -D DLIB_USE_BLAS=1 -D DLIB_USE_LAPACK=1

LDFLAGS=-L/Users/jlees/miniconda3/envs/pp_sketch/lib -lz -lbz2 -lhdf5_cpp -lhdf5 -llapack -lblas -pthread

PROGRAMS=sketch_test

SKETCH_OBJS=dist.o reference.o seqio.o sketch.o database.o api.o linear_regression.o link_function.o

all: $(PROGRAMS)

static: $(STATIC_PROGRAMS)

clean:
	$(RM) *.o ~* $(PROGRAMS)

install: all
	install -d $(BINDIR)
	install $(PROGRAMS) $(BINDIR)

sketch_test: $(SKETCH_OBJS) main.o
	$(LINK.cpp) $^ -o $@



.PHONY: all clean install