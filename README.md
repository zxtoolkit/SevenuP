# SevenuP

A ZX Spectrum graphic editor by Jaime Tejedor Gómez (Metalbrain), written in
C++ against wxWidgets.

Upstream homepage, with source and binary packages:
<https://metalbrain.speccy.org/>

This repository carries the original source forward to a compiler and a
wxWidgets that exist today, on macOS. The upstream code stops at 2009 and does
not build against wxWidgets 3.

`README.TXT` is the author's own documentation and is preserved untouched. Its
build instructions describe wxWidgets 2.4/2.6/2.8 and no longer match this
tree; use the instructions below instead.

## Building on macOS

```sh
brew install wxwidgets
cd SRC
make -f makefile.osx
```

That produces `SRC/SevenuP.app`. Built and run against wxWidgets 3.3.3 on
macOS 26 (Apple silicon). The build is clean; the remaining warnings are
deprecations (`wxPen`/`wxBrush` style constants, `sprintf`).

`wx-config` must be on `PATH`. For a wxWidgets installed elsewhere:

```sh
make -f makefile.osx WX_CONFIG=/path/to/wx-config
```

The bundle is ad-hoc signed by the makefile, because unsigned binaries are
killed on launch on Apple silicon.

### Other platforms

`makefile.unx`, `makefile.gcc`, `makefile.g95`, `makefile.freebsd` and
`compileSevenuPFreeBSD` are upstream files, left as they were shipped. They
have not been touched or tested here, and they carry the same wxWidgets 3
incompatibilities that `makefile.osx` needed fixing for.

## What it does

Edits ZX Spectrum graphics under the machine's attribute constraints: 8x8
character cells, one ink and one paper colour per cell, bright and flash
flags. Menus for fills, effects, masks, sprite frames, grid and zoom.

- Opens and saves `.sev` (SevenuP's own format) and `.scr` (6912-byte screen
  memory dump)
- Imports images (BMP, GIF, JPG, PNG, PCX, TIF, IFF, XPM) and raw binary
- Exports images (PNG, BMP, JPG, PCX, TIF, XPM)
- Exports data as raw binary, ASM source or C source

## Repository lineage

The history is ordered so that upstream releases come first and unmodified,
and every later commit is a change this repository makes.

1. **Import SevenuP 1.20 source** — the 1.20 release, extracted verbatim
2. **Import SevenuP 1.21 WIP source** — the unfinished work-in-progress
   release, extracted verbatim
3. **Fix two defects left behind by the unfinished 1.21 refactor** — first
   non-upstream commit
4. **Ignore build output**
5. **Build against wxWidgets 3 on macOS**
6. **Open files double-clicked in the Finder**
7. **Add LICENSE so the GPL is detected**

Both import commits are byte-for-byte identical to the published archives,
CRLF line endings included. `core.autocrlf` is off in this repository so they
stay that way.

- 1.20: <https://metalbrain.speccy.org/SevenuP-v1.20-src.zip>
- 1.21 WIP: <https://metalbrain.speccy.org/SevenuP-v1.21src_WIP.zip>

### What 1.21 WIP was

Its entire changelog entry reads:

```
2009-??-??: Version 1.21 (Work In Progress)
    (Source) Fixed Unicode compilation and also fixed gcc 4.1 compilation
        problems. Welcome Ubuntu users!
```

It is a build-fix branch for its own era, not a feature release, and it was
never finished. It does not help with wxWidgets 3: the API removals that
break the build here postdate it, and it is carried as the base only because
it is the author's latest published source.

### What this repository changes

In short: two defects 1.21 left mid-refactor, wxWidgets 3 compatibility, and
Finder double-click support. `CHANGELOG.md` has the detail.

Note that `CHANGELOG.md` covers only this repository's changes. The program's
own version history, 2002 to 2009, is in `SevenuP.txt` under "Version
History", written by the author and left untouched.

## License

SevenuP is © 2002–2007 Jaime Tejedor Gómez, aka Metalbrain.

Licensed under the GPL, version 2 or later, as granted by the header of every
source file. Copyright and licence belong to the author; this repository only
carries the source forward and claims nothing over it.

The two upstream statements disagree on the end year: every source file header
says 2002–2007, while the "Legal" section of `SevenuP.txt` still says
2002–2006, not having been updated when 1.21 bumped the headers. The later
range is used above.

`LICENSE` is a byte-identical copy of the `GNU-GPL License.txt` the project
has always shipped, added under a name GitHub's licence detection recognises.
It is the 1991 printing of the GPL: the only differences from the text the FSF
publishes today are the old postal address, the pre-1999 "Library" (now
"Lesser") GPL naming, and whitespace.
