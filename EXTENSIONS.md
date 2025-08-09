# Extensions

This is the documentation about the custom extension format. A lot of things are
identical to normal Scratch extensions so for more detail, see
[TurboWarp's Extension Docs](https://docs.turbowarp.org/development/extensions/introduction).

## Glossary

- Extension folder: The extension folder is `sdcard:/3ds/extensions` on 3DS,
  `sdcard:/wiiu/scratch-wiiu/extension` on Wii U, and `./extensions` on PC.
- Extension library: The code or compiled library of a extension.

## Directory Structure

The directory structure is pretty simple, it's just a library inside of the
extensions folder. The library is named the id of the extension.

## Extension Library

The extension library is simply a ton of functions each representing a block.
The functions returns `void`, `bool`, or `int` and their only argument is
`std::map<std::string, std::any> arguments` or if they need to return a string
they accept a `std::string*` which the string is put into.

Along with the library there should also be a json file with the same name which
contains the types of each function. See the
[Scratch 3DS TurboWarp Extension Repository](https://github.com/gradylink/turbowarp-3ds-extensions)
for examples.

## Limitations

The current incomplete system currently has a ton of limitations, here are all
of them:

- No access to sprite data
- No way to override anything
- No way to make terminal blocks
- No way to do custom rendering
- No way to do anything on every tick

## Examples

See the
[Scratch 3DS TurboWarp Extension Repository](https://github.com/gradylink/turbowarp-3ds-extensions)
for examples.
