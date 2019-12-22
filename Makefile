# ####################################################### #
TARGET     = mit
# ####################################################### #
MAKE       = remake
RM         = rm

GDB        = gdb-multiarch
CC         = arm-none-eabi-gcc
CPP        = arm-none-eabi-g++
OBJCOPY    = arm-none-eabi-objcopy
OBJDUMP    = arm-none-eabi-objdump
SIZE       = arm-none-eabi-size

BURNER     = openocd

# ####################################################### #
DEVICE     = PART_TM4C1231H6PZ
BINDIR    ?= build
INCDIR    ?= inc
LIBDIR    ?= libs
SRCDIR    ?= src
CSRC       = $(wildcard $(SRCDIR)/*.c $(SRCDIR)/*/*.c)
#CPPSRC     = $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/*/*.cpp)
ASRC       = $(wildcard $(SRCDIR)/*/*.S $(SRCDIR)/*/*.asm)
OBJ        = $(CSRC:.c=.o) $(CPPSRC:.cpp=.o) $(ASRC:.S=.o)

# ####################################################### #
TIVADIR    = libs/tivaware
#LIBLIST    =  -lm -lnosys -lc -lc_nano  -ldriver
LIBLIST    = -lgcc -lm -ldriver

CINC       = -I$(TIVADIR)/driverlib  -I$(TIVADIR)/inc
CINC      += -I$(INCDIR)  -I$(LIBDIR)  -I$(SRCDIR)
CINC      += -I./newlib-1.18.0/newlib/libc/include

# ####################################################### #
CFLAGS     = -fdata-sections -ffunction-sections -g2 -Os -w
CFLAGS    += -fno-common -fverbose-asm -fmax-errors=100
CFLAGS    += -W -Wall -Wextra -Wshadow -Wcast-align
CFLAGS    += -Werror=implicit-function-declaration
CFLAGS    += -mcpu=cortex-m4 -mlittle-endian -mthumb
CFLAGS    += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS    += $(CINC)
CFLAGS    += -D $(DEVICE)

ASMFLAGS   = $(CFLAGS)

DEBUGFLAGS = -f openocd.cfg -c "adapter_khz 2000"
BURNFLAGS  = -c "program $(BINDIR)/$(TARGET).elf verify reset" -c shutdown

LDSCRIPT   = project.ld
LDFLAGS    = -T $(LDSCRIPT) -L$(TIVADIR)/driverlib/gcc $(LIBLIST)
LDFLAGS   += --static -nostartfiles -Wl,--gc-sections
LDFLAGS   += --specs=nano.specs --specs=nosys.specs

# ####################################################### #
.PHONY: all burn clean list  init debug

clean:
	$(RM) -f $(OBJ) $(SRCDIR)/*.d $(SRCDIR)/*/*.d $(BINDIR)/$(TARGET).*

burn: $(TARGET).elf
	$(BURNER) $(DEBUGFLAGS) $(BURNFLAGS)

list: $(TARGET).elf
	$(OBJDUMP) -DSlf $(BINDIR)/$(TARGET).elf > $(BINDIR)/$(TARGET).asm

all: $(TARGET).elf
	$(SIZE) $(BINDIR)/$(TARGET).elf | tee $(BINDIR)/$(TARGET).size

$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BINDIR)/$@ $(LDFLAGS)
#	$(CPP) $(CFLAGS) $(OBJ) -o $(BINDIR)/$@ $(LDFLAGS)

init: $(TARGET).elf
	$(BURNER) $(DEBUGFLAGS)

debug:
	$(GDB) -ex "target remote localhost:3333" $(BINDIR)/$(TARGET).elf

# ####################################################### #
#$(LIBOPENCM3):
#	$(MAKE) -C libopencm3 TARGETS=stm32/f1 $(MAKEFLAGS)

#%.o: %.c $(LIBOPENCM3)
#	$(CC) -MMD -c $(CFLAGS) $< -o $@

#%.o: %.s $(LIBOPENCM3)
#	$(CC) -MMD -c $(ASMFLAGS) $< -o $@

-include $(SRCDIR)/*.d
