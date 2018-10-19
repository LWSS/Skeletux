<p align="center">
<img src="http://i.imgur.com/mCtrbIN.png">
</p>
===================

## What is Skeletux?

Skeletux is an experimental base for making your own CS:GO cheat on Linux.

It is mostly aimed towards developers.


## Features
* Fully dynamic runtime created Panorama GUI that does not touch any files and is even removed on unloading.
* Custom ConVar settings
* A few simple features ( NoFog, No Post Processing, Custom SkyBoxes )


#### Dependencies:
* A Compiler that supports C++17
* CMake  >= 2.8
* In addition to the standard C++ packages you probably have, you'll need the  v8-development package


===================

#### How to Use

Build like any cmake project
```bash
cmake . && make 

```

Afterwards you can inject with the load script
```bash
sudo ./load
```

You can open/close the Menu with `Insert`

## Screenshots

This looks like garbage lol
![menu](https://i.imgur.com/lATYWiH.jpg)


## Credits

This project is essentially a trimmed down version of Fuzion: [https://github.com/LWSS/Fuzion](https://github.com/LWSS/Fuzion)
