cpp_files := $(wildcard *.cpp)
wx_flags := $(shell wx-config --cxxflags)
wx_libs := $(shell wx-config --libs)
output := exec

build: $(cpp_files)
	@echo "Compiling"
	@g++ $(wx_flags) -o $(output) $(cpp_files) $(wx_libs)
run: build
	@echo "Running"
	@./exec
clean:
	@echo "Cleaning"
	@rm exec
