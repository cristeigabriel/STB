#include <iostream>
#include "stb/stb.hh"

// tests
int main() {
    // string is not pushed. the whole process is explicitly compile-time. what's present after compilation is the value, inlined
    // with no trace.
    using make         = stb::compiletime_string_to_byte_array_data;
    constexpr auto arr = make::getter<"         AA   BB CC DD EE         BB             ">::value;

    // assert to expected result
    static_assert(arr[0] == 0xaa);
    static_assert(arr[1] == 0xbb);
    static_assert(arr[2] == 0xcc);
    static_assert(arr[3] == 0xdd);
    static_assert(arr[4] == 0xee);
    static_assert(arr[5] == 0xbb);
    static_assert(arr.size() == 6);
}