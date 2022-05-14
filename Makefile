PROG = main
SRCS = rsa_test.cpp memtrace.cpp
HDRS = bigint.h algorithms.h message.h gtest_lite.h memtrace.h
OBJS = $(SRCS:.cpp=.o)
CXXFLAGS = -ggdb -std=c++17 -Wall -Werror -DMEMTRACE
#CXXFLAGS = -Ofast -std=c++17

$(PROG): $(OBJS) 
	$(CXX) -o $(PROG) $(OBJS)

.PHONY:
clean:
	rm -f $(OBJS) $(PROG)

# Egyszerusites: Minden .o fugg minden header-tol, es meg a Makefile-tol is 
$(OBJS): $(HDRS) Makefile
