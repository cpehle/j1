packages = 
interfaces =
modules = stack.v
top_module = j1

CXXFLAGS = -fPIC

verilator:
	verilator -trace --trace-structs --top-module $(top_module) -Wno-fatal -Wall -y ../src/ --cc $(packages) $(interfaces) $(modules) $(top_module).v
	cd obj_dir && make CXX=$(CXX) CXXFLAGS=$(CXXFLAGS) -f Vj1.mk

clean:
	rm -rf obj_dir
