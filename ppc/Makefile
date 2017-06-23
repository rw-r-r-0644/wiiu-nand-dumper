.SUFFIXES:

SOURCES		:=	.
BUILD		:=	build
TARGET 		:= 	ppc-stuff

CFLAGS		:=
LDFLAGS		:=	-n
ASFLAGS		:=

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
					$(CFILES:.c=.o) $(SFILES:.S=.o)

all: $(TARGET).elf

$(TARGET).elf: link.ld $(OFILES)
	powerpc-eabi-ld -T $^ $(LDFLAGS) -o $@

%.o: %.c
	powerpc-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.S
	powerpc-eabi-gcc $(ASFLAGS) -c $< -o $@
