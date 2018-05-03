# READ ME
## Steps:
```
1.create bin and src in the project folder
2.Extract the Iron_data.cxx and cMakeLists.txt files into src folder
3.Open cMake and give "where is source code" as the location of the src folder and "where to build the binaries" as the location of the bin folder
4.Then configure and generate
5.Open the solution file(bin folder) in visual studio and build it (Here Iron_data.exe is created in the bin>debug folder)
6.Open command prompt and change directory to >debug directory of build
7.run Iron_data.exe ironprot.vtk(this argument is the location of the iron protein data file, arguments are passed to the program)
8.Now the renderer window opens and iso-surface, cross section, 2D contours are displayed.
9.Output coontains two sliders which are used to interact with the renderers, Iso-value can be set using the Iso value Slide and
Cross section displayed in second and third renderers can be set using the Cross section slider
```
