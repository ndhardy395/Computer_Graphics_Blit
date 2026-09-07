# Computer Graphics — Lab 1: 2D BLIT System

A software rasterizer written from scratch for Full Sail University's Computer Graphics
course. There is no GPU and no graphics API here: the program owns a flat array of 32-bit
colors, writes into it one pixel at a time, and hands the finished array to a Win32 window
once per frame.

**Author:** Noah Hardy

---

## Building

Open `BlitCGs.slnx` in Visual Studio 2026 and build. The project targets the v145 toolset,
C++20, x64, Console subsystem.

From the command line:

```
msbuild BlitCGs.vcxproj -p:Configuration=Release -p:Platform=x64
```

The executable lands in `x64\Release\BlitCGs.exe`. Close the window to exit — `RS_Update`
returns `false` when the window closes, which ends the render loop.

---

## Project layout

| File | Role |
| --- | --- |
| `BlitCGs.cpp` | `main` — sets up the surface, runs the render loop, tears down |
| `Surface.h` / `Surface.cpp` | The raster surface class. All drawing lives here |
| `SpriteAtlas.h` | Named rectangles into the texture sheet |
| `Scene.h` | Placeholder for scene composition |
| `RasterSurface.h` / `.cpp` | **Instructor-provided.** Creates the Win32 window and blits to it |
| `XTime.h` / `.cpp` | **Instructor-provided.** High-resolution timer |

Files marked instructor-provided are course utilities by L. Norri and are not my work.

---

## How it works

### Two pixel formats, and why the conversion exists

The generated texture headers and the window expect different byte orders:

| | Layout | Alpha position |
| --- | --- | --- |
| Texture headers | `0xBBGGRRAA` | low byte |
| `RS_Update` | `0xAARRGGBB` | high byte |

Converting between them is a full four-byte reversal. `SourceToXRGB` masks off one channel
at a time, shifts each to its destination, and ORs the pieces back together. It runs once per
pixel when a texture is loaded, so every `Surface` in the running program is already in the
format the window wants and no conversion happens in the draw path.

A quick way to read a raw value: `0xffffff00` is white with alpha `00` — a fully transparent
pixel, which is most of any sprite sheet. `0x2e3771ff` is opaque, with blue `2e`, green `37`,
red `71`.

### The Surface class

`Surface` owns a heap-allocated pixel buffer and implements the Rule of Three — destructor,
copy constructor, and copy assignment with a self-assignment guard — so copying one never
double-frees or aliases another's memory.

The design decision that matters: **the screen and every source image are the same type.**
A BLIT is therefore always `Surface -> Surface`, and tiling a background, stamping a building,
and drawing one cell of an animation are the same function called with different numbers.

| Member | Purpose |
| --- | --- |
| `Index(x, y)` | The 2D-to-1D conversion: `y * width + x` |
| `operator()(x, y)` | Unchecked element access, for callers that already know they are in range |
| `Clear(color)` | Fills the whole buffer with one color |
| `SetPixel(x, y, color)` | Writes one pixel, discarding out-of-range coordinates |
| `BlendPixel(x, y, color)` | Source-over blend against what is already there |
| `Blit(...)` | Copies a rectangle out of a source surface, clipped on all four edges |
| `Tile(...)` | Repeats one rectangle across the entire surface |
| `Data()` / `Count()` | The two arguments `RS_Update` needs |

### Row stride

`Blit` reads through the source's width and writes through its own. Those two numbers are
different, and confusing them is the classic failure in this lab — it produces a skewed,
sheared copy. Nothing assumes a width; each surface supplies its own.

### Clipping, not clamping

The buffer is one flat run of pixels, so an x coordinate past the right edge does not fall off
the screen — it silently lands on the next row down. `SetPixel` therefore **discards**
out-of-range coordinates rather than clamping them, which would smear a column of pixels along
the border instead. This is what lets a sprite hang off any edge safely and what keeps the
partial tiles at the right and bottom of the window from wrapping.

### Alpha blending

Sprite cells carry alpha below 255, so a straight copy would paint a hard box. `BlendPixel`
interpolates per channel:

```
out = (source * alpha + destination * (255 - alpha)) / 255
```

Each channel is unpacked to its own byte before the arithmetic. Blending the packed
`unsigned int` directly would let one channel's math overflow into the next channel's byte.

---

## Status

Tracked against the assignment's grading tiers.

| Tier | Requirement | Status |
| --- | --- | --- |
| 25% | Tiled background across the entire color array | Not yet — needs a texture |
| 50% | Correct channel order, 10 randomly placed BLITs, no wrap, no Release crash | Not yet |
| 75% | One animation cell rendered with alpha blending | Not yet |
| 100% | Animation playing at 30 FPS, window at 30 FPS or better | Not yet |

**Where the code currently stands:** `Surface` is complete and compiles clean. `main` does not
yet create a surface or run a render loop, so none of the drawing code has executed. There are
no image assets or generated texture headers in the repository yet, which blocks everything
from the 25% tier onward.

Immediate next step is the instructor's step 1 — wire up the render loop and draw a single
pixel in a contrasting color to confirm the buffer, `Index`, and `SetPixel` behave. That needs
no assets and exercises most of `Surface`.

`Source.cpp` is an empty file left over from project creation and should be removed.
`Scene.h` and the `TownAtlas` namespace in `SpriteAtlas.h` are empty placeholders; the atlas
cannot be filled in until a texture header exists, since its rectangles are coordinates into a
specific sheet.

---

## Assets and attribution

Image assets come from the weekly handout.

- **Town tileset** — by **Buch**. Credit is not required but is appreciated by the author;
  see `TownTiles/Attribution.txt` in the handout.
- **Particle FX pack** (`para_CC0_particlefx-2`) — CC0. Sheets are laid out as a grid of
  128x128 cells at 30 FPS: 1024x1024 sheets hold 64 frames, 512x512 sheets hold 16.

Cells run left to right across the full width of a sheet, so frame *N* sits at
`srcX = (N % columns) * 128`, `srcY = (N / columns) * 128`. Cropping a square quadrant out of
a large sheet does **not** yield a contiguous run of frames — it skips every other block. To
shorten a 64-frame sheet, crop whole rows instead.

`TGA_Pow2_32bit_To_TextureArray.exe` from the handout converts a 32-bit uncompressed `.tga`
into a header. As the name says, it requires power-of-two dimensions on both axes, so any
source image that is not already power-of-two has to be padded before conversion.

---

## Notes

- Window is 500x500. That divides evenly by no reasonable tile size, which is deliberate —
  the leftover strip at the right and bottom edges is what the clipping in `Blit` exists for.
- The window title comes from the first argument to `RS_Initialize` and must carry the
  author's name and lab number.
- Generated texture headers are large (roughly 11 bytes of text per pixel) but are committed
  deliberately: the project does not compile without them. Build output, the `.vs` folder, and
  IntelliSense caches are excluded in `.gitignore`.
