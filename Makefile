## Automation of building executable using Nmake with the MSVC compiler [v2.0] by Oscar Milstein.

## Path variables.
INCLUDES = Includes\\
BUILD = Build\\
BIN = Bin\\
DEBUG = Debug\\
EXECUTABLE = $(BIN)Archaeus.exe
SOURCE = Source\\

## Compile command variables.
CFLAGS =-c -permissive- -std:c17 -TC -MP -W4 -nologo -I$(INCLUDES) -I$(INCLUDES)cgltf -D"_CRT_SILENCE_NONCONFORMING_TGMATH_H" -D"COBJMACROS" -D"WIN32_LEAN_AND_MEAN" -Fd$(DEBUG)
LFLAGS =-nologo -SUBSYSTEM:WINDOWS $(LIBS)
LIBS =User32.lib d3d11.lib dxgi.lib d3dcompiler.lib dxguid.lib Winmm.lib Gdi32.lib

debug: $(EXECUTABLE)

$(EXECUTABLE): $(BUILD)*.obj
   LINK $(LFLAGS) -DEBUG:FASTLINK $** -OUT:$@

{$(SOURCE)}.c{$(BUILD)}.obj::
   $(CC) $(CFLAGS) -Zi -MTd -Od -FC -D"DEBUG" $< -Fo:$(BUILD)

## Builds an optimized release build of the project.
release:
   $(CC) $(CFLAGS) -O2 -fp:fast -D"NDEBUG" $(SOURCE)*.c -Fo:$(BUILD)
   LINK $(LFLAGS) $(BUILD)*.obj -OUT:$(EXECUTABLE)
   @echo Build complete!

## Cleans the project folder.
clean:
   -del $(BIN)*.ilk $(BIN)*.pdb $(BIN)*.exe $(BUILD)*.obj $(DEBUG)*.pdb
   @echo Clean complete!