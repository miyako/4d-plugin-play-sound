![version](https://img.shields.io/badge/version-17%2B-3E8B93)
![platform](https://img.shields.io/static/v1?label=platform&message=mac-intel%20|%20mac-arm%20|%20win-32%20|%20win-64&color=blue)
[![license](https://img.shields.io/github/license/miyako/4d-plugin-play-sound)](LICENSE)
![downloads](https://img.shields.io/github/downloads/miyako/4d-plugin-play-sound/total)

# 4d-plugin-play-sound

On Windows, legacy [`WinMM`](https://docs.microsoft.com/en-us/previous-versions/dd743680(v=vs.85)) is called via [`mciSendString`](https://docs.microsoft.com/en-us/previous-versions/dd757161(v=vs.85)). On Mac, [`NSSound`](https://developer.apple.com/documentation/appkit/nssound?language=objc) is called.

Supported actions on Windows:

* [open](https://docs.microsoft.com/ja-jp/windows/win32/multimedia/open)
* [pause](https://docs.microsoft.com/ja-jp/windows/win32/multimedia/pause)
* [play](https://docs.microsoft.com/ja-jp/windows/win32/multimedia/play)
* [stop](https://docs.microsoft.com/ja-jp/windows/win32/multimedia/stop)
* [close](https://docs.microsoft.com/ja-jp/windows/win32/multimedia/close)
* [resume](https://docs.microsoft.com/ja-jp/windows/win32/multimedia/resume)

Extra selectors supported on Mac:

* [duration](https://developer.apple.com/documentation/appkit/nssound/1477313-duration?language=objc)
* [playing](https://developer.apple.com/documentation/appkit/nssound/1477302-playing?language=objc)

---

For recording and playback, consider [4d-plugin-sfml-audio](https://github.com/miyako/4d-plugin-sfml-audio).
