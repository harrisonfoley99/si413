
# SI413 FAY20
# Lab 4
# Makefile for simple calculator

CXX = g++
CXXFLAGS = -Wall -Wextra -Wno-unused-function

compile:
	bison -d lab04.ypp
	flex lab04.lpp
	g++ -o mycalc lab04.tab.cpp lex.yy.c


# Run "make clean" to clear all extra files.
clean:
	rm -f *.o *.yy.cpp *.tab.?pp calc

# This must be here!  It is used by the submit system
run:
	@./mycalc

# This is how we tell make that these two targets aren't actual files.
.PHONY: clean run
