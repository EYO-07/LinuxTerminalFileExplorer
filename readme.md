# Linux Terminal File Explorer 

This project is just a personal attempt to create a functional interactive file explorer for easy and safe usability and with fast compilation. There are other well established projects similar to this project, for example: **nnn**.

This project is small, limited and simple. And always will be. It's just a interactive mode for terminals in Linux. There will be no fancy stuffs on this project.

## Build 

Read the file `build.sh`, it's a simple bash script using gcc toolchain to compile the program. Run it with `bash build.sh` and execute the generated binary with `./terminalFileExplorer`. 

## Usage

To display usage help use `./terminalFileExplorer --help`. The scope of this program is limited for safety use, this program don't copy, delete or move files/folders directly on interactive mode, instead it generates a bash script inside the starting folder. Check it with `ls` and `cat SCRIPT` command and run it with `bash SCRIPT`. The current directory will be the destination diretory for copy and move operations.

1. Navigate with **i,j,k,l**
2. Fast navigation with **h,y**
3. Update current list with **u**
4. Select/Deselect with **s,d,f**
5. Copy/Move/Delete with **c,m,x** 
6. To open a new terminal emulator on current location use **t** (don't work on tty)

## Installation 

What did you mean by installation? If you just want to access it by it's name `terminalFileExplorer`, just copy on any binary folder accessible by system path variable `echo $PATH`. For example, copy to `~/.local/bin/` folder. Alternatively, you can just create an alias pointing to:

```bash
# add this to .bashrc or .bash_aliases
alias terminalFileExplorer='PATH_TO_BUILD_LOCATION/terminalFileExplorer --terminal=xterm'
```
