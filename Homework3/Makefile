# Get the platform name
UNAME = $(shell uname)

# C++ compiler
CXX = g++

# General flags
CXXFLAGS = -std=c++11 
LDFLAGS = -lm -lpthread -ldl -ldrm -ljpeg
GLFLAGS = -lGL -lglfw  -lGLEW

# Add X flags for X11 systems
ifeq ($(UNAME), Linux)
	LDFLAGS += -lXdamage -lX11-xcb -lxcb-glx -xcb-dri2 -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lxcb -lXau -lXdmcp
endif

# Add Darwin-specific linking flags
ifeq ($(UNAME), Darwin)
	GLFLAGS += -framework OpenGL
endif

SOURCES = $(wildcard *.cpp)

.PHONY: all clean

all:
	$(CXX) $(CXXFLAGS) -o hw3 $(SOURCES) -I./glm-0.9.9.3/ \
		$(LDFLAGS) \
		$(GLFLAGS)

clean:
	@rm -f hw3 *.o
