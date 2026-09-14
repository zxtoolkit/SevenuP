# Changelog

Changes this repository makes on top of the upstream SevenuP 1.21 WIP source.
The program's own version history is in `SevenuP.txt` under "Version History".

Format: [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [Unreleased]

## [v1.21.0-rc4] - 2026-09-14

### Added

- Windows version resource, so `SevenuP.exe` carries a product name, version
  and copyright in its file properties instead of showing none.

### Changed

- Linux AppImage halved, 33 MB to 17 MB. It no longer carries ICU, which
  nothing in it used, nor the CUPS print backend and the TLS, Kerberos and
  Avahi stack behind it, which SevenuP has no way to reach.

## [v1.21.0-rc3] - 2026-09-14

### Fixed

- macOS build required macOS 14 and would not start on anything older. The
  deployment target was set for wxWidgets only, not for the app itself.
- Intel slice now targets macOS 10.13; arm64 targets 11.0, its lowest possible.

## [v1.21.0-rc2] - 2026-09-13

### Fixed

- macOS toolbar was drawn in the title bar, hiding most tools behind an
  overflow chevron. It is now a child window on macOS; Windows and Linux
  unchanged.
- Nine toolbar icons were washed out or invisible on non-Windows platforms.
  Their XPMs declared a transparent background where the artwork expects black.

### Changed

- CI keeps the cached wxWidgets build when a later step fails.

## [v1.21.0-rc1] - 2026-09-13

### Added

- Self-contained binaries for macOS, Linux and Windows, built and published by
  GitHub Actions. wxWidgets is linked in statically, so nothing needs
  installing to run them.
- `makefile.mingw` for Windows; `makefile.unx` and `makefile.osx` fixed to link
  with a C++ driver.
- Files double-clicked in the Finder now open. `Info.plist` declares the `.sev`
  and `.scr` document types.
- `LICENSE`, `README.md`, `CHANGELOG.md`, `.gitignore`.

### Fixed

- `SevenuP.ini` was never read, so every setting fell back to its default.
- `_()` applied to a char literal, rejected by wxWidgets 3.
- 28 build errors against wxWidgets 3: `AddTool()`, `wxBitmap::SetDepth()`,
  `wxSAVE` and `wxOVERWRITE_PROMPT`.
- Sprite frame stepping was unreachable on Linux: GTK refuses bare arrow keys
  as accelerators. The canvas now handles them directly.
- New Graph dialog's spin controls were allocated negative width on GTK.

[Unreleased]: https://github.com/zxtoolkit/SevenuP/compare/v1.21.0-rc4...HEAD
[v1.21.0-rc4]: https://github.com/zxtoolkit/SevenuP/compare/v1.21.0-rc3...v1.21.0-rc4
[v1.21.0-rc3]: https://github.com/zxtoolkit/SevenuP/compare/v1.21.0-rc2...v1.21.0-rc3
[v1.21.0-rc2]: https://github.com/zxtoolkit/SevenuP/compare/v1.21.0-rc1...v1.21.0-rc2
[v1.21.0-rc1]: https://github.com/zxtoolkit/SevenuP/releases/tag/v1.21.0-rc1
