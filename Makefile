BASENAME := hotwheelsturbo
YAML     := $(BASENAME).yaml
LD_SCRIPT := $(BASENAME).ld
BUILD    := build
ELF      := $(BUILD)/$(BASENAME).elf
ROM      := $(BUILD)/$(BASENAME).z64
ROM_SHA1 := 94913a07e49005ffd43dfdfa16ff5862bdb93748

CROSS   := mips-linux-gnu-
AS      := $(CROSS)as
LD      := $(CROSS)ld
OBJCOPY := $(CROSS)objcopy
PYTHON  ?= python3

KMC_DIR := tools/gcc_2.7.2
KMC_CC  := $(KMC_DIR)/gcc

# Opciones por archivo: el juego mezcla -O2 y -O0
OPTFLAGS := -O2
CFLAGS   := -G0 -mips3 -mgp32 -mfp32 -Iinclude

$(BUILD)/src/lz.o: OPTFLAGS := -O0
$(BUILD)/src/unit_6E0C0.o: OPTFLAGS := -O0
# __os{Pi,Si}CreateAccessQueue van insertadas en línea en __os{Pi,Si}GetAccess: estos archivos de libultra usan -O3.
$(BUILD)/src/unit_792B0.o: OPTFLAGS := -O3
$(BUILD)/src/unit_79630.o: OPTFLAGS := -O3
$(BUILD)/src/unit_7AA80.o: OPTFLAGS := -O3
$(BUILD)/src/unit_81B50.o: OPTFLAGS := -O3
$(BUILD)/src/unit_82AB0.o: OPTFLAGS := -O3
$(BUILD)/src/unit_81170.o: OPTFLAGS := -O3
$(BUILD)/src/unit_7D3B0.o: OPTFLAGS := -O3
$(BUILD)/src/unit_7D130.o: OPTFLAGS := -O3
$(BUILD)/src/unit_7DE70.o: OPTFLAGS := -O3
$(BUILD)/src/unit_7E0F0.o: OPTFLAGS := -O3
$(BUILD)/src/unit_7C710.o: OPTFLAGS := -O3
$(BUILD)/src/unit_7B750.o: OPTFLAGS := -O3
$(BUILD)/src/unit_7A970.o: OPTFLAGS := -O3
$(BUILD)/src/unit_7FD20.o: OPTFLAGS := -O3
# osSpTaskLoad lleva insertado _VirtualToPhysicalTask (estático): -O3.
$(BUILD)/src/unit_7EFB0.o: OPTFLAGS := -O3

ASFLAGS := -EB -march=vr4300 -mtune=vr4300 -mabi=32 -Iinclude
LDFLAGS := -EB -T $(LD_SCRIPT) -T undefined_syms_auto.txt -T undefined_funcs_auto.txt \
           -Map $(BUILD)/$(BASENAME).map --no-check-sections

OBJECTS = $(shell grep -oE 'build/[^ ]+\.o' $(LD_SCRIPT) 2>/dev/null | sort -u)

ifeq ($(shell uname -s),Darwin)
    TOOLCHAIN_OS := mac
else
    TOOLCHAIN_OS := linux
endif
GCC_URL      := https://github.com/decompals/mips-gcc-2.7.2/releases/download/v0.1/gcc-2.7.2-$(TOOLCHAIN_OS).tar.gz
BINUTILS_URL := https://github.com/decompals/mips-binutils-2.6/releases/download/v0.3/binutils-2.6-$(TOOLCHAIN_OS).tar.gz

.PHONY: all split extract verify progress report clean toolchain

all: verify

toolchain:
	mkdir -p $(KMC_DIR)
	curl -fsSL $(GCC_URL) | tar -xz -C $(KMC_DIR)
	curl -fsSL $(BINUTILS_URL) | tar -xz -C $(KMC_DIR)

split:
	$(PYTHON) -m splat split $(YAML)
	$(PYTHON) tools/fix_jump_labels.py

extract:
	$(PYTHON) tools/extract_files.py

progress: $(ELF)
	$(PYTHON) tools/progress.py --readme

# objdiff report for ByteMatch; commit report/us/report.json afterwards.
report: verify
	$(PYTHON) tools/objdiff_report.py

verify: $(ROM)
	@sha1=$$(shasum $(ROM) | cut -d' ' -f1); \
	if [ "$$sha1" = "$(ROM_SHA1)" ]; then echo "$(ROM): OK"; \
	else echo "$(ROM): NO coincide ($$sha1)"; exit 1; fi

$(ROM): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(ELF): $(OBJECTS) $(LD_SCRIPT)
	$(LD) $(LDFLAGS) -o $@

$(BUILD)/src/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(PYTHON) tools/cc.py $(OPTFLAGS) $< $@

$(BUILD)/asm/%.o: asm/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD)/assets/%.o: assets/%.bin
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary -O elf32-tradbigmips -B mips $< $@

clean:
	rm -rf $(BUILD)
