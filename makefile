# Mail: nachum.amit@msmail.ariel.ac.il

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
INCLUDES = -I./coup -I./roles

# Add SFML libraries for GUI rendering
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

SRC = coup/main.cpp \
      coup/Game/Game.cpp \
      coup/Player/Player.cpp \
      roles/Governor/Governor.cpp \
      roles/Judge/Judge.cpp \
      roles/Baron/Baron.cpp \
      roles/General/General.cpp \
      roles/Spy/Spy.cpp \
      roles/Merchant/Merchant.cpp

OBJ = $(SRC:.cpp=.o)
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)
