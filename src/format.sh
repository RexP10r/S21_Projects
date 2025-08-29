clang-format -style=Google -i *.h funcs/* tests/* simple_tests/*
clang-format -style=Google -n *.h funcs/* tests/* simple_tests/*
cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h funcs/* tests/* simple_tests/*

