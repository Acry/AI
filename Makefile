define colorecho
      @tput setaf 2
      @echo $1
      @tput sgr0
endef

CFLAGS   = -Wall -Wextra -mtune=native -no-pie `sdl2-config --cflags`
LDFLAGS  = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx -lm

TARGETS = random_motion tracking

.PHONY: all
all: $(TARGETS)

dummy: dummy.c 
	@cc $(CFLAGS) -o $@ $< $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

random_motion:   helper.c  random_motion.c
	@cc $(CFLAGS) -o $@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

tracking:   helper.c  tracking.c
	@cc $(CFLAGS) -o $@ $+ $(LDFLAGS)
	$(call colorecho,"$@ success. ./\"$@\" to execute.")

.PHONY: clean
clean:
	@rm $(TARGETS) 2>/dev/null || true
	$(call colorecho,"It is clean now.")

# hot compile
# while inotifywait -e close_write ./code.c; do date +%X; make > /dev/null; done

#$<	first dependency
#$@	target name
#$+	list of all dependencies
#$^	list of all unique dependencies
