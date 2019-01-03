# MUConvert
Converts Yuzo Koshiro's MUCOM88 MUC Windows files for PC-8801 use

# Background
Yuzo Koshiro released the tools he used to create FM music with the YM2608.

Not only did he release the tools for PC-8801, but also windows executables. 

At the same time he posted sample tracks for Bare Knuckle (Streets of Rage), Actraiser, Super Shinobi, Slap Fight, The Scheme and Etrian Odissey.

# This tool

The purpose of this tools is to format the MUC files for use in the PC-8801 d88 image disks and in an emulator or the original PC-8801. Since MUC files are easily released for Windows this simply adds the BASIC line numbers, removes unwanted characters and removes comments if requested.

It also does teh reverse process, when extracting a BASIC file from the PC-8801 disks, it can remove line numbers for use in the MUCOM88 windows version.

# Usage 

Files can be inserted and removed from and to d88 image disks with the M88 emulator and the dskdrv02a plug in as described in http://fullmotionvideo.free.fr/phpBB3/viewtopic.php?p=16621#p16621

Before uploading, you can convert the file with:

MUConvert -pc88 <source> <target> [-withcomments]

The [-withcomments] parameter is optional, and it retains the comments marked with ; in the MUC file.
The reverse is also possible, you can use:

MUConvert -muc <source> <target>


# Original Files
His post: https://twitter.com/yuzokoshiro/status/1075806760340869121
PC-8801 floppy downloads: https://www.ancient.co.jp/~mucom88/
Windows version: https://onitama.tv/mucom88/
Code at Github: https://github.com/onitama/mucom88

# Thanks

Special thanks to Yuzo Koshiro for releasing these files, they are awesome.
