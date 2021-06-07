Simple wheel to wheel translator.
Takes the ring device of a HuionHS610 tablet and makes uses https://gitlab.com/interception/linux/tools  magic to map those events to a fake mouse that only scrolls.

Had to do it myself as unfortunately the libwacom driver stooped supporting rings of 3rd parties tablets. Thanks a lot to the interception tools guys and to https://github.com/DIGImend/digimend-kernel-drivers/  for doing the harder part.


To use it just setup `interception tools` and then run

`./Taskfile build` # To build the program

`./Taskfile install` # To install it and add the configurations. This one will ask for sudo permissions


