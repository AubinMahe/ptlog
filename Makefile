.PHONY: all clean run archive

SRCS     := tests/ptlog.cpp src/ptlog.cpp
OBJS     := $(patsubst %.cpp,BUILD/%.o,$(SRCS))
CXXFLAGS := -I inc -pthread -W -Wall -pedantic -O3
LDFLAGS  := -pthread

all: BUILD/ptlog.x

clean:
	rm -fr BUILD

run: BUILD/ptlog.x
	BUILD/ptlog.x

archive:
	zip -r ../per_thread_log.zip *

BUILD/ptlog.x: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

BUILD/%.o: %.cpp
	@mkdir -p $$(dirname "$@")
	$(CXX) -c -o $@ $? $(CXXFLAGS)
