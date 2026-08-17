# CR Identicon
An identicon icon generator in both C and Rust (CR). I made this for use in my website, it really is a nice way of providing a default user profile image (that does not look boring).

## Inner-Working
Identicon are a visual representation of a hash value, normally the IP address + epoch_time, so it does not sacrifice the privacy of the user. A lot more about them can be read at [wikipedia](https://en.wikipedia.org/wiki/Identicon) however the core essence is quite simple.

Where most libraries do diverge is the effective rendering, some use squares, some distinct forms, this library uses squares.
To achieve their symmetric shapes most generators use a mirror technique (they only really generate 1 half).

You can pass a border offset so it will still look good for rounded profile pictures!

```c
// code example here!
```

## Attribution
MIT License - Wolf Pieter Schulz. Identicon Inventor: Don Park.