ifeq ($(CXX),icpc)
    OMPFLAG=-openmp
else
    OMPFLAG=-fopenmp
endif

CFLAGS = -c -O2 -Wall $(OMPFLAG)
CPPFLAGS = -DNDEBUG
LDFLAGS = $(OMPFLAG)

APP = ldmm
BUILDDIR = build

.PHONY: all release debug

all: release

release: $(APP)

debug: CFLAGS = -c -g -Wall $(OMPFLAG)
debug: CPPFLAGS = -UNDEBUG
debug: $(APP)

objects=\
$(BUILDDIR)/configreader.o\
$(BUILDDIR)/cubepbc.o\
$(BUILDDIR)/groconfigreader.o\
$(BUILDDIR)/groconfigwriter.o\
$(BUILDDIR)/lj12_6.o\
$(BUILDDIR)/main.o\
$(BUILDDIR)/mdconfigreader.o\
$(BUILDDIR)/mdsystem.o\
$(BUILDDIR)/util.o\
$(BUILDDIR)/vverlet.o

$(APP): $(objects)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(objects): | $(BUILDDIR)

$(BUILDDIR)/%.o: src/%.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) $(APP) $(BUILDDIR)/*
