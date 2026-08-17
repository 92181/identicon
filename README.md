# CR Identicon
An identicon icon generator in both C and Rust (CR). I made this for use in my website, it really is a nice way of providing a default user profile image (that does not look boring).

## Inner-Working
Identicon are a visual representation of a hash value, normally the IP address + epoch_time, so it does not sacrifice the privacy of the user. A lot more about them can be read at [wikipedia](https://en.wikipedia.org/wiki/Identicon) however the core essence is quite simple.

Where most libraries do diverge is the effective rendering, some use squares, some distinct forms, this library uses squares.
To achieve their symmetric shapes most generators use a mirror technique (they only really generate 1 half).

You can pass a border offset so it will still look good for rounded profile pictures!

```c
struct timespec s, e; clock_gettime(1, &s);

char color[4] = {0}, background[4] = {0, 0, 0}; color[0] = 0; color[1] = 0; color[2] = 255;

char *icon = identi_as_buffer(s.tv_nsec, 16, 8, 512, color, background);

if(icon != 0)
{
  icon_buff_to_bmp(icon, GRID_WIDTH, "./output.bmp");

  free(icon);
}
```

```rs
let seed = Instant::now().elapsed().as_nanos() as u64;

let (color, background) = ([0, 0, 255], [0, 0, 0]);

// Generate Icon
let icon = cr_identicon::gen_identicon(seed, 32, 32, 512, color, background);

cr_identicon::icon_buff_to_png(&icon, 512, "./output.png").expect("Failed to save PNG");
cr_identicon::icon_buff_to_jpg(&icon, 512, "./output.jpg").expect("Failed to save JPG");
```

## Attribution
MIT License - Wolf Pieter Schulz. Identicon Inventor: Don Park.

No AI was used in the making of this project.