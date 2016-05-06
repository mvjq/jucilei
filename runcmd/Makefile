#    Makefile, Copyright 2016 Danilo Tedeschi <danfyty@gmail.com>
#    Makefile is derived from posixeg, Copyright 2014  Monaco F. J.   <monaco@icmc.usp.br>
#
#    This file is part of Jucilei
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with .  If not, see <http://www.gnu.org/licenses/>. 


#TODO:       ####
#   make distcheck
#   make check

include config.mk

lib_static = $(lib:%=%.a)
lib_shared = $(lib:%=%.so)

lib_all = $(lib_static) $(lib_shared)

inst_headers = $(foreach i, $(lib:%=%_h), $($i))

proj_src += $(foreach i, $(lib:%=%_obj), $($i:%o=%c))
proj_src += $(foreach i, $(bin:%=%_obj), $($i:%o=%c))

proj_headers += $(filter %.h, $(shell $(CC) $(CPP_FLAGS) $(CPPFLAGS) -I. -MM -MG -MT " " $(proj_src)) )
proj_headers += $(inst_headers)
proj_dep = $(proj_src:%.c=%.d)

all: $(lib_all) $(bin)

%.d : %.c
	$(CC) $(CPP_FLAGS) $(CPPFLAGS) -I. -MM -MG -MT $(C_FLAGS) $(CFLAGS) '$(<:.c=.o) $(<:.c=-pic.o) $@' $< > $@


#Didnt have time to make it a pattern
test.o: test.c debug.h
	$(CC) -c $< $(LDFLAGS) $(LD_FLAGS) $(CPPFLAGS) $(CPP_FLAGS) -I. $(CFLAGS) $(C_FLAGS) -o $@
test: test.o runcmd.o
	$(CC) $^ -o $@
#just to quickly test runcmd


.SECONDEXPANSION:
$(lib_static): %.a : $$($$*_obj)
	$(AR) rcs $@ $^

$(lib_shared) : %.so : $$($$*_obj:.o=-pic.o)
	$(CC) -shared $(LD_FLAGS) $(LDFLAGS) -L. $^ -o $@

%.o: %.c
	$(CC) -c $< $(LD_FLAGS) $(LDFLAGS) $(CPP_FLAGS) $(CPPFLAGS) -I. $(C_FLAGS) $(CFLAGS) -o $@

%-pic.o: %.c
	$(CC) -c $< $(LD_FLAGS) $(LDFLAGS) $(CPP_FLAGS) $(CPPFLAGS) -I. $(C_FLAGS) $(CFLAGS) -fPIC -o $@

-include $(proj_dep)

.PHONY: clean install dist uninstall

install: $(bin) $(lib_all) $(inst_headers)
	install -d $(PREFIX)/bin
	install -d $(PREFIX)/lib
	install -d $(PREFIX)/include
	cp $(lib_all) $(PREFIX)/lib
	cp $(inst_headers) $(PREFIX)/include
	for i in $(bin); do if test -f $$i; then cp $$i $(PREFIX)/bin; fi done

uninstall:
	for i in $(lib_all); do rm -rf $(PREFIX)/lib/$$i; done
	for i in $(bin); do rm -rf $(PREFIX)/bin/$$i; done

dist: $(PROJECT).tgz

$(PROJECT).tgz: $(proj_src) $(proj_headers)
	rm -rf $(PROJECT)
	mkdir $(PROJECT)
	cp $(proj_src) $(proj_headers) $(EXTRA_DIST) $(PROJECT)/
	$(TAR) czpf $(PROJECT).tgz $(PROJECT)
	rm -rf $(PROJECT)

clean:
	rm -f *.d *.o *.a *.so $(bin)
	rm -f $(PROJECT).tgz
	rm -f test
