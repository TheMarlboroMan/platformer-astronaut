# platformer-astronaut

And old thing I did way back then. It's really nothing. I am pretty much sure it did have the ability to launch a spear and climb on it (Volgarr style... Only I don't think I knew of Volgarr) but I can't find it.

Sometimes I think of revisiting these ideas.

## Building

This is built on top of a custom and very old libdansdl2 bundled with the repo. It seems it has also some misplaced dependencies of SDL1.2.

- Go into libdan2 and "make -f makefile_linux" (make sure you create the objects directory first)
- Copy the resulting libdansdl2.a file to the root of the project.
- "make all" in the root (make sure you create the objects directory first!)
- ./a.out

