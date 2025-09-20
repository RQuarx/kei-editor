<div align=center>

# Kei Editor

Kei Editor or Keditor for short, is a text/code editor made with C++23 and SDL3.

</div>


## Installation

### Prerequisites

- C++23 compatible compiler (e.g., GCC 12+, Clang 14+, MSVC 2022+)
- SDL3 and SDL3_TTF development libraries
- Meson 1.8.0+

### Build from source

```bash
git clone https://github.com/RQuarx/kei-editor
cd kei-editor
meson setup target -Dbuildtype=release
meson compile -C target

./target/keditor
```

## Attribution

This project uses icons from the [vscode-icons](https://github.com/vscode-icons/vscode-icons) project,  
licensed under the [Creative Commons Attribution-ShareAlike 4.0 International License](https://creativecommons.org/licenses/by-sa/4.0/).

© vscode-icons team and contributors.

## Disclaimer

The vscode-icons project and its contributors are **not affiliated with, nor do they endorse or sponsor, this project**.  
All product names, trademarks, and registered trademarks are property of their respective owners.

## License

This program is licensed under the GNU General Public License version 3. See the
[LICENSE](LICENSE).