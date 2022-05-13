PROG = main
SRCS = bigint_test.cpp memtrace.cpp
HDRS = bigint.h algorithms.h message.h gtest_lite.h memtrace.h
OBJS = $(SRCS:.cpp=.o)

CXXFLAGS = -ggdb -std=c++11 -pedantic -Wall -DMEMTRACE
#CXXFLAGS = -Ofast -std=c++11 -pedantic

$(PROG): $(OBJS) 
	$(CXX) -o $(PROG) $(OBJS)

.PHONY:
clean:
	rm -f $(OBJS) $(PROG)

# Egyszerusites: Minden .o fugg minden header-tol, es meg a Makefile-tol is 
$(OBJS): $(HDRS) Makefile
