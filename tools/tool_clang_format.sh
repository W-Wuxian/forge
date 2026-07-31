find ../fwht_utils -name '*.cu' -o -name '*.c' -o -name '*.h' | xargs clang-format -i -style=file:../.clang-format
find ../fwht -name '*.cu' -o -name '*.c' -o -name '*.h' | xargs clang-format -i -style=file:../.clang-format
find ../ -name 'base_datatype.h' | xargs clang-format -i -style=file:../.clang-format
find ../ -name 'base_header.h' | xargs clang-format -i -style=file:../.clang-format