# Inserter
Inserts and replaces an ASCII string into a binary file.
This program will scan the input binary file specified after -in, looking in there for the ASCII string specified after -find (256 bytes max) and replacing it with the ASCII string specified after -insert (256 bytes max).
Note that -insert string will overwrite -find pattern at the matching address.

Usage: inserter.exe -in <FILENAME> -out <FILENAME> -find <FINDPATTERN> -insert <INSERTPATTERN>
