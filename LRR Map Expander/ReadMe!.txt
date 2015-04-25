====================================
== LEGO Rock Raiders Map Expander ==
====================================
==           Alcom Isst           ==
====================================

===== (1) Description =====

This program is designed to assist with developing and editing LEGO Rock Raiders levels. This program works with all 9 types of .map files found in the levels, along with the .ol file that comes with them. This program will expand the size of the level borders by a specified integer, which involves expanding each .map type and modifying the coordinates in .ol files.

This program comes with the option to use HeightMatch, an auxiliary function that will match the height levels of the border expansion with the rest of the map.

===== (2) How to use =====

1. Place the files for a single level into the Input folder.
2. Run LRRMapExpander.exe.
3. Enter integer to expand map borders by.
4. Enter whether or not you want to use HeightMatch.
5. Grab files from Output folder.

===== (3) Notes and Quirks =====

1. This program requires that there exists a Dugg.map file in the input folder to function. It will proceed if any other files it can read are missing.

2. This program currently will only read the following files:

	Dugg.map
	Cror.map
	Emrg.map
	Erod.map
	Fall.map
	High.map
	Path.map
	Surf.map
	Tuto.map
	ObjectList.ol

3. If you have a file listed above under a different name, the name of that file must be changed to match a name above for the program to read it.