all:
	@astyle --quiet --options=astylerc --recursive "src/*.cpp,*.hpp"
	cmake -Bbuild -H. -DDEBUG_BUILD=OFF -DLOG_RAW=ON; cmake --build build

clean:
	@rm -rf build/
	@echo "All build artifacts removed"

.PHONY: all clean