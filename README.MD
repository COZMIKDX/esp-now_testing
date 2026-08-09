Just testing out esp-now and trying to get it working between esp32 and esp8266.
I want to figure out how to properly structure a project such as this with multiple boards.
The main differences in this case are the two chips.

I think I'll make folders for each board and give each board their own env in the plaformio.env file.
I'll specify the folders that aren't for that env to be ignored by the compiler.
I'll make some files for the esp-now stuff like the packet structs and callbacks in the the lib folder.
For those, I think I will use #ifdefs and custom build flags to to let me put multiple chip's specific code in one file, instead of making multiple for each chip.
This should only depend on the chip and not the board.