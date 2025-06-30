.PHONY: all clean run archive

SRCS     := tests/per-thread-log.cpp src/per-thread-log.cpp
OBJS     := $(patsubst %.cpp,BUILD/%.o,$(SRCS))
CXXFLAGS := -I inc -pthread -W -Wall -pedantic -O3
LDFLAGS  := -pthread

all: BUILD/per-thread-log.x

clean:
	rm -fr BUILD /tmp/per-thread-log.x

run: BUILD/per-thread-log.x
	BUILD/per-thread-log.x
	cat /tmp/per-thread-log.x/*.log | sort > BUILD/merged.log

archive:
	zip -r ../per-thread-log.zip *

BUILD/per-thread-log.x: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

BUILD/%.o: %.cpp
	@mkdir -p $$(dirname "$@")
	$(CXX) -c -o $@ $? $(CXXFLAGS)
