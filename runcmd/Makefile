include config.mk

lib_static = $(lib:%=%.a)
lib_shared = $(lib:%=%.so)
lib_all = $(lib_static) $(lib_shared)

#idk why the _h
inst_headers = $(foreach i, $(lib:%=%_h), $($i))

proj_src += $(foreach i, $(lib:%=%_obj), $($i:%o=%c))
proj_src += $(foreach i, $(bin:%=%_obj), $($i:%o=%c))
proj_dep = $(proj_src:%.c=%.d)

%.d : %.c
	$(CC) $(CPP_FLAGS) $(CPPFLAGS) -I. -MM -MG -MT $(C_FLAGS) $(CFLAGS) '$(<:.c=.o) $@' $< > $@

.SECONDEXPANSION:
$(lib_static): %.a : $$($$*_obj)
	$(AR) rcs $@ $^

$(lib_shared): %.so : $$($$*_obj:.o=-pic.o)
	$(CC) -shared $(LD_FLAGS) $(LDFLAGS) -L. $^ -o $@

%.o: %.c
	$(CC) -c $< $(LD_FLAGS) $(LDFLAGS) $(CPP_FLAGS) $(CPPFLAGS) -I. $(C_FLAGS) $(CFLAGS) -o $@

%-pic.o: %.c
	$(CC) -c $< $(LD_FLAGS) $(LDFLAGS) $(CPP_FLAGS) $(CPPFLAGS) -I. $(C_FLAGS) $(CFLAGS) -fPIC -o $@

-include $(proj_dep)

all: $(lib_all) $(bin)

.PHONY: clean

clean:
	rm *.o *.a *.so $(bin) $(lib)
