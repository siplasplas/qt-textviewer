# qt-textviewer
Text viewer especially for huge files

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


