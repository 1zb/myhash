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
# train.o: $(SRC)/train.cpp
# 	$(CXX) $(CFLAGS) $^ -c -o $(TARGET)/$@

# utils.o: $(SRC)/utils.cpp
# 	$(CXX) $(CFLAGS) $^ -c -o $(TARGET)/$@

$(TARGET)/%.o: $(SRC)/%.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

gen_gist: $(TARGET)/gen_gist.o $(LEAR_GIST)/gist.o $(LEAR_GIST)/standalone_image.o
	$(CXX) $(CFLAGS) $^ -o $(TARGET)/$@ -lm -lfftw3f

$(TARGET)/gen_gist.o: $(SRC)/gen_gist.cpp
	$(CXX) -c $(CFLAGS) -I$(LEAR_GIST) $< -o $@

# gist: $(LEAR_GIST)/gist.o $(LEAR_GIST)/standalone_image.o

$(LEAR_GIST)/gist.o: $(LEAR_GIST)/gist.c
	$(CC) -c -Wall -g $< -DUSE_GIST -DSTANDALONE_GIST -o $@

$(LEAR_GIST)/standalone_image.o: $(LEAR_GIST)/standalone_image.c
	$(CC) -c -Wall -g $< -o $@

.PHONY: clean
clean:
	rm ./build/*
# LIBLINEAR = 3rdparty\liblinear-2.1

# all: train test half_ones

# train: train.obj utils.obj
#   $(CXX) $(CFLAGS) -Fe$(TARGET)\train.exe $(TARGET)\utils.obj $(TARGET)\train.obj /link $(LIBLINEAR)\windows\liblinear.lib

# test: test.obj utils.obj
#   $(CXX) $(CFLAGS) -Fe$(TARGET)\test.exe $(TARGET)\utils.obj $(TARGET)\test.obj /link $(LIBLINEAR)\windows\liblinear.lib

# half_ones: half_ones.obj utils.obj
#   $(CXX) $(CFLAGS) -Fe$(TARGET)\half_ones.exe $(TARGET)\utils.obj $(TARGET)\half_ones.obj /link $(LIBLINEAR)\windows\liblinear.lib

# utils.obj: $(SRC)\utils.cpp $(SRC)\utils.hpp
#   $(CXX) $(CFLAGS) -Fo$(TARGET)\utils.obj -c $(SRC)\utils.cpp /I $(LIBLINEAR)

# train.obj: $(SRC)\train.cpp
#   $(CXX) $(CFLAGS) -Fo$(TARGET)\train.obj -c $(SRC)\train.cpp -I $(SRC) /I $(LIBLINEAR)

# test.obj: $(SRC)\test.cpp
#   $(CXX) $(CFLAGS) -Fo$(TARGET)\test.obj -c $(SRC)\test.cpp -I $(SRC) /I $(LIBLINEAR)

# half_ones.obj: $(SRC)\half_ones.cpp
#   $(CXX) $(CFLAGS) -Fo$(TARGET)\half_ones.obj -c $(SRC)\half_ones.cpp -I $(SRC) /I $(LIBLINEAR)

# clean:
#   -erase /Q $(TARGET)\*.exe $(TARGET)\*.obj

# CXX = g++
# CFLAGS = -std=c++11
# TARGET = ./build
# SRC = ./src

# QPBO_DIR = ./3rdparty/QPBO-v1.4.src
# QPBO_FLAGS = -I$(QPBO_DIR) -L$(QPBO_DIR) -lqpbo
# LIBLINEAR_DIR = ./3rdparty/liblinear-2.1
# LIBLINEAR_FLAGS = -I$(LIBLINEAR_DIR) -L

# all: ilh myhash

# ilh: $(SRC)/ilh.cpp
# 	$(CXX) $(CFLAGS) $^ -o $(TARGET)/$@ $(QPBO_FLAGS) -llinear -lblas

# myhash: $(SRC)/myhash.cpp
	# $(CXX) $(CFLAGS) $^ -o $(TARGET)/$@ -llinear