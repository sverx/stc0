# Simple Tile Compression Zero

## BMP2Tile plugin and Z80 decompression routine

The Simple Tile Compression Zero compression algorithm is a very simple tile compression algorithm that exploits the fact that SEGA Master System mode-4 tile data typically includes lots of bytes having either value 0x00 or 0xFF. It works on groups of 4 bytes (8 groups are needed for each tile) and in a best case scenario it compresses them up to 75%, that's when a whole group of 4 uncompressed bytes turns into a single compressed byte. Beside that, it compresses sequences of same bytes in the group too. (Average compression is around 30%, which is comparable to other known compression algorithms such as "Sonic 1", "Phantasy Star" and "LZ4", according to Maxim's compressor/decompressor benchmark https://github.com/maxim-zhao/bmp2tilecompressors#benchmark )

The provided Z80 decompression routine has the following features:
- Master System VRAM write timing and IRQ safe
- the fastest decompressor available (writes approximately 1000 bytes to VRAM each frame on average - see the abovementioned benchmark)
- needs no RAM
- it's very small (under 100 bytes)
- permissive license

