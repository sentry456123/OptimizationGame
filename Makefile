all: execute

CXX := g++
CXXFLAGS := -Wall
LINK_FLAGS := -m64 -mwindows # 64bit SYSTEM:WINDOWS
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
EXTERN_DIR := extern
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
INCLUDE := -I$(EXTERN_DIR)/SDL2/include -I$(EXTERN_DIR)/SDL2_ttf/include -I$(EXTERN_DIR)/SDL2_image/include -I$(EXTERN_DIR)/SDL2_mixer/include
LIB_AND_LINKER := -L$(EXTERN_DIR)/SDL2/lib/x64 -L$(EXTERN_DIR)/SDL2_ttf/lib/x64 -L$(EXTERN_DIR)/SDL2_image/lib/x64 -L$(EXTERN_DIR)/SDL2_mixer/lib/x64 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -static
EXE_FILE_NAME := Optimization

debug: CXXFLAGS += -DDEBUG -g
debug: LINK_FLAGS += -mconsole
debug: execute

execute: $(BIN_DIR)/$(EXE_FILE_NAME)

$(BIN_DIR)/$(EXE_FILE_NAME): $(OBJ_FILES) $(OBJ_DIR)/resource.res
	$(CXX) $(CXXFLAGS) $(LINK_FLAGS) -o $@ $^ $(LIB_AND_LINKER) $(OBJ_DIR)/resource.res

compile: $(OBJ_FILES) $(OBJ_DIR)/resource.res

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(wildcard src/*.h)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCLUDE)

$(OBJ_DIR)/resource.res:
	windres res/resource.rc -O coff -o $@

exe:
	cd bin && $(EXE_FILE_NAME)

clean:
	-rm $(OBJ_FILES) $(BIN_DIR)/$(EXE_FILE_NAME) res/resource.res
