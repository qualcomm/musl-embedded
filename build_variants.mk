#Make sure "all" is the default target.
all: $(ALL_LIBS) $(ALL_TOOLS)

# Generate rules to build variants for libc-{variant}.a
define build_static_variant
ifeq ($$(QUIC_VARIANT_$(1)_CFLAGS),)
$$(error CFLAGS (QUIC_VARIANT_$(1)_CFLAGS) for variant `$(1)` not set)
endif

lib_$(1) := lib/libc-$(1).a
STATIC_LIBS := $(STATIC_LIBS) $$(lib_$(1))

AOBJS_$(1) = $$(subst obj/,obj_$(1)/,$$(AOBJS))

OBJ_DIRS_$(1) = $$(sort $$(patsubst %/,%,$$(dir $$(AOBJS_$(1)))))

$$(OBJ_DIRS_$(1)):
	mkdir -p $$(@)

obj_$(1)/%.o: $(srcdir)/%.s
	$$(AS_CMD)

obj_$(1)/%.o: $(srcdir)/%.S
	$$(CC_CMD)

obj_$(1)/%.o: $(srcdir)/%.c $(GENH) $(IMPH) $(GENH_INT)
	$$(CC_CMD)

obj_$(1)/%.o: CFLAGS := $$(QUIC_VARIANT_$(1)_CFLAGS)

lib/libc-$(1).a: $$(OBJ_DIRS_$(1)) $$(AOBJS_$(1))
	rm -f $$(@)
	$$(AR) rc $$(@) $$(AOBJS_$(1))
	$$(RANLIB) $$(@)


clean_$(1):
	rm -fr obj_$(1)

clean: clean_$(1)

.PHONY: clean_$(1)

endef

ifneq ($(STATIC_LIBS),)
  $(foreach v,$(QUIC_VARIANTS),$(eval $(call build_static_variant,$(v))))
endif

# Generate rules to build variants for libc-{variant}.so
define build_shared_variant
ifeq ($$(QUIC_VARIANT_$(1)_CFLAGS),)
$$(error CFLAGS (QUIC_VARIANT_$(1)_CFLAGS) for variant `$(1)` not set)
endif

SHARED_LIBS := $(SHARED_LIBS) lib/libc-$(1).so

LOBJS_$(1) = $$(subst obj/,obj_$(1)/,$$(LOBJS) $$(LDSO_OBJS))
LDSO_OBJS_$(1) = $$(subst obj/,obj_$(1)/, $$(LDSO_OBJS))
LDSO_DIRS_$(1) = $$(sort $$(patsubst %/,%,$$(dir $$(LDSO_OBJS_$(1)))))

obj_$(1)/%.lo: $(srcdir)/%.s
	$$(AS_CMD)

obj_$(1)/%.lo: $(srcdir)/%.S
	$$(CC_CMD)

obj_$(1)/%.lo: $(srcdir)/%.c $(GENH) $(IMPH) $(GENH_INT)
	$$(CC_CMD)
obj_$(1)/%.lo: CFLAGS := $$(QUIC_VARIANT_$(1)_CFLAGS)

lib/libc-$(1).so: $$(LDSO_DIRS_$(1)) $$(LOBJS_$(1))
	$$(CC) $$(CFLAGS_ALL) $$(LDFLAGS_ALL) -nostdlib -shared \
	-Wl,-e,_dlstart -o $$(@) $$(LOBJS_$(1)) $$(LIBCC)

endef

ifneq ($(SHARED_LIBS),)
  $(foreach v,$(QUIC_VARIANTS),$(eval $(call build_shared_variant,$(v))))
endif
