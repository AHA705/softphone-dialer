# softphone-dialer
[Softphone App link](https://play.google.com/store/apps/details?id=co.uk.voicehost.softphone.android&hl=en-US)

Tested to work on Linux Fedora 40.

[USB Debugging](https://www.lifewire.com/enable-usb-debugging-android-4690927)
In developer options scroll down and find pointer location, you'll need to find the x,y of the text box and the dialer. you'll be asked for it in the setup checks. on Pixel 7 the coords are:
TAP1_X=558
TAP1_Y=500
TAP2_X=540
TAP2_Y=2200

Run setup for first time then run CLI Interface.

Program function: Every time you press "Enter"
* It'll take your clipboard and remove the whitespaces
* Tap and Type it into the number area
* Dial the number and tell you the last 4 digits.

Open terminal, cd into the directory:

./setup
./CLI_Interface
