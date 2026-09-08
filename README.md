# Computer Graphics — Lab 1: 2D BLIT System

A software rasterizer written from scratch for Full Sail University's Computer Graphics
course. There is no GPU and no graphics API involved: the program owns a flat array of
32-bit colors, writes into it one pixel at a time, and hands the finished array to a Win32
window once per frame.

The result is a small animated town — a tiled grass field, hand-placed buildings, ten
randomly scattered props that never overlap the town, and a 64-frame fire playing at a
fixed 30 frames per second.

**Author:** Noah Hardy

---

## Building and running

Open `BlitCGs.slnx` in Visual Studio 2026 and build. The project targets the v145 toolset,
C++20, x64, Console subsystem.

From the command line:

```
msbuild BlitCGs.vcxproj -p:Configuration=Release -p:Platform=x64
```

The executable lands in `x64\Release\BlitCGs.exe`. Closing the window ends the program —
`RS_Update` returns `false` once the window is gone, which terminates the render loop.

Expect a slow first build. `fire_02.h` is 11 MB of hex literals and `TownTiles.h` is
another 0.7 MB; the compiler has to parse both.

---

## Project layout

| File | Role |
| --- | --- |
| `BlitCGs.cpp` | `main` — builds the surfaces, runs the render loop, drives the animation clock |
| `Surface.h` / `.cpp` | The raster surface class. All pixel work lives here |
| `Scene.h` / `.cpp` | Town layout, random scatter with collision rejection, and drawing |
| `SpriteAtlas.h` | Named rectangles into the town sheet |
| `TownTiles.h` | Generated. The town tileset as pixel data |
| `fire_02.h` | Generated. The fire animation sheet as pixel data |
| `RasterSurface.h` / `.cpp` | **Instructor-provided.** Creates the Win32 window and blits to it |
| `XTime.h` / `.cpp` | **Instructor-provided.** High-resolution timer |

Files marked instructor-provided are course utilities by L. Norri and are not my work.

---

## How it works

### Two pixel formats

The generated texture headers and the window disagree about byte order:

| | Layout | Alpha position |
| --- | --- | --- |
| Texture headers | `0xBBGGRRAA` | low byte |
| `RS_Update` | `0xAARRGGBB` | high byte |

Converting between them is a full four-byte reversal. `SourceToXRGB` masks off one channel
at a time, shifts each to its destination, and ORs the pieces back together. It runs once
per pixel at load time, so every `Surface` in the running program is already in the format
the window wants and no conversion happens in the draw path.

Reading a raw value by hand: `0xffffff00` is white with alpha `00` — a transparent pixel,
which is roughly half of the town sheet. `0x2e3771ff` is opaque, blue `2e`, green `37`,
red `71`.

### The Surface class

`Surface` owns a heap-allocated pixel buffer and implements the Rule of Three — destructor,
copy constructor, and copy assignment with a self-assignment guard.

The design decision that matters: **the screen and every source image are the same type.**
A BLIT is always `Surface -> Surface`, so tiling a background, stamping a building, and
drawing one cell of an animation are the same function called with different numbers.

| Member | Purpose |
| --- | --- |
| `Index(x, y)` | The 2D-to-1D conversion: `y * width + x` |
| `operator()(x, y)` | Unchecked element access for callers already known to be in range |
| `Clear(color)` | Fills the whole buffer with one color |
| `SetPixel(x, y, color)` | Writes one pixel, discarding out-of-range coordinates |
| `BlendPixel(x, y, color)` | Source-over blend against what is already there |
| `Blit(...)` | Copies a rectangle from a source surface, clipped on all four edges |
| `Tile(...)` | Repeats one rectangle across the entire surface |
| `Data()` / `Count()` | The two arguments `RS_Update` needs |

### Row stride

`Blit` reads through the source's width and writes through its own. Those two numbers are
different, and confusing them is the classic failure in this lab — it produces a sheared
copy. Nothing assumes a width; each surface supplies its own.

### Clipping, not clamping

The buffer is one flat run of pixels, so an x coordinate past the right edge does not fall
off the screen — it silently lands on the next row down. `SetPixel` therefore **discards**
out-of-range coordinates rather than clamping them, which would smear a column of pixels
down the border instead.

That single decision is what lets a sprite hang off any edge safely, what keeps the partial
tiles at the right and bottom of the window from wrapping, and what stops the program
crashing in Release when a random prop lands half off the screen.

### Alpha blending

Animation cells carry alpha below 255, so a straight copy would paint a hard box.
`BlendPixel` interpolates per channel:

```
out = (source * alpha + destination * (255 - alpha)) / 255
```

Each channel is unpacked to its own byte first. Blending packed `unsigned int` values
directly would let one channel's arithmetic overflow into the next channel's byte.

Two shortcuts keep it fast: alpha `0` returns immediately, and alpha `255` skips the
arithmetic and copies.

### The town layout

Placements live in a table rather than a wall of `Blit` calls, so the layout is data.

Sprites sharing a row have their **bottom** edges aligned, not their tops — each `y` is
`baseline - height`. The top row all lands on y = 113, the bottom row on y = 468. Aligning
tops instead makes buildings look like they are floating at different depths.

### The random scatter

`ScatterRandom` uses **rejection sampling**: pick a position at random, test it, and try
again if it fails. Computing the space a sprite could legally occupy is hard; guessing and
checking is easy.

Each candidate is tested against the fire's reserved footprint, then every town placement,
then every prop already placed, using an axis-aligned bounding box test. Two rectangles
miss if either is entirely to one side of the other on either axis; `RectsOverlap` is the
negation of that, written positively.

Positions are offset by half the sprite so props can hang off any edge — which exercises
the clipping path on every run — while guaranteeing the inner half stays visible. An
attempt cap prevents an infinite loop on a crowded screen, which is why the function
returns how many it actually placed instead of assuming it placed them all.

### Animation timing

The window redraws far faster than 30 Hz, so the frame index cannot advance once per loop
iteration or the fire would play at whatever speed the machine happens to hit.

Instead `XTime::Signal()` and `Delta()` bank elapsed seconds in an accumulator, and a
`while` loop spends them one thirtieth of a second at a time. Playback is 30 FPS regardless
of how fast the window runs.

Frame *N* sits at `srcX = (N % columns) * 128`, `srcY = (N / columns) * 128` — modulo gives
the column, integer division gives the row.

---

## Status

| Tier | Requirement | Status |
| --- | --- | --- |
| 25% | Tiled background across the entire color array | **Complete** |
| 50% | Correct channel order, 10 randomly placed BLITs, no wrap, no Release crash | **Complete** |
| 75% | One animation cell rendered with alpha blending | **Complete** |
| 100% | Animation playing at 30 FPS, window at 30 FPS or better | **Complete** |
| Extra | Static placements with randoms that avoid intersecting them | **Complete** |

The last row is the assignment's optional additional challenge. The twelve town placements
are hand-authored, and the ten scattered props reject any position that overlaps them.

---

## Assets

Both sheets come from the weekly handout, converted with the provided tool.

| Sheet | Source | Size | Mip levels | Notes |
| --- | --- | --- | --- | --- |
| `TownTiles.h` | `tiles_12.png` | 400 x 224 | 1 | Irregular sprites, not a grid |
| `fire_02.h` | `fire_02.png` | 1024 x 1024 | 11 | 8 x 8 grid of 128 px cells, 64 frames |

### Pipeline

1. Export the PNG as **32-bit uncompressed TGA** (RLE off) — the tool cannot read PNG.
2. Put the TGA next to `TGA_Pow2_32bit_To_TextureArray.exe` and run it.
3. It writes a `.h` beside the TGA. Symbol names come from the filename.

### Two things the handout gets wrong or leaves out

**The runtime.** The handout says to install the Visual C++ **2015** redistributable. The
tool actually imports `MSVCP120.dll` and `MSVCR120.dll`, which is Visual C++ **2013**.
Installing 2015 changes nothing and the tool keeps failing with `0xC0000135`
(DLL not found). Confirmed by dumping the executable's import table.

**"Pow2" describes the mipmaps, not a requirement.** Non-power-of-two images convert fine —
the tool prints "Image is NOT a power of 2! Mipmap generation will be skipped" and emits
`numlevels = 1`. The 400 x 224 town sheet needs no padding.

### Reading a generated header

The tool appends a full mip chain to power-of-two images, so `numpixels` is about 1.33x
`width * height`. Level 0 sits at offset 0, so passing the pixel array straight into
`Surface(source, width, height)` reads the full-resolution image and ignores the rest.

Cell size, column count and frame count are animation metadata the tool cannot know. They
are derived in `BlitCGs.cpp` from the header's own width and height, so re-cropping the
sheet and regenerating updates them automatically.

### How the sprite rectangles were measured

`SpriteAtlas.h` holds tight bounding boxes found by connected-component labeling on the
sheet's alpha channel: build a mask of pixels with alpha above 8 (ignoring antialiased
edges), flood-fill each blob with 8-connectivity, and record the extents.

`GRASS_TILE` is the exception — it is a region inside a sprite, not a sprite, found by
scanning for a 16 x 16 block that is fully opaque and low-variance so it repeats without a
visible seam.

### Attribution

- **Town tileset** by **Buch**. Credit is not required but is appreciated by the author.
- **Particle FX pack** (`para_CC0_particlefx-2`) is CC0.

---

## Notes

- The window is 500 x 500, which divides evenly by no reasonable tile size. That is
  deliberate: the leftover strip at the right and bottom edges is exactly what the clipping
  in `Blit` exists to handle.
- The scatter is seeded from `time(nullptr)`, so every launch produces a different layout.
  Replace it with a fixed value to reproduce a specific one.
- Generated texture headers are committed deliberately — the project does not compile
  without them. Build output, `.vs`, and IntelliSense caches are excluded in `.gitignore`.
