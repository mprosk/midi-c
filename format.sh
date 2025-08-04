#!/bin/bash

find midi -name "*.c" -o -name "*.h" | xargs clang-format -i --verbose
find test -name "*.c" -not -name "unity.c" -o -name "*.h" -not -name "unity.h" -not -name "unity_internals.h" | xargs clang-format -i --verbose
