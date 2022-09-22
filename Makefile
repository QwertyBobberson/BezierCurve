CFLAGS=-std=c++17 -O3
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

Vulkan: *.cpp
	g++ $(CFLAGS) -o Vulkan main.cpp $(LDFLAGS)

.PHONY: test clean

test: Vulkan
	optirun ./Vulkan

clean:
	rm -f Vulkan
