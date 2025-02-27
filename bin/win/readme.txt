MConvert

... converts Ogre .mesh to .raw, .opc, .ntt.

Motivation: I created MConvert because there are several exporters for different 3D Modelling tools that create .mesh files for Ogre. Some apply modifications to the models while doing so like scaling or flipping axis. Therefore, to make sure the physics collisions are equivalent to the Ogre meshes, I wanted to create the collisions from the .mesh file, and that's what MConvert does for me right now.

.raw is a simple text format that consists of one line per triangle. For each triangle, the 9 coordinates of the 3 vertices are inserted.

.opc is a binary format, consisting of an array of vertices and an array of triangles referencing the vertices. This was meant for loading into an Opcode collision at runtime.

.ntt is a binary format, the idea is equivalent to .opc, but instead for opcode, it's targeted towards Newton Game Dynamics. The created file is exactly what Newton returns when asked to serialize a collision. So you can easily recreate the NewtonCollison by loading an .ntt file and passing it to Newton's Deserialization Function.

Just start MConvert from the cmdline. It displays the help.

Note that the workdir has to be the directory where MConvert lies in - always start it from the directory where it lies in. Otherwise, Ogre won't find its plugins and MConvert will crash. You can specify input and output file with absolute or relative paths from there.

Also note that when using it for .ntt files, the format of newtons collision serialization is not supposed to stay unchanged over different versions. So always compile it against the version of newton you are using and use that version then. The included binary version is based on newton 2.08. Since the api hasn't changed to 2.10 (the current version while writing this text), it probably also works to just replace the included newton.dll with the new version of the newton.dll.


M. Rochel