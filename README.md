Danger! work in progress by C-n00b!

# Magic Tone Network for Launchpad Pro
*A [custom firmware for Launchpad Pro](https://github.com/dvhdr/launchpad-pro)*

Not yet made famous by allround nice guy Andreas from [Xpresspads](http://xpresspads.com/) fingerdrumming: the [Magic Tone Network](http://www.xpresspads.com/tag/magic-tone-network/). It is a non linear layout for pad controllers, based on music theory. It helps to find interesting chords and might be a better alternative to regular pad layouts, even scaled ones.

## Installation
* turn on your launchpad pro holding the setup key
* upload the .syx file in the build directory with your favorite sysex tool
* done

## Usage
*Press a key and play a little melody.*
* All keys (except setup) play a note in the network.
* The round buttons on the launchpad are not velocity sensitive and don't send aftertouch. I set the default velocity for those keys to 40, which seems better to me than 127. 
* The pressed key and the other keys that play the same note light up white.
* the bottom leftmost pad is C1.
* See information about the range in [this spreadsheet](https://docs.google.com/spreadsheets/d/e/2PACX-1vT2yyNmcxbW1oj1S9KBUE1fmY2ACqy414S8aB-S-v0M3DpBvpG-V4ByjrNyFdDpDgFWv8H7D7-QWCiU/pubhtml)
* The pink pads indicate octaves for F#.
* the turqoise pads indicate octaves for B.
* alternating columns are darkened slightly in blue, to indicate the inversions.
* Velocity and aftertouch are transmitted as is, which means that there are no settings and that aftertouch starts immediately vs. after a little press in the [original firmware](https://github.com/dvhdr/launchpad-pro)

## Plans
* make this respond to [Synthesia](http://synthesiagame.com/) so I can learn to play the songs I already know on this layout
* add settings for transpose and octave up-down (in setup)
* stretch goal: scale highlighting, with scales from Novation Circuit
* add controls for velocity and aftertouch sensitivity, 
* stretch goal: add velocity calibration per pad
* add a way to use one pad for modulation via aftertouch
* add pitch bend, implemented similarly to OP-1
* stretch goal: add more refined pitch bend, with pressure and velocity response
* make the color scheme nicer
* stretch goal: customizable colors
* ultra stretch goal: clean up code

## Modifying and building
* I used docker so:
`docker build -t novation-launchpad-pro-dev .`
`docker run -it -v $(pwd):/launchpad-pro novation-launchpad-pro-dev`
`make`
* everything I did is in app.c
* No, I don't know what I'm doing half the time
* for more info: see [Novation LPP open firmware on github](https://github.com/dvhdr/launchpad-pro)
