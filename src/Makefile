#########################################
# 				CONFIG 					#
#########################################

PROG = ejecutable
CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic
OBJS = auxiliares.o algoritmos.o main.o
DEPS = auxiliares.h algoritmos.h 

#########################################
# 				TARGETS					#
#########################################

.PHONY: all
all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

main.o : main.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

algoritmos.o : algoritmos.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

auxiliares.o : auxiliares.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

#########################################
# 				CLEANING				#
#########################################

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(PROG)