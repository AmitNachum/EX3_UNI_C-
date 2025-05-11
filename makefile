# SFML GUI libraries
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# File that contains GUI's main()
GUI_MAIN = GUI/main.cpp

# Executables
MAIN_EXEC = main
GUI_EXEC = gui

# Shared sources (no GUI/main.cpp here!)
SRC = coup/Game/Game.cpp \
      coup/Player/Player.cpp \
      roles/Governor/Governor.cpp \
      roles/Judge/Judge.cpp \
      roles/Baron/Baron.cpp \
      roles/General/General.cpp \
      roles/Spy/Spy.cpp \
      roles/Merchant/Merchant.cpp

OBJ = $(SRC:.cpp=.o)

# === Build both console and GUI ===
all: $(MAIN_EXEC) $(GUI_EXEC)

$(MAIN_EXEC): coup/main.cpp $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

$(GUI_EXEC): $(GUI_MAIN) $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(MAIN_EXEC) $(GUI_EXEC)
