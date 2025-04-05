BRANCH := $(shell git rev-parse --abbrev-ref HEAD)

ifeq ($(BRANCH),main)
VERSION := $(shell git log -n 1 --format="%cd.%h" --date=format-local:'%Y%m%d')
else ifeq ($(BRANCH),stable)
VERSION := $(shell cat VERSION)
else
VERSION := unknown
endif

CFLAGS = `pkg-config --cflags gtk+-3.0` -Dpver=\"$(VERSION)\"
CC = cc
LIBS = `pkg-config --libs gtk+-3.0`
SRC = settings.c exec.c quickaccess.c entryloader.c events.c keyhandler.c cfgmgr.c calc.c mainwindow.c sglauncher.c 
OBJ = $(SRC:.c=.o)
EXE = sglauncher

LOCALEDIR = po
LOCALE_INSTALL_DIR = $(PREFIX)/share/locale
LANGUAGES = es
DOMAIN = $(EXE)

PREFIX = /usr
BIN_DIR = $(PREFIX)/bin
APP_DIR = $(PREFIX)/share/applications

all: $(EXE) compile-locales

sglauncher: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -lm

compile-locales:
	xgettext --language=C --keyword=_ --output=po/messages.pot *.c *.h
	@for lang in $(LANGUAGES); do \
		mkdir -p $(LOCALEDIR)/$$lang/LC_MESSAGES; \
		echo "Updating .po file for locale: $$lang"; \
		msgmerge --update $(LOCALEDIR)/$$lang/$(DOMAIN).po po/messages.pot; \
		echo "Compiling locale: $$lang"; \
		msgfmt $(LOCALEDIR)/$$lang/$(DOMAIN).po --output-file=$(LOCALEDIR)/$$lang/LC_MESSAGES/$(DOMAIN).mo --verbose; \
	done

debug: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -lm -g

test:
	./$(EXE)

install:
	@echo installing SGLauncher $(VERSION)
	install -Dvm755 $(EXE) $(BIN_DIR)/$(EXE)
	install -Dvm644 $(EXE)-cfg.desktop $(APP_DIR)/$(EXE)-cfg.desktop
	install -Dvm644 icons/$(EXE).svg $(PREFIX)/share/icons/hicolor/48x48/apps/$(EXE).svg
	@for lang in $(LANGUAGES); do \
		if [ -f $(LOCALEDIR)/$$lang/LC_MESSAGES/$(DOMAIN).mo ]; then \
			install -Dvm644 $(LOCALEDIR)/$$lang/LC_MESSAGES/$(DOMAIN).mo $(LOCALE_INSTALL_DIR)/$$lang/LC_MESSAGES/$(DOMAIN).mo; \
		fi \
	done

uninstall:
	rm -v $(BIN_DIR)/$(EXE)
	rm -v $(APP_DIR)/$(EXE)-cfg.desktop
	rm -v $(PREFIX)/share/icons/hicolor/64x64/apps/sglauncher.svg
	@for lang in $(LANGUAGES); do \
		rm -f $(LOCALE_INSTALL_DIR)/$$lang/LC_MESSAGES/$(DOMAIN).mo; \
	done

clean:
	rm -f $(OBJ) $(EXE) debug
