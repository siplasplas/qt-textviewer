# qt-textviewer
Text viewer especially for huge files

git clone --recurse-submodules https://github.com/siplasplas/qt-textviewer.git

Twin panel file managers have built in file viewers. Total Commander for Windows and
Double Commander for Linux have text viewer launched by key F3.
Unlike editors, viewed file is not loaded whole to memory, instead only viewed pieces.
Viewed files can be huge: hundreds of gigabytes of dumped databases.<br>
Text viewer differs from hex viewer in that hex viewer has constant, known positions to proceed 
goto any place of file, whereas text file has lines from one byte char LF to thousands chars in one line,
even whole multi-gigabyte file can be one line.<br>
Core of viewer is non-visual class ViewerLogic, which is tested with Google Test. This library
is installed in Mint with command sudo apt install googletest.
It uses simple Unicode library, from https://github.com/siplasplas/UTF , is needed to 
clone recursive with sub-repositories.

Features that the class will have:
- go to any place of file
- position in file is 64 bit
- file can have empty lines
- empty lines can be at start and at end, chars LF or CRLF on end of file are
treated specially: single break char not caused empty lines, except single break-line
at file end
- lines are searched forward and backward
- very long line are forced to break, in some situation whole file (hundreds of
gigabytes) can be one line; it would enforce read too much disk and slow reading;
very long lines are wrapped
- lines can be optionally wrapped to screen width
- wrap optionally can preserve whole words
- file is default treated as is in UTF-8 format; utf8 codes are varabla length, from 1 to 4
(in theory even 6) bytes, and 1 or 2 words od wide-string
- tabulation \t is changed to spaces , but variable length spaces, from 1 to max,
tab spaces intended to equal place of screen
- beside goto, scrolling to small distances: one line down and up, one page down and up 
