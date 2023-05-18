.PHONY: format

format:
	find ./ -iname *.hpp -o -iname *.cpp | xargs clang-format -i
