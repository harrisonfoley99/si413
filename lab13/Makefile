## SI 413 Fall 2018
## Makefile for Lab 11

SHELL:=/bin/bash
CPPFLAGS ?= -Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -ggdb -fmax-errors=3 -g
CXXFLAGS ?= -std=c++11

PROGS=splc
CODEF = ast.cpp spl.tab.cpp spl.yy.cpp
HEADERS = ast.hpp spl.tab.hpp splc.hpp colorout.hpp readlineistream.hpp

# Default target
all: $(PROGS)

# Dependencies

# Rules to generate the final compiled parser programs
$(PROGS): %: %.cpp $(CODEF) $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< $(CODEF) -lreadline

# Generic rule for running C++-style flex code generation
# For instance, this will make 'pat.yy.cpp' from 'pat.lpp'.
%.yy.cpp: %.lpp
	flex -o $@ $<

# Generic rule for bison code generation
%.tab.cpp %.tab.hpp: %.ypp
	bison -d $<

# Generic rule to compile from SPL to LLVM IR
%.ll: %.spl splc
	./splc $<

# Generic rule to compile from LLVM IR to an executable
%: %.ll
	clang -O2 $< -o $@

.PHONY: clean all
clean:
	rm -f *.o *.yy.cpp *.tab.* $(PROGS)
