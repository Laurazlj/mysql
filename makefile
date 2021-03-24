CXX=clang++
EXE=sql
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
CXXFLAGS=-g -Wall -DDEBUG -std=c++11
all: prelex depend $(EXE)
depend:
	@$(CXX) -MM $(CXXFLAGS) $(SRC) > .depend

-include .depend
$(EXE):$(OBJ)
	$(CXX) $(OBJ) -o $(EXE)

prelex:
	flex -o lex.yy.cpp tokenize.l
	bison -o parser.tab.cpp  -d parser.y 

clean:
	rm $(EXE) $(OBJ) .depend -f
