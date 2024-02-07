all:
	cmake -Bbuild -H. -DDEBUG_BUILD=ON -DLOG_RAW=ON; cmake --build build

clean:
	@rm -rf build/
	@echo "All build artifacts removed"

.PHONY: all clean