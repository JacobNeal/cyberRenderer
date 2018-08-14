CXX = g++
CXXFLAGS = -I../include -Wall -std=gnu++14
LDFLAGS = -lglfw3 -lopengl32 -lgdi32
EXE = test.exe

SRCLOC = ../src
MAINLOC = ../src/main.cpp

SOURCES = $(MAINLOC)
SOURCES += $(wildcard $(SRCLOC)/*.c)
SOURCES += $(wildcard $(SRCLOC)/*.cpp)
SOURCES += $(wildcard $(SRCLOC)/Window/*.cpp)
SOURCES += $(wildcard $(SRCLOC)/Services/*.cpp)
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))

all: $(EXE)

main.o:$(MAINLOC)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(SRCLOC)/%.c
	gcc -c -o $@ $<

%.o:$(SRCLOC)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(SRCLOC)/Window/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(SRCLOC)/Services/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(EXE) $(OBJS)
