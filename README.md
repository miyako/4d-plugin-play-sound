![version](https://img.shields.io/badge/version-17%2B-3E8B93)
![platform](https://img.shields.io/static/v1?label=platform&message=mac-intel%20|%20mac-arm%20|%20win-32%20|%20win-64&color=blue)
[![license](https://img.shields.io/github/license/miyako/4d-plugin-play-sound)](LICENSE)
![downloads](https://img.shields.io/github/downloads/miyako/4d-plugin-play-sound/total)

# 4d-plugin-play-sound

Plays a sound file through the OS: on Windows the legacy [`WinMM`](https://docs.microsoft.com/en-us/previous-versions/dd743680(v=vs.85)) API is driven via [`mciSendString`](https://docs.microsoft.com/en-us/previous-versions/dd757161(v=vs.85)), on macOS [`NSSound`](https://developer.apple.com/documentation/appkit/nssound?language=objc) is called. The plugin exposes a single command, `process sound file`, whose second parameter selects the action (open, play, pause, resume, stop, close, plus two macOS-only queries). Sound files are addressed by their full path, which doubles as the alias the plugin keeps internally.

| Command | Returns | Purpose |
| --- | --- | --- |
| [process sound file](#process-sound-file) | Text | Open, control, and query a sound file identified by its path |

**Platforms:** macOS, Windows

---

## Requirements & platform notes

* The plugin is built as a universal binary for macOS 11.5 or later (Intel and Apple silicon) and for 32/64-bit Windows; `mciSendString`/`WinMM` is available on every supported Windows version.
* The badge targets 4D 17+; the manifest declares the command `threadSafe`, so it may be called from worker processes, not only the main process.
* The command **never raises a 4D error**: failures are reported through the return value on Windows and silently on macOS. Read the [Error handling](#error-handling--troubleshooting) section before relying on it.
* The six "Sound File Action" constants (`sound action open`…`sound action resume`) and the two query selectors (`sound duration`, `sound is playing`) are shipped in `constants.xlf` under the **Sound File Action** theme; `$2` is a plain Longint, so you can also pass the values `0`–`7` directly.
* Platform parity should not be assumed: the return value, the behavior for unknown aliases, and two of the eight selectors differ between macOS and Windows (detailed per selector below).

---

## process sound file

### Syntax

```4d
process sound file ( file ; action ) → info
```

### Parameters

| Parameter | Type | Description |
| --- | --- | --- |
| `file` | Text | `$1`, full path of the sound file to act on. The path is the alias: the same path identifies the same sound until it is `close`d. |
| `action` | Longint | `$2`, one of the selectors below. On macOS a path whose file was never `open`ed is silently ignored; on Windows the same case returns an MCI error text. |
| `info` | Text | `Result`. **On Windows** the MCI error text when the underlying MCI command failed, otherwise empty — success is an empty string. **On macOS** always empty for `open`/`pause`/`play`/`stop`/`close`/`resume`, a seconds value for `sound duration`, `"YES"`/`"NO"` for `sound is playing`. |

`action` selectors (theme **Sound File Action**, from `constants.xlf` and the implementation in `4DPlugin-Sound.cpp`):

| Constant | Value | macOS (`NSSound`) | Windows (MCI) |
| --- | --- | --- | --- |
| `sound action open` | 0 | Loads the sound and registers the path as an alias; a previous alias for the same path is stopped and replaced. | `open "path" alias "path" wait` — opens an MCI device; the path is used as the alias. |
| `sound action pause` | 1 | Pauses the sound. | `pause "path" wait`. |
| `sound action play` | 2 | Plays the sound. | `play "path"`. |
| `sound action stop` | 3 | Stops playback (position is not kept). | `stop "path" wait`. |
| `sound action close` | 4 | Stops and releases the sound and forgets the alias. | `close "path" wait`. |
| `sound action resume` | 5 | Resumes a paused sound. | `resume "path" wait`. |
| `sound duration` | 6 | Returns the duration of the sound in seconds. **Not implemented on Windows** (returns an MCI error text — see below). | — |
| `sound is playing` | 7 | Returns `"YES"` or `"NO"`. **Not implemented on Windows** (returns an MCI error text — see below). | — |

### Description

`process sound file` is a thin pass-through to the OS player: it does not decode, pre-load, or cache audio itself, and it plays one sound per alias. It supports the formats each OS API accepts — on macOS, whatever `NSSound` recognizes (AIFF, WAV, MP3, CAF, and others), on Windows whatever the installed MCI audio device can play. There is no volume, looping, or completion-notification mechanism exposed; use the plugin for straightforward play/pause/resume/stop workflows.

**On macOS**, `NSSound` is stored under the file path as its key. `open` replaces a previously opened alias of the same path (the old sound is stopped first); `close` stops the sound and removes the alias. Acting on an alias that has not been `open`ed — including the `sound duration` and `sound is playing` queries — is a silent no-op that returns an empty string. `sound duration` returns the value of [`NSSound duration`](https://developer.apple.com/documentation/appkit/nssound/1477313-duration?language=objc) as text; for formats whose length NSSound cannot determine this may be `0` or a negative value, not a 4D error.

**On Windows**, every selector is translated to an MCI command string sent with `mciSendString`. The return value is the MCI error text whenever the command fails (for example when the file was never opened, the file is already open, or the format has no available MCI device); success returns an empty string — the comment in the test method "info is returned only on windows" refers to this. `sound duration` and `sound is playing` are **not implemented** on Windows: there is no MCI command string for them, so the plugin sends an empty command and the MCI error text is returned; do not use selectors 6 and 7 on Windows.

### Example

From the plugin's own test method (`TEST.4dm`):

```4d
//%attributes = {}
$file:=Get 4D folder:C485(Current resources folder:K5:16)+"sample.mp3"

$info:=process sound file ($file;sound action open)  //info is returned only on windows

$info:=process sound file ($file;sound action play)

$info:=process sound file ($file;sound action pause)

$info:=process sound file ($file;sound action resume)

If (Is macOS:C1572)
	
	
	Repeat 
		
	Until (process sound file ($file;sound playing))
	
	
End if 

$info:=process sound file ($file;sound action stop)

$info:=process sound file ($file;sound action close)
```

A Windows-flavored variation, checking the returned error text after each MCI command:

```4d
$file:=Get 4D folder:C485(Current resources folder:K5:16)+"sample.mp3"
$info:=process sound file ($file;sound action open)
If ($info#"")
	ALERT("MCI: "+$info)
End if 
$info:=process sound file ($file;sound action play)
```

---

## Error handling & troubleshooting

* **Failure is silent on macOS.** An unopenable file, an unknown alias, or an unsupported format returns an empty string with no error; the only detectable state is through `sound is playing` and `sound duration`, which also return empty for unknown aliases.
* **On Windows, success and failure are distinguished by the return value.** Empty means the MCI command succeeded; any non-empty result is the MCI error text. Ignore the returned text at your own risk — it is the only failure signal the Windows path produces.
* **Selectors 6 and 7 are macOS-only.** `sound duration` and `sound is playing` have no MCI implementation; on Windows they send an empty command and surface an MCI error text. Branch on `Is macOS:C1572` as the test method does before using them.
* **An alias must be opened before it can be controlled.** macOS silently ignores the other actions; Windows returns an MCI error text. If you reuse a path, `open` it again (macOS replaces the previous alias) or `close` it first.
* **No volume control or completion event.** The plugin exposes neither; for recording, volume, or richer playback consider [4d-plugin-sfml-audio](https://github.com/miyako/4d-plugin-sfml-audio).

---

## Quick reference

```4d
// open and play a sound file (path doubles as the alias)
$info:=process sound file ($file;sound action open)
$info:=process sound file ($file;sound action play)

// pause and resume, then stop and release
$info:=process sound file ($file;sound action pause)
$info:=process sound file ($file;sound action resume)
$info:=process sound file ($file;sound action stop)
$info:=process sound file ($file;sound action close)

// macOS only: wait until playback ends
Repeat 
	Until (process sound file ($file;sound is playing))
```