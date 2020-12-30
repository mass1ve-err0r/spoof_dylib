# spoof_dylib
> C11 | OS Independent 

## Prelude / What is this?
> *TL;DR:* allows you to audit the LC_UUID segment of mach object files on Windows + MacOS

While reading through the Mach-O format specs I noticed that there is a structure dubbed `LC_UUID` which stores a unique identifier (UUID v5). The real purpose of having an UUID is to uniquely identify an object file _or_ to find its corresponding `.dSYM` file _(debug symbol file)_.

<details>
  <summary>(Click here for a quick dirty Mach-O recap)</summary>
  
  Mach Object Files *(short Mach-O)* are usually made up of a `mach_header` or `mach_header_64` (depending on your target) which describes the object files' layout.
  
  _(You can join multiple Mach-O's together with tools such as `lipo` to create "universal" binaries, these use a different header, namely `fat_header` and need to be browsed a bit differently)_
  
  Immediately after follow a series of `LC_*` structs which describe the object's content (vm mapping, symbol table location, linkage et. al.).
  
  **However**, before any `LC_*` struct is a `load_command` struct which describes the `LC_*` type and total size _(the `cmd` and `cmdsize`fields)_ for easier identification.
</details>

This had me curious if two object files can have the same _internal_ identifier and still be loaded without issues.

Test were done on the following devices:
- iPhone 5 _(arm v7, jailbroken and non-jailbroken)_
- iPhone 8 _(arm64, jailbroken and non-jailbroken)_
- iPhone XS Max _(arm64e, jailbroken and non-jailbroken)_
- macOS Catalina _(x64)_

From my testing this hasn't caused any issues so far nor conflicts, which means this segment _can_ be freely altered for other purposes!

This is primarily interesting from a DRM perspective since you could embed a _(strong)_ license within the `LC_UUID` during generation and implement a self-checking algorithm.

An exmaple for the above will be posted sometime in the future as PoC.


This works for all Mach-O types (fat or thin) and on Windows and macOS.

## Usage
`spoof_dylib <absolute_path_to_dylib> <new_uuid_as_hex>`
- The first argument has to be the `.dylib`'s full **absolute** path. Why no relative? I have no idea.
- The second argument is the new UUID as **full 32 character (padded) hex string**.

Examples:
`spoof_dylib /Users/saadat/some_folder/example1.dylib 65656565656565656565656565656565`
=> This should yield editing _all_ slices' `LC_UUID` to `41414141-4141-4141-4141-414141414141`

You can double-check if it actually rewrote the UUID with `dwarfdump -u <dylib>` or `otool -hlv <dylib>`.

## Credits
- Apple for Mach-O and the respective headers I took their _(type-)_ defs from
	- _mach-o/loader.h, mach-o/fat.h, machine/machine.h_
- aidansteele for his wonderful [Mach-O format ref](https://github.com/aidansteele/osx-abi-macho-file-format-reference)
- Local pigeon gang for keeping the bad vibes away

## License
This project is dual-licensed as **AGPL v3 and APSL v2**.

