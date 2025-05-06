# Mail: nachum.amit@msmail.ariel.ac.il

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
INCLUDES = -I./coup -I./roles

SRC = coup/main.cpp \
      coup/Game/Game.cpp \
      roles/Governor/Governor.cpp \
	  roles/Judge/Judge.cpp \
	  roles/Baron/Baron.cpp \
	  coup/Player/Player.cpp

OBJ = $(SRC:.cpp=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
