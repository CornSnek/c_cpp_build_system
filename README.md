C CPP BUILD SYSTEM
=
I made a build system using GNU make and shell scripts for personal hobby programming uses. The purpose of this was to create a build system that uses separate c/cpp flags for different small projects in a source folder (`src/`) by using just one general makefile (`Makefile_build`). There is also a `shared_src/` directory where it acts like a "copy and paste" folder to add .o files for any projects in (`src/`).

One example for `src` or `shared_src` usage is to build one projects but with different purposes. Ex: One for release, and one for debug. The only difference is that the release and debug folders have different c or cpp flags. Any `shared_src` .o files are compiled separately for a release and debug folder.

There are example .c and .h code that uses different `build.conf` files, and two of the projects are using a folder in the `shared_include` and `shared_src` directory. One binary requires [libsodium](https://doc.libsodium.org/) to work (`src/random_hex`).

#####Usage

Copy build.conf into a `src/(project_name)` file. Change the flags for the Makefile_build to parse. Note that the folders that are auto included for a project_name are `-Iinclude/(project_name)` only. Any shared_src names added in the build.conf file auto includes the directory `-Ishared_include/(shared_name)` for each shared_name. 

This project was built using Visual Studio Code, but it is optional for the build system to work. To build the binary, use a shell terminal using `make.sh` or source tasks.conf. The binary output is in the `bin/(project_name)` folder.

#####Limitations

In the build folder, any src .o files are in the directory `build/(project_name)`, and any shared_src .o files are in the directory `build/(project_name)/shared_src/(shared_name)`. Do not name any files or folders as `shared_src`, or there might be makefile problems.

If using Visual Studio Code and the C/C++ extension, the `c_cpp_properties.json` needs to be manually edited for the "includePath", "compilerArgs", "defines"... etc. for the C/C++ extension to work properly.
Update August 12, 2021: For Visual Studio Code, Added `change_includes.sh` that can add a project name's flags and includes to the `.vscode/c_cpp_properties.json` automatically, and makes a backup file to `.vscode/c_cpp_properties.json.old`. This requires the [jq library](https://stedolan.github.io/jq/). It only rewrites "includePath", "compilerArgs", and "defines". Other keys may need to be manually configured.
Usage: `./change_includes.sh` (project_name in src) (Platform such as Linux,Win32,or Mac). This has been only tested for Linux.