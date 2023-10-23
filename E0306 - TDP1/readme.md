# Get Started
Download software and firmware last releases from its GitHub repositories.

* Software: https://github.com/epernia/software
* Firmware: https://github.com/epernia/firmware_v3

Open GNU Eclipse and choose an empty workspace. Import firmware from File > New > Makefile Project with Existing Code. Then, try to compile the project.

# Build Configuration
Go to Run > Debug Configurations > GDB OpenOCD Debugging > New launch configuration

In Main tab, select Firmware_v3 as Project and the ELF file as C/C++ Application. Also change "Use Active" to "Select Automatically" in Build Configuration.

In Debugger tab, find openocd.exe on tools\openocd\bin folder for the Executable Path, and set ```-f scripts/openocd/lpc4337.cfg``` in Config Options. Then below in GDB Client Setup, find arm-none-eabi-gdb.exe on tools\arm-none-eabi-gcc\bin folder for the Executable name.

# Select program
Modify program.mk file from project root folder. If that file doesn't exists, you can modify the Makefile as it follows:

![image](https://github.com/sergiocarp10/unlp-tramo-final/assets/66924320/c80a27c6-78e0-4e6a-b980-b275aa8621ec)

Then, build the project (hammer icon) and go to Run > Debug Configurations > C/C++ Application > Browse, and select the .elf file target

# Hide errors
Go to Project > Properties > C/C++ General > Code Analysis > Syntax and Semantic Errors. Untick the "Symbol is not resolved" option

# Troubleshoot
C compilers PATH variables are not mandatory to compile the project.

![image](https://github.com/sergiocarp10/unlp-tramo-final/assets/66924320/3975d30b-23eb-4abd-9ac2-f771667977fd)
