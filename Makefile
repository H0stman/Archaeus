## Automation of building executable using Nmake with the MSVC compiler [v1.0] by Oscar Milstein.

## Path variables
INCLUDES = Includes\\
BUILD = Build\\
BIN = Bin\\
DEBUG = Debug\\
EXECUTABLE = $(BIN)Archaeus.exe
SOURCE = Source\\

## Compile command variables
CFLAGS =-c -permissive- -std:c17 -TC -W4 -nologo -IIncludes -D"COBJMACROS" -D"WIN32_LEAN_AND_MEAN" -Fd$(DEBUG)
LFLAGS =-nologo -SUBSYSTEM:WINDOWS -INCREMENTAL:NO $(LIBS)
LIBS =User32.lib d3d11.lib dxgi.lib d3dcompiler.lib dxguid.lib Winmm.lib

all: $(EXECUTABLE)

$(EXECUTABLE): $(BUILD)*.obj
!IF DEFINED(debug)
   LINK $(LFLAGS) -DEBUG:FASTLINK $** -OUT:$@
!ELSE
   LINK $(LFLAGS) $** -OUT:$@
!ENDIF
   @echo Build complete!

{Source\}.c{$(BUILD)}.obj::
!IF DEFINED(debug)
   $(CC) $(CFLAGS) -Zi -MTd -Od -FC -D"DEBUG" $< -Fo:$(BUILD)
!ELSE
   $(CC) $(CFLAGS) -O2 -D"NDEBUG" $< -Fo:$(BUILD)
!ENDIF

clean:
   -del $(BIN)*.pdb $(BIN)*.exe $(BUILD)*.obj $(DEBUG)*.pdb
   @echo Clean complete!