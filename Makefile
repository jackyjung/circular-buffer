CC := gcc

OBJ_EXT := .o
SRC_EXTS := *.c

TARGET := circular-buffer

SRC_DIRS = .
INC_DIR = -I.

SRC_FILES += $(foreach d,$(SRC_DIRS),$(wildcard $(addprefix $(d)/,$(SRC_EXTS))))
OBJ_FILES += $(addprefix obj/,$(addsuffix $(OBJ_EXT),$(basename $(notdir $(SRC_FILES)))))

CC_FLAGS += -g $(INC_DIR) 
LD_FLAGS += -lpthread -lrt 

$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LD_FLAGS) $(CP_LIBS)

vpath %.c $(SRC_DIRS)
obj/%.o: %.c
	$(CC) $(CC_FLAGS) -c -o $@ $<

clean:
	rm -rf ./obj/* $(TARGET)
