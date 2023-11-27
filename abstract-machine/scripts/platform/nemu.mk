AM_SRCS := platform/nemu/trm.c \
           platform/nemu/ioe/ioe.c \
           platform/nemu/ioe/timer.c \
           platform/nemu/ioe/input.c \
           platform/nemu/ioe/gpu.c \
           platform/nemu/ioe/light.c

CFLAGS    += -fdata-sections -ffunction-sections
LDFLAGS   += -T $(DD_HOME)/scripts/linker.ld \
             --defsym=_pmem_start=0x00000000 --defsym=_entry_offset=0x0
LDFLAGS   += --gc-sections -e _start
NEMUFLAGS += -l $(shell dirname $(IMAGE).elf)/nemu-log.txt -b #added -b to start with batch mode
NEMUFLAGS += -e $(IMAGE).elf
COEFLAGS  += -t $(shell dirname $(IMAGE).elf)/$(NAME)-0.coe
COEFLAGS  += -y $(shell dirname $(IMAGE).elf)/$(NAME)-1.coe
COEFLAGS  += -u $(shell dirname $(IMAGE).elf)/$(NAME)-2.coe
COEFLAGS  += -i $(shell dirname $(IMAGE).elf)/$(NAME)-3.coe

CFLAGS += -DMAINARGS=\"$(mainargs)\"
CFLAGS += -I$(DD_HOME)/am/src/platform/nemu/include
.PHONY: $(DD_HOME)/am/src/platform/nemu/trm.c

image: $(IMAGE).elf
	@$(OBJDUMP) -d $(IMAGE).elf > $(IMAGE).txt
	@echo + OBJCOPY "->" $(IMAGE_REL).bin
	@$(OBJCOPY) -S --set-section-flags .bss=alloc,contents -O binary $(IMAGE).elf $(IMAGE).bin
	@$(DD_HOME)/tools/gencoe $(COEFLAGS) $(IMAGE).bin
	

run: image
	$(MAKE) -C $(NEMU_HOME) ISA=$(ISA) run ARGS="$(NEMUFLAGS)" IMG=$(IMAGE).bin
    
gdb: image
	$(MAKE) -C $(NEMU_HOME) ISA=$(ISA) gdb ARGS="$(NEMUFLAGS)" IMG=$(IMAGE).bin
