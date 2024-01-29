# wol_win
A WOL magic packet sender made with C.

Made as a learning project; I strongly advise against the use of this program. There are many way better (and actually finished) alternatives already on GitHub.
It doesn't handle errors (yet) and will probably brick your computer if you do something stupid... Besides that, it does work! 😉

## How to compile:
Compile it on Windows using MinGW:
```
gcc .\wol.c .\settings.c -o wol.exe -l ws2_32
```

## How to use it:
~~you shouldn't~~

Saving a computer on the __settings.ini__ file:
```
.\wol.exe save [COMPUTER NAME] [MAC ADDRESS]
```
Waking a computer saved on the __settings.ini__ file:
```
.\wol.exe wake [COMPUTER NAME]
```
Waking a computer via it's MAC address:
```
.\wol.exe wake mac [MAC ADDRESS]
```
Changing the WOL port, the broadcast address and deleting computers:
- Can only be done manually for now. Edit the __settings.ini__ file with a text editor (and try not to delete any blank space, it can - and probably will - break stuff 😞).

## TO-DO:
- [ ] actual error handling;
- [ ] ~~editing the settings file from within the program (delete computers, change the broadcast address/WOL port);~~ (probably too dangerous to be implemented by a moron)
- [ ] refactor some bad code (most of the program really);
- [ ] try to find a better solution for searching the settings file;
- [ ] compiling for linux? (most important part)
