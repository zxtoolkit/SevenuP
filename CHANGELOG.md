# Changelog

Changes this repository makes on top of the upstream SevenuP 1.21 WIP source.

This is not the program's own changelog. Jaime Tejedor Gómez's version
history, covering 0.0 alpha in 2002 through 1.21 WIP in 2009, is in
`SevenuP.txt` under "Version History" and is left untouched.

Nothing here is released or tagged; the entries below describe the commits
that sit on top of the second import. See "Repository lineage" in `README.md`
for how the history is ordered.

The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [Unreleased]

### Fixed

- **`SevenuP.ini` was never read.** 1.21 commented out the `ifstream`
  declaration and the whole read loop, but the `__WXMAC__` block below them
  still called `inifile.fail()`, `.clear()` and `.open()`, so it did not
  compile on Mac. On every platform `ValidIni` was left at 0, so
  `GetIniValue()` always returned its default argument and every setting in
  `SevenuP.ini` was silently ignored: zoom, grid, colours, export options.
  Restored by uncommenting the author's own lines, which are already the
  Unicode-aware version. Nothing was rewritten.

- **`_()` applied to a char literal.** `_('\0')` appears three times, twice in
  `OpenFile.cpp` and once in `TheFrame.cpp`. `_()` is the gettext translation
  macro and takes a string, not a char; wxWidgets 3 rejects it with a
  `static_assert`. The intent is to append a NUL, not to translate one.

- **Build against wxWidgets 3**, 28 errors. `AddTool()` gained a label
  parameter and replaced its toggle, position and clientData arguments with a
  `wxItemKind`, affecting 16 calls. `wxBitmap::SetDepth()` was removed; the
  loop calling it is dropped rather than replaced, because every element it
  touched was overwritten by `wxBITMAP()` on the following lines and it had no
  effect already. `wxSAVE` and `wxOVERWRITE_PROMPT` became `wxFD_SAVE` and
  `wxFD_OVERWRITE_PROMPT`, 10 uses across 5 file dialogs.

- **`makefile.osx`.** `wx-config --static` finds no archives against a
  Homebrew keg, which ships dylibs only, so the flag is dropped. The link rule
  used `$(LINK.o)`, which expands to `$(CC)`; linking with `cc` leaves every
  C++ runtime symbol undefined, so the variable is now `CXX` and the rule
  calls it directly. `WX_CONFIG` is overridable. The bundle rule depends on
  its inputs so it rebuilds when they change, and ad-hoc signs the result,
  because unsigned binaries are killed on launch on Apple silicon; `install -s`
  is dropped because stripping invalidates the signature.

### Added

- **Opening files double-clicked in the Finder.** The code only ever read file
  names from `argv`, and macOS does not put them there: the Finder sends an
  `odoc` Apple Event, surfaced as `wxApp::MacOpenFiles()`, which nothing
  overrode, so files opened from the Finder were silently discarded. The
  override forwards to the frame's existing `OpenArrayFiles()`, the same entry
  point the drag-and-drop target already used, passing the whole array at once
  so the canvas is refreshed after the last file rather than once per file.
  `~TheFrame` clears the cached frame pointer, so an Apple Event arriving
  between the frame being deleted and the app exiting cannot dereference freed
  memory.

- **`SRC/Info.plist`.** `makefile.osx` has always installed this file into the
  bundle, but it is in neither source release, so the build stopped at the
  bundle step. Reconstructed from the copy embedded in `SevenuP.pbproj`, plus
  a bundle identifier, `NSPrincipalClass` and `NSHighResolutionCapable`. It
  also declares `CFBundleDocumentTypes` and `UTExportedTypeDeclarations` for
  `.sev` and `.scr`, so the Finder routes those files here at all. `.sev` is
  rank Owner; `.scr` is Alternate, being a bare 6912-byte memory dump with no
  magic number that other Spectrum tools also claim.

- **`LICENSE`**, a byte-identical copy of the `GNU-GPL License.txt` the
  project has always shipped, under a name GitHub's licence detection
  recognises. The original file is left in place.

- **`README.md`**, describing the program, the build, and the repository's
  lineage. `README.TXT` is the author's own documentation and is left
  untouched, though its build instructions no longer match this tree.

- **`.gitignore`** for the object files, linked binary and app bundle that
  `makefile.osx` leaves in the source directory.
