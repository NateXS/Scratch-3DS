

# Scratch Runtime for 3DS

A W.I.P. runtime made in C++ aimed to bring any Scratch 3 project over to the Nintendo 3DS.


![Software running a simple Scratch Project](https://raw.githubusercontent.com/NateXS/Scratch-3DS/refs/heads/main/scratchcats3ds.gif)

## Controls


![Controls](https://raw.githubusercontent.com/NateXS/Scratch-3DS/refs/heads/main/scratch%203ds%20controls.png)

To use the mouse pointer you must enter mouse mode by holding L and using the DPad to move the mouse. to click you press R while holding L.

## Limitations

As this is in a very W.I.P state, you will encounter many bugs, crashes, and things that will just not work. 

**List of known limitations:**
- Sound is not yet implemented.
- Sprites are always set to "all around" rotation.
- The "Touching" block may be a bit innacurate at times, due to using a more performant method of touching.
- Overall performance is currently quite poor when many scripts are run at once, so try to keep cloning and long looping scripts down to a minimum.
- - Performance was tested on a New 3DS, Old 3DS performance is probably worse
- There is no vector/svg sprite rendering. Images will only render if converted to bitmap beforehand, otherwise the image will show as a black square.
- Images will only work if it's in png or jpg format.
- Images cannot be over about 5mb cumulatively. if you go over this limit, some images may fail to load.
- Images cannot be over 1024x1024 in resolution.
- Extensions (eg: pen and music extensions) are not supported.
- many blocks are not yet implmented, and some implemented blocks has very little testing and may lead to crashing/unintended behavior.


## Unimplimented blocks
| Motion | Looks | Events | Sensing | Variables/Lists |
| :----- | :---- | :----- | :------ |:--------------- |
||All say and think blocks||Touching Color|Show/Hide variable|
||All Costume Effects|        |When this sprite clicked|Color is Touching Color|Show/Hide List|
||       |When backdrop switches to|| Cloud variables |
|        |       |When loudness > x||
|        |       |Broadcast and wait|Set Drag mode|
|        |       |        |Loudness|
|        |       |        ||
|        |       |        ||




## Get up and running the easy way

Download the 3dsx file in the Releases tab.

Place the 3dsx file in the `3ds/` folder of your 3DS SD card, along with the Scratch project you want to run.
- The Scratch project MUST be named `project.png` , all lowercase.
Then it should be as simple as opening the homebrew launcher on your 3DS and running the app.


## Building

In order to embed a Scratch project in the 3dsx executable, you'll need to compile the source code.

In order to build, you will need to have Devkitpro's SDKs installed with the DevkitARM toolchain and libctru.

- Devkitpro's install instructions are available at : https://devkitpro.org/wiki/Getting_Started

You will also need libvorbisidec installed. open a Terminal and run the following:
- `pacman -S 3ds-libvorbisidec 3ds-pkg-config`

Download the source code from the releases tab and unzip it.

Make a `romfs` folder inside the unzipped source code and put the Scratch project inside of that.
- The Scratch project MUST be named `project.png` , all lowercase.
- For faster load times/less limitations, you can also unzip the sb3 project file and put the contents into a new folder called `project`.
- If you would like to  change the name of the app, go inside the `Makefile` and edit
`APP_TITLE`, `APP_DESCRIPTION` and `APP_AUTHOR` to whatever you please.

Then it should be as simple as running `make` in the source code folder.
    
