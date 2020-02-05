# ####################################################### #
TARGET     = mit
# ####################################################### #
MAKE      := remake
BURNER    := openocd
GDB       := gdb-multiarch

AS         = arm-none-eabi-gcc -x assembler-with-cpp
CC         = arm-none-eabi-gcc
COPY       = arm-none-eabi-copy
DUMP       = arm-none-eabi-objdump
SIZE       = arm-none-eabi-size

STACKUSAGE = cat $(find obj/ -iname *.su | tr '\n' ' ')

# ####################################################### #
DEVICE     = PART_TM4C1231H6PZ
BINDIR    := build
INCDIR    ?= inc
LIBDIR    ?= libs
OBJDIR    := obj
SRCDIR    ?= src

# ####################################################### #
ASRC       = $(shell find $(SRCDIR) -type f -name '*.asm')
CSRC       = $(shell find $(SRCDIR) -type f -name '*.c')
CPPSRC     = $(shell find $(SRCDIR) -type f -name '*.cpp')

OBJ        = $(addprefix $(OBJDIR)/, $(notdir $(CSRC:.c=.o)))
OBJ       += $(addprefix $(OBJDIR)/, $(notdir $(CPPSRC:.cpp=.o)))
OBJ       += $(addprefix $(OBJDIR)/, $(notdir $(ASRC:.asm=.o)))

vpath %.c    $(sort $(dir $(CSRC)))
vpath %.cpp  $(sort $(dir $(CPPSRC)))
vpath %.asm  $(sort $(dir $(ASRC)))

# ####################################################### #
TIVADIR    = libs/tivaware
LIBLIST    = -lgcc -lm -lc -ldriver

CINC       = -I$(TIVADIR)/driverlib  -I$(TIVADIR)/inc
CINC      += -I$(INCDIR)  -I$(LIBDIR)  -I$(SRCDIR)
CINC      += -I$(INCDIR)/*/*
CINC      += -I./newlib-1.18.0/newlib/libc/include

# ####################################################### #
#CFLAGS     = -funroll-loops -fstack-usage -Og -g -gdwarf-2
CFLAGS     = -funroll-loops -fstack-usage -Os
CFLAGS    += -fdata-sections -ffunction-sections -w
CFLAGS    += -fno-common -fverbose-asm -fmax-errors=16
CFLAGS    += -W -Wall -Wextra -Wshadow -Wcast-align
CFLAGS    += -Werror=implicit-function-declaration
CFLAGS    += -mcpu=cortex-m4 -mlittle-endian -mthumb
CFLAGS    += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS    += $(CINC)  -D $(DEVICE)

ASMFLAGS   = $(CFLAGS)

# ####################################################### #
LDSCRIPT   = project.ld
LDFLAGS    = -T $(LDSCRIPT) -L$(TIVADIR)/driverlib/gcc $(LIBLIST)
LDFLAGS   += --static -nostartfiles -Wl,--gc-sections
LDFLAGS   += --specs=nano.specs --specs=nosys.specs

DEBUGFLAGS = -f openocd.cfg -c "adapter_khz 2000"
BURNFLAGS  = -c "program $(BINDIR)/$(TARGET).elf verify reset" -c shutdown

# ####################################################### #
.PHONY: all burn clean list  init debug

clean:
	rm -f $(OBJDIR)/* $(BINDIR)/*

burn: $(TARGET).elf
	$(BURNER) $(DEBUGFLAGS) $(BURNFLAGS)

all: $(BINDIR)/$(TARGET).elf
	$(SIZE) $(BINDIR)/$(TARGET).elf
	$(value STACKUSAGE) > $(BINDIR)/$(TARGET).su

list: $(BINDIR)/$(TARGET).elf
	$(DUMP) -DSlf $< > $(BINDIR)/$(TARGET).list

# ####################################################### #
$(BINDIR):
	mkdir -p $@
$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%.o: %.asm $(OBJDIR)
	$(AS) $(ASMFLAGS) -c -o $@ $<
$(OBJDIR)/%.o: %.cpp $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<
$(OBJDIR)/%.o: %.c $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINDIR)/$(TARGET).elf: $(OBJ) $(BINDIR)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# ####################################################### #
init: $(TARGET).elf
	$(BURNER) $(DEBUGFLAGS)
debug:
	$(GDB) -ex "target remote localhost:3333" $(BINDIR)/$(TARGET).elf

-include $(SRCDIR)/*.d
