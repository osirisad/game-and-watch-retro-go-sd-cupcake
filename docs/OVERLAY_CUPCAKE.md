# Cupcake Crisis overlay

Cupcake uses the Homebrew overlay slot at `__RAM_EMU_START__` (`0x2404B000`), section `.overlay_cupcake`, with a RAM budget of `__RAM_EMU_LENGTH__` (~724 KiB on the SD linker layout). The full game is **not** linked into firmware; users install `cupcake.bin` and `cupcake_assets.dat` from the port repo on SD (same model as PICO-8 / `flash_sd` overlays).

## In-tree sources

| Path | Role |
|------|------|
| `Core/Src/porting/cupcake/cupcake_entry.c` | Entry trampoline at overlay offset 0 (`.cupcake_entry`) |
| `Core/Src/porting/cupcake/main_cupcake.c` | Smoke-test stub — reserves linker RAM budget only |
| `Core/Inc/porting/cupcake/main_cupcake.h` | Stub entry declaration |
| `Core/Src/retro-go/rg_emulators.c` | Homebrew loader dispatch for `"cupcake"` |
| `STM32H7B0VBTx_FLASH.ld` / `STM32H7B0VBTx_SDCARD.ld` | `.overlay_cupcake` / `.overlay_cupcake_bss` sections |
| `Core/Inc/gw_linker.h` | `_OVERLAY_CUPCAKE_*` linker symbols |
| `Makefile` | `CUPCAKE_C_SOURCES`, extflash section list |
| `Makefile.common` | Include path, object rules, link deps |

## Build

```bash
make
# or: make DOCKER=1 release
```

After linking, check overlay usage:

```bash
scripts/size.sh   # ram_emu_cupcake vs __RAM_EMU_LENGTH__
```

## Runtime dispatch

When Homebrew launches a file whose stem is `cupcake`:

1. `odroid_overlay_cache_file_in_ram()` loads `/roms/homebrew/cupcake.bin` into `__RAM_EMU_START__`.
2. Firmware zeroes BSS from end of the loaded image through `__RAM_EMU_END__`.
3. Caches are cleaned/invalidated; execution jumps to **offset 0** via Thumb trampoline (`__RAM_EMU_START__ | 1`).

The SD-loaded bin provides its own `.cupcake_entry` and `app_main_cupcake` — firmware does not call the in-tree stub at runtime when a port build is present.

## Port repo (external build)

The Bart Simpson port repo builds the release overlay independently (`make cupcake-bin`). Firmware only needs a rebuild when the overlay ABI or RAM layout changes.

| Port artifact | SD path |
|---------------|---------|
| `release/cupcake.bin` | `/roms/homebrew/cupcake.bin` |
| `release/cupcake_assets.dat` | `/roms/homebrew/cupcake_assets.dat` |

Only `.bin` stems appear in the Homebrew menu; the `.dat` file is opened at runtime by the game.
