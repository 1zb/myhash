CXX = g++
CC = gcc
SRC = ./src
TARGET = ./build
CFLAGS = -std=c++11 -Wall -O3 -fPIC -I$(SRC) -Wconversion -D_LINEAR1
OBJS = utils.o train.o

LEAR_GIST = ./3rdparty/lear_gist-1.2

all: train test half_ones test_all test_nchoosek

train: $(TARGET)/train.o $(TARGET)/utils.o
	$(CXX) $(CFLAGS) $^ -o $(TARGET)/$@ -llinear

test: $(TARGET)/test.o $(TARGET)/utils.o
	$(CXX) $(CFLAGS) $^ -o $(TARGET)/$@ -llinear

test_all: $(TARGET)/test_all.o $(TARGET)/utils.o
	$(CXX) $(CFLAGS) $^ -o $(TARGET)/$@ -llinear

half_ones: $(TARGET)/half_ones.o $(TARGET)/utils.o
	$(CXX) $(CFLAGS) $^ -o $(TARGET)/$@ -llinear

test_nchoosek: $(TARGET)/test_nchoosek.o $(TARGET)/utils.o
	$(CXX) $(CFLAGS) $^ -o $(TARGET)/$@ -llinear


$(TARGET)/%.o: $(SRC)/%.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

gen_gist: $(TARGET)/gen_gist.o $(LEAR_GIST)/gist.o $(LEAR_GIST)/standalone_image.o
	$(CXX) $(CFLAGS) $^ -o $(TARGET)/$@ -lm -lfftw3f

$(TARGET)/gen_gist.o: $(SRC)/gen_gist.cpp
	$(CXX) -c $(CFLAGS) -I$(LEAR_GIST) $< -o $@

$(LEAR_GIST)/gist.o: $(LEAR_GIST)/gist.c
	$(CC) -c -Wall -g $< -DUSE_GIST -DSTANDALONE_GIST -o $@

$(LEAR_GIST)/standalone_image.o: $(LEAR_GIST)/standalone_image.c
	$(CC) -c -Wall -g $< -o $@

.PHONY: clean
clean:
	rm ./build/*
