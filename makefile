###########################################################
#     google test build
###########################################################
GTEST_DIR = ./googletest/googletest/

# Where to find user code.

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = unittest

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.

all : $(TESTS)

clean :
	rm -f $(TESTS) gtest.a gtest_main.a *.o

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

###########################################################
#     unit test build
###########################################################

PROJECT_ROOT=.

INCLUDES =  $(PROJECT_ROOT)/Common \
           -I$(PROJECT_ROOT)/StorageLayer \
           -I$(PROJECT_ROOT)/IO_Stream

LIB_HEADERS = $(PROJECT_ROOT)/Common/common.h

#
# storage unit test
STORAGE_DIR = ./StorageLayer
StorageAccess.o : $(STORAGE_DIR)/StorageAccess.c \
                     $(STORAGE_DIR)/StorageAccess.h $(GTEST_HEADERS) $(LIB_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCLUDES) -c $(STORAGE_DIR)/StorageAccess.c

UnitTest_StorageAccess.o: $(STORAGE_DIR)/UnitTest_StorageAccess.cpp\
                     $(STORAGE_DIR)/StorageAccess.h $(GTEST_HEADERS) $(LIB_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCLUDES) -c $(STORAGE_DIR)/UnitTest_StorageAccess.cpp

IO_STREAM = ./IO_Stream
io_base.o : $(IO_STREAM)/io_base.cpp \
                     $(IO_STREAM)/io_base.hpp\
                     $(STORAGE_DIR)/StorageAccess.h\
                     $(STORAGE_DIR)/StorageDescriptor.h\
                     $(GTEST_HEADERS) $(LIB_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCLUDES) -c $(IO_STREAM)/io_base.cpp

UnitTest_io_base.o : $(IO_STREAM)/UnitTest_io_base.cpp \
                     $(IO_STREAM)/io_base.hpp \
                     io_base.o \
                     $(GTEST_HEADERS) $(LIB_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(INCLUDES) -c $(IO_STREAM)/UnitTest_io_base.cpp

unittest : StorageAccess.o UnitTest_StorageAccess.o gtest_main.a UnitTest_io_base.o io_base.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

