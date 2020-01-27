SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/easyvr
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

C_LIBS   := dbus-1 glib-2.0 dbus-glib-1 libconfig

CFLAGS	 := -Wall -Iinclude $(shell pkg-config --cflags $(C_LIBS))  
LDFLAGS  := -Llib 
LDLIBS   := -lm $(shell pkg-config --libs $(C_LIBS))  


.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $@

$(BIN_DIR):
	mkdir $@

clean:
	$(RM) $(OBJ) $(EXE)	
