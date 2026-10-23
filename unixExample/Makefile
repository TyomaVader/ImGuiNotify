CMAKE ?= cmake

.PHONY: all clean
all:
	$(CMAKE) -S . -B build -DCMAKE_BUILD_TYPE=Debug
	$(CMAKE) --build build --parallel

clean:
	$(CMAKE) --build build --target clean
