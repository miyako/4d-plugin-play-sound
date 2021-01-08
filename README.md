![version](https://img.shields.io/badge/version-16%2B-8331AE)
![platform](https://img.shields.io/static/v1?label=platform&message=osx-64%20|%20win-32%20|%20win-64&color=blue)
[![license](https://img.shields.io/github/license/miyako/4d-plugin-play-sound)](LICENSE)
![downloads](https://img.shields.io/github/downloads/miyako/4d-plugin-play-sound/total)

# 4d-plugin-play-sound

To use on v16 or v17, move manifest.json to contents.

On Windows, [`WinMM`](https://docs.microsoft.com/en-us/previous-versions/dd743680(v=vs.85)) is called via [`mciSendString`](https://docs.microsoft.com/en-us/previous-versions/dd757161(v=vs.85)). It's a legacy API.

Supported actions on Windows:

* open
* pause
* play
* stop
* close
* resume

On Mac, [`NSSound`](https://developer.apple.com/documentation/appkit/nssound?language=objc) is called.
