## Automation of building executable using Nmake with the MSVC compiler [v2.0] by Oscar Milstein.

## Path variables.
INCLUDES = Includes\\
BUILD = Build\\
BIN = Bin\\
DEBUG = Debug\\
EXECUTABLE = $(BIN)Archaeus.exe
SOURCE = Source\\

## Compile command variables.
CFLAGS =-c -permissive- -std:c17 -TC -MP -W4 -nologo -I$(INCLUDES) -I$(INCLUDES)import -D"_CRT_SILENCE_NONCONFORMING_TGMATH_H" -D"COBJMACROS" -D"WIN32_LEAN_AND_MEAN" -Fd$(DEBUG)
LFLAGS =-nologo -SUBSYSTEM:WINDOWS $(LIBS)
LIBS =User32.lib d3d11.lib dxgi.lib d3dcompiler.lib dxguid.lib Winmm.lib

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

init:
   if not exist $(MAKEDIR)\Build mkdir $(MAKEDIR)\Build
   if not exist $(MAKEDIR)\Debug mkdir $(MAKEDIR)\Debug
   if not exist $(MAKEDIR)\Bin mkdir $(MAKEDIR)\Bin
   if not exist $(MAKEDIR)\Resources mkdir $(MAKEDIR)\Resources