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

ASFLAGS := -EB -march=vr4300 -mtune=vr4300 -mabi=32 -Iinclude
LDFLAGS := -EB -T $(LD_SCRIPT) -T undefined_syms_auto.txt -T undefined_funcs_auto.txt \
           -Map $(BUILD)/$(BASENAME).map --no-check-sections

OBJECTS = $(shell grep -oE 'build/[^ ]+\.o' $(LD_SCRIPT) 2>/dev/null | sort -u)

.PHONY: all split extract verify clean

all: verify

split:
	$(PYTHON) -m splat split $(YAML)

extract:
	$(PYTHON) tools/extract_files.py

verify: $(ROM)
	@sha1=$$(shasum $(ROM) | cut -d' ' -f1); \
	if [ "$$sha1" = "$(ROM_SHA1)" ]; then echo "$(ROM): OK"; \
	else echo "$(ROM): NO coincide ($$sha1)"; exit 1; fi

$(ROM): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(ELF): $(OBJECTS) $(LD_SCRIPT)
	$(LD) $(LDFLAGS) -o $@

$(BUILD)/asm/%.o: asm/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD)/assets/%.o: assets/%.bin
	@mkdir -p $(dir $@)
	$(OBJCOPY) -I binary -O elf32-tradbigmips -B mips $< $@

clean:
	rm -rf $(BUILD)
