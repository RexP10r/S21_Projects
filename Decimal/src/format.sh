clang-format -style=Google -i *.h funcs/* tests/* 
clang-format -style=Google -n *.h funcs/* tests/* 
cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h funcs/* tests/*

