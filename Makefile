## Automation of building executable using Nmake with the MSVC compiler [v1.1] by Oscar Milstein.

## Path variables.
INCLUDES = Includes\\
BUILD = Build\\
BIN = Bin\\
DEBUG = Debug\\
EXECUTABLE = $(BIN)Archaeus.exe
SOURCE = Source\\

## Compile command variables.
CFLAGS =-c -permissive- -std:c17 -TC -W4 -nologo -I$(INCLUDES) -I$(INCLUDES)cgltf -D"COBJMACROS" -D"WIN32_LEAN_AND_MEAN" -Fd$(DEBUG)
LFLAGS =-nologo -SUBSYSTEM:WINDOWS $(LIBS)
LIBS =User32.lib d3d11.lib dxgi.lib d3dcompiler.lib dxguid.lib Winmm.lib

all: $(EXECUTABLE)

$(EXECUTABLE): $(BUILD)*.obj
   LINK $(LFLAGS) -DEBUG:FASTLINK $** -OUT:$@

{Source\}.c{$(BUILD)}.obj::
   $(CC) $(CFLAGS) -Zi -MTd -Od -FC -D"DEBUG" $< -Fo:$(BUILD)

## Builds a debug build of the entire project in parallel using max avaliable cores on the system.
pard:
   $(CC) $(CFLAGS) -MP -Zi -MTd -Od -FC -D"DEBUG" $(SOURCE)*.c -Fo:$(BUILD)
   LINK $(LFLAGS) -DEBUG:FASTLINK $(BUILD)*.obj -OUT:$(EXECUTABLE)
   @echo Build complete!

## Builds an optimized release build of the entire project in parallel using max avaliable cores on the system.
parr:
   $(CC) $(CFLAGS) -MP -O2 -D"NDEBUG" $(SOURCE)*.c -Fo:$(BUILD)
   LINK $(LFLAGS) $(BUILD)*.obj -OUT:$(EXECUTABLE)
   @echo Build complete!

## Cleans the project folder.
clean:
   -del $(BIN)*.ilk $(BIN)*.pdb $(BIN)*.exe $(BUILD)*.obj $(DEBUG)*.pdb
   @echo Clean complete!