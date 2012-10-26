APP = ldmm
CC = icpc
CFLAGS = -c -g -Wall -openmp
CPPFLAGS += -I include
LDFLAGS = -static-intel -openmp

vpath %.cpp src
vpath %.o   obj

output_obj = ./obj

sources=$(notdir $(wildcard ./src/*.cpp))
objects=$(sources:.cpp=.o)


$(APP): $(objects)
	$(CC) $(LDFLAGS) $^ -o $@

define ldmd_rules=
$(1):$(2)
endef

$(eval $(foreach s, $(sources), $(call ldmd_rules, $(s:.cpp=.o),$(s) )))

%.o: %.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $(output_obj)/$@

.PHONY: clean
clean:
	$(RM) $(APP) $(output_obj)/*
