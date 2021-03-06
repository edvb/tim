# See LICENSE file for copyright and license details.

include config.mk

ifeq ($(DEBUG), 1)
CFLAGS  += -O0 -g -DDEBUG
LDFLAGS += -g
else
CFLAGS += -O3
endif

EXE = tim
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: options $(EXE)

options:
	@echo $(EXE) build options:
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"

.o:
	@echo $(LD) $@
	@$(LD) -o $@ $< $(LDFLAGS)

.c.o:
	@echo $(CC) $<
	@$(CC) -c -o $@ $< $(CFLAGS)

${OBJ}: config.h config.mk

config.h:
	@echo creating $@ from config.def.h
	@cp config.def.h $@

$(EXE): $(OBJ)
	@echo $(CC) -o $@
	@$(CC) -o $@ $(OBJ) $(LDFLAGS)

clean:
	@echo -n cleaning ...
	@rm -f $(OBJ) $(EXE)
	@echo \ done

install: all
	@echo -n installing executable file to $(DESTDIR)$(PREFIX)/bin ...
	@mkdir -p $(DESTDIR)$(PREFIX)/bin
	@cp -f $(EXE) $(DESTDIR)$(PREFIX)/bin
	@chmod 755 $(DESTDIR)$(PREFIX)/bin/$(EXE)
	@echo \ done
	@echo -n installing support files to $(DESTDIR)$(SHAREPREFIX)/$(EXE) ...
	@mkdir -p $(DESTDIR)$(SHAREPREFIX)/$(EXE)
	@cp -rf data plugins $(DESTDIR)$(SHAREPREFIX)/$(EXE)
	@echo \ done
	@echo -n installing manual page to $(DESTDIR)$(MANPREFIX)/man1 ...
	@mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	@sed "s/VERSION/$(VERSION)/g" < $(EXE).1 > $(DESTDIR)$(MANPREFIX)/man1/$(EXE).1
	@chmod 644 $(DESTDIR)$(MANPREFIX)/man1/$(EXE).1
	@echo \ done

uninstall:
	@echo -n removing executable file from $(DESTDIR)$(PREFIX)/bin ...
	@rm -f $(DESTDIR)$(PREFIX)/bin/$(EXE)
	@echo \ done
	@echo -n removing support files from $(DESTDIR)$(SHAREPREFIX)/$(EXE) ...
	@rm -rf $(DESTDIR)$(SHAREPREFIX)/$(EXE)
	@echo \ done
	@echo -n removing manual page from $(DESTDIR)$(MANPREFIX)/man1 ...
	@rm -f $(DESTDIR)$(MANPREFIX)/man1/$(EXE).1
	@echo \ done

update-man:
	@echo -n updating man page $(EXE).1 ...
	@cat README.md | sed "s/# $(EXE)/# $(EXE) 1\n\n##NAME\n\n$(EXE) /" | \
		md2man-roff | sed "s/\\[la\]/\</" | sed "s/\\[ra\]/\>/" > $(EXE).1
	@echo \ done

.PHONY: all options clean install uninstall update-man
