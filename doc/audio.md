
# How to play noise on audio device

Play noise on default audio device:
```bash
$ gst-launch-1.0 audiotestsrc ! audioconvert ! autoaudiosink
```

Play noise on specific audio device:
```bash
$ aplay -l
 card 0: b1 [bcm2835 HDMI 1], device 0: bcm2835 HDMI 1 [bcm2835 HDMI 1]
 ...
 card 1: Headphones [bcm2835 Headphones], device 0: bcm2835 Headphones [bcm2835 Headphones]
 ...
 card 2: Device [USB PnP Audio Device], device 0: USB Audio [USB Audio]
$ gst-launch-1.0 audiotestsrc ! audioconvert ! alsasink device="hw:2"
```
Where "hw:2" - means playing on card `card 2`.

# How to set certain audio device as default audio device

```bash
$ cat /proc/asound/cards
 0 [b1             ]: bcm2835_hdmi - bcm2835 HDMI 1
                      bcm2835 HDMI 1
 1 [Headphones     ]: bcm2835_headpho - bcm2835 Headphones
                      bcm2835 Headphones
 2 [Device         ]: USB-Audio - USB PnP Audio Device
                      Solid State System Co.,Ltd. USB PnP Audio Device at usb-3f980000.usb-1.2, full
$ vi /etc/asound.conf
defaults.pcm.card 2
defaults.ctl.card 2
```
Where `2` means setting the last audio card as default audio device. 

