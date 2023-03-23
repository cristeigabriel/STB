# STB
Compile-time conversion library, from IDA-style string to array (supports wildcarding).

# How to use
Step 1:
```
git clone https://github.com/cristeigabriel/STB.git
```

Step 2:
Include `include/stb.hh` to your project.

Step 3:
Either create a custom conversion type with `stb::basic_hex_string_array_conversion` or use `stb::hex_string_array_conversion` (whitespace delimiter, question-mark wildcard, `-1` wildcard value) (short alias: `stb::simple_conversion`).

Example:
```
stb::simple_conversion::build<stb::fixed_string {"AA BB CC DD EE FF"}>::value
// stb::simple_conversion::build<"AA BB CC DD EE FF">::value also works
```

You may also create a sanitary macro that looks something like:
```
#define CONV(x) stb::simple_conversion::build<stb::fixed_string {X}>::value
```
then do:
```
CONV("AA BB CC DD EE FF")
```

# How this works
We pass a `stb::fixed_string` to a conversion type's build, we run `make` on it, we ensure constant evaluation through `stb::consteval_value` on `make` which itself calls `parse::get` then does some logic utilizing the information and `detail`, we store the result of `make` in `value` inside the build instance.

# Results
This leaves no trace in the binary by ensuring constant evaluation (doesn't require `consteval`) of conversion string parser and conversion builder.

# Compile times
This should go in a pre-compiled header. You may also disable compliance verifications by defining `STB_OMIT_TESTS` before include.

# License
[LICENSE](./LICENSE)