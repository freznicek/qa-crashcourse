# QA crashcourse makefile

base_dir := $(patsubst %/,%, $(dir $(realpath $(firstword $(MAKEFILE_LIST)))))

rebuild: clean build

clean:
	$(RM) triangle triangle.stripped

build: triangle triangle.stripped

triangle:
	$(CXX) -g -std=c++11 $(base_dir)/src/triangle/triangle.cpp -o $(base_dir)/triangle

triangle.stripped: triangle
	cp -f $(base_dir)/triangle $(base_dir)/triangle.stripped
	strip $(base_dir)/triangle.stripped

info:
	file $(base_dir)/triangle*

doc:
	less $(base_dir)/README.md
# eof
