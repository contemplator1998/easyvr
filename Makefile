EXE := easyvr

SRC_DIR := src
OBJ_DIR := obj

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

#CPPFLAGS := -Iinclude -I/usr/include/dbus-1.0 -I/usr/lib/arm-linux-gnueabihf/dbus-1.0/include -I/usr/include/glib-2.0 -I/usr/lib/arm-linux-gnueabihf/glib-2.0/include 
#CFLAGS	 := -Wall 
#LDFLAGS  := -Llib 
#LDLIBS   := -lm -ldbus-glib-1 -ldbus-1 -lgobject-2.0 -lglib-2.0

CFLAGS	 := -Wall -Iinclude $(shell pkg-config --cflags dbus-1 glib-2.0 dbus-glib-1)  
LDFLAGS  := -Llib 
LDLIBS   := -lm $(shell pkg-config --libs dbus-1 glib-2.0 dbus-glib-1)  


.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $@

clean:
	$(RM) $(OBJ) $(EXE)	
