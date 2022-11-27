# STB
**Compile-time** Hex-sequence String to Byte Array.

# Why?
You may ask, why'd you want to do this? Well, this is a common issue in the cheat development scene, where people resort to inefficient alternatives, which are done at runtime, despite that often not being required. (Obviously, anyhow, if your input is not available at compile-time, you must adapt to a run-time approach, but in publicly available projects, this is not the case, more often than not.) This allows you to use a standard format input (spec. "IDA-style") string, which'll get converted to a standard array, which then you can directly use. This, unlike runtime methods, hides the string behind the operation. This also has the most compliant semantics allowing for complex formations. it also allows for parser customization (i.e. change delimiter character, wildcard character, wildcard character value) by the generic formator.

# How?
You can download the self-reliant header and include it to your project, or do this:
```
#include <https://raw.githubusercontent.com/cristeigabriel/STB/main/stb/stb.hh>
```

# Usage
Refer to [this](https://github.com/cristeigabriel/STB/blob/main/stb.cc) for an example and test.

# License
[WTFPL](https://github.com/cristeigabriel/STB/blob/main/LICENSE)
