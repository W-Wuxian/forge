find ../ -name '*.cu' -o -name '*.c' -o -name '*.h' | xargs clang-format -i -style=file:../.clang-format
