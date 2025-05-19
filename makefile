#
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g


LIBS = -lsfml-graphics -lsfml-window -lsfml-system


GUI_MAIN = GUI/main.cpp
TEST_SRC = tests/test_coup.cpp


MAIN_EXEC = main
GUI_EXEC = gui
TEST_EXEC = test


SRC = \
	coup/Game/Game.cpp \
	coup/Player/Player.cpp \
	roles/Governor/Governor.cpp \
	roles/Judge/Judge.cpp \
	roles/Baron/Baron.cpp \
	roles/General/General.cpp \
	roles/Spy/Spy.cpp \
	roles/Merchant/Merchant.cpp

OBJ = $(SRC:.cpp=.o)


all: $(MAIN_EXEC) $(GUI_EXEC)


$(MAIN_EXEC): coup/main.cpp $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)


$(GUI_EXEC): $(GUI_MAIN) $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

test: $(TEST_SRC) $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $(TEST_EXEC) $(LIBS)
	./$(TEST_EXEC)



valgrind: $(GUI)
	valgrind ./$(GUI_EXEC)


clean:
	rm -f $(OBJ) $(MAIN_EXEC) $(GUI_EXEC) $(TEST_EXEC)

.PHONY: all test valgrind clean
