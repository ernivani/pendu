SRC = src
INCLUDES = -Iinclude
DLLS = 
OUT_DIR = out

# Output files
OUT = $(OUT_DIR)/main

# Compilers
CC = g++

# Determine the OS
ifeq ($(OS),Windows_NT)
    RM = del /Q
    RMDIR = rmdir /S /Q
    COPY = copy
    XCOPY = xcopy
else
    RM = rm -f
    RMDIR = rm -rf
    COPY = cp
    XCOPY = cp -r
endif


SRCS = $(wildcard $(SRC)/*.cpp)


all: $(OUT) run

$(OUT): $(SRCS)
	$(CC) $(SRCS) $(INCLUDES) -o $(OUT)

copy-dlls:
	$(foreach DLL, $(DLLS), $(COPY) $(DLL) $(OUT_DIR) &)
	$(XCOPY) res $(OUT_DIR)\res /E /I /Y

run:
	$(OUT_DIR)/main

clean:
	-$(RM) $(OUT_DIR)\*.exe
	-$(RM) $(OUT_DIR)\*.dll
	-$(RM) $(OUT_DIR)\*.js $(OUT_DIR)\*.wasm $(OUT_DIR)\*.data $(OUT_DIR)\*.html
	-$(RMDIR) $(OUT_DIR)\res
	@echo Cleaned
