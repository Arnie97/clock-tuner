# Clock Tuner
This tool is developed to view and modify the main clock frequency of
ARM-based HP 39 series calculators (HP 39g+ / 39gs / 40gs).
These models run a S3C2410X processor at 75 MHz to emulate the legacy
Saturn 'Yorke' architecture at a relatively high speed.

Some games developed for the original HP 39 depend on CPU ticks for timing,
so they would appear to be too fast for players using newer models.
With this tool, you are able to slow down the ARM processor, and get a
reasonable speed for those old school SysRPL games.

## Features
- [x] Installs on a vanilla 39gs with no hardware hacks
- [x] Packed as an aplet, and provides a simple user interface
- [x] Well tested on my device
- [x] Free as in speech and as in beer

## FAQ
##### Is it safe to modify the main clock frequency? Will it brick my calculator?
- All frequency options are cautiously selected from the S3C2410X datasheet,
hence won't cause any hardware damage to your device.
- However, the LCD display and other clock-sensitive components are likely to
encounter problems if an inappropriate clock is selected. Please read the
following answer to choose a proper frequency.
- It's also recommended to backup your data before operations.

##### Can I use this tool to overclock my device?
- Yes, of course. ***However, it is recommended to limit your clock between
10 MHz and 110 MHz.***
- Clocks out of this range may also work inside this aplet, but could make
your system and aplets crash, reset or get stuck.
- ***Clocks higher than 200 MHz are highly deprecated and only preserved
for experimental usage.***
- If you've already triggered a system crash, the next answer might be helpful.

##### How to restore my clock settings to factory default?
- The calculator 'OS' or whatever will restore the clock to factory default
(75 MHz) during a reboot, either a normal one or a forced one.
- ***If you got into trouble after changing your clock, reboot by pressing
the reset hole should be the best solution in most cases.***

##### My calculator restarts or halts when opening this aplet.
- Please upgrade your ROM to 2.22 or any later versions, and make sure you
delete this aplet and transfer it again after your ROM upgrade.

##### It says `Undefined XLIB Name` when trying to open this aplet.
- Go to the `MEMORY MANAGER` and check whether the ARM ToolBox
(i.e. Library 275, or L275 for short) is installed in your libraries.
- If not, please follow the [install instructions](#install).

## Prerequisites
You need the following packages before building clock tuner:

- GNU Make
- An toolchain for the `arm-elf` ABI
- `minstub39`, to combine ARM native code with SysRPL-based aplet skeleton
- `hplib`, which is included in HP-GCC v1.1
- `elf2hp`, also included in HP-GCC distributions

## Build
Clone the source code from GitHub, and `$ make believe`.

## Install
Use the official connectivity kit (conn3x) to transfer it into your calculator.
The 'HP 39/40' methods are much faster than the 'Disk drive' methods, but
won't help you install library dependencies, so you should transfer both
`clock_tuner.apt` and `lib275.sys` to your calculator manually.

## Contributions
[Stars](https://github.com/Arnie97/clock-tuner/stargazers), [pull requests](https://github.com/Arnie97/clock-tuner/pulls) and [bug reports](https://github.com/Arnie97/clock-tuner/issues) are welcome.

## License
Clock Tuner is distributed under the GPLv2 license.
See [`LICENSE`](LICENSE) for details.
