# xtracepp
Proxy between X11 display server (or [Wayland] compositor via [Xwayland]) and client applications, logging their communication and selectively altering message data based on user options. Is to [X11 protocol] traffic what [strace] is to Linux syscalls. Complete rebuild of the original [xtrace] in [C++17], with expanded [features](#added-features).

### Developer Objectives
- improve personal understanding of [X11 protocol]
- revise [xtrace] output format
- add [features](#added-features) to [xtrace]

### Primary Use Cases
- recording protocol traffic for a given X11 server (or [Wayland] compositor via [Xwayland])
- debugging client communications with X11 server (or [Wayland] compositor via [Xwayland])
- selective filtering of X11 extension use by clients


## Usage

<details>
<summary><h3>Build From Source</h3></summary>

#### Requires

| | recommended | developed with |
|-|-------------|-------------|
| `git` | | 2.43.0 |
| `cmake` | 3.14+ | 3.28.3 |
| `g++`/`clang++` | 7+/4+ | 13.3.0/18.1.3 |
| `libstdc++`/`libc++` | 9+/7+ | 13.3.0/NA |
| `libxcb` (`XTRACEPP_BUILD_TESTS=ON`) | | 1.15 |
| `doxygen` (`XTRACEPP_BUILD_DOCS=ON`) | | 1.9.8 |
| `dot` (`graphviz`) (`XTRACEPP_BUILD_DOCS=ON`) | | 2.43.0 |

<!-- TBD libstd++-\<maj\>-dev? libxcb-dev? libxcb-\<extensions\>? -->

#### Clone
```bash
$ git clone https://github.com/allelomorph/xtracepp
```

#### Build
```bash
$ cd xtracepp
$ mkdir build_dir
$ cmake [-DXTRACEPP_BUILD_TESTS=ON] [-DXTRACEPP_BUILD_DOCS=ON] -S . -B build_dir
$ cmake --build build_dir
```
For documentation:
```bash
$ cmake --build build_dir --target doxygen
```
</details>

### Command Line

#### Display Names
On POSIX systems that support X11, the environmental variable `DISPLAY` should contain an X [display name] to help clients connect with the server. It can be in the documented format for TCP:
```
[[protocol/]hostname|URL]:display_number[.screen_number]
```
or a second undocumented format for unix sockets (supported by [libxcb]):
```
[unix:]socket_path[.screen_number]
 ```
For `xtracepp` to act as a proxy server and stand between the clients and real X server, it must take a [display name] to connect with the server (defaulting to `$DISPLAY` if not passed with `--display`,) and also choose a proxy [display name] to give to client apps (defaulting to `:9` if not passed with `--proxydisplay` or `$PROXYDISPLAY`.)

#### Trace Multiple Clients
Normally `xtracepp` closes when it has no open connections. But an easy way to use it to trace multiple clients is to first set it running as a background process:
```bash
$ xtracepp [options...] --keeprunning &
```
then launch as many X client apps as you like with a proxy `DISPLAY` name:
```bash
$ DISPLAY=:9 xclock
```

#### Single Client As Child Process
If you want to prioritize tracing of a particular client without bothering to set its `DISPLAY`, you can launch it and `xtracepp` in a single command simply by using `--` as a delimiter between the two commands. This launches the client as a child process on connection 0 (see [formatting](#formatting).)
```bash
$ xtracepp [options...] -- client_command [client args...]
```
This can be combined with the `--keeprunning` option above to stay listening for connections after the child exits.


## Output
`xtracepp` outputs:
- errors and notifications to `stderr`
- logs to `stdout` by default, or to file with `--outfile`(`-o`)` log_file`

<details>
<summary><h3>Formatting</h3></summary>

A typical log entry from `xtracepp` is one line for one protocol message, following a general format:
- prefix (uses `:` as delimiter)
  - `C` + connection number (three digits, zero padded)
  - message size in bytes (four digits, zero padded) + `B`
  - direction glyph
    - `s<c` client to server
    - `s>c` server to client
  - `S` + sequence number (five digits, zero padded)
  - description
    - category of message ([requests](#requests), [replies](#replies), [events](#events), [errors](#errors))
    - message name
    - message code(s)
- message data in mock JSON
  - `{` and `}` delimit structs
  - `[` and `]` delimit lists
  - struct members appear as `name=value`
    - `(brief description)=value` if data field is unnamed by protocol
    - `value` may be:
      - decimal integer
      - hexadecmial integer
      - enum name
      - list of bitmask flag names
      - quoted string
      - (other special data formatting)
  - formatting can be modified with options
    - [`--multiline`](#multiline-log-entries)
    - [`--verbose`](#variable-verbosity)
```
C000:0008B:s<c:S00001: Request GetWindowAttributes(3):  { window=0 }
```

#### [Connection Setup]
Messages during the establishment of a connection. In this phase, message descriptions in the log prefix are more phrase-like.

First the client attempts to initiate:
```
C000:0048B:s<c: client "unknown(local)" attempting connection: { byte-order=LSBFirst protocol-major-version=11 protocol-minor-version=0 authorization-protocol-name="MIT-MAGIC-COOKIE-1" authorization-protocol-data=(16 bytes) }
```
The server may refuse the connection, which ends communication, or request further authentication, at which point `xtracepp` ends the connection itself. But, if all goes well, the connection is accepted (lists are truncated in example):
```
C000:6740B:s>c: server accepted connection: { protocol-major-version=11 protocol-minor-version=0 release-number=12302006 resource-id-base=14680064 resource-id-mask=0x001fffff motion-buffer-size=256 maximum-request-length=65535 image-byte-order=LSBFirst bitmap-format-bit-order=LeastSignificant bitmap-format-scanline-unit=32 bitmap-format-scanline-pad=32 min-keycode=8 max-keycode=255 vendor="The X.Org Foundation" pixmap-formats=[ { depth=1 bits-per-pixel=1 scanline-pad=32 } ... ] roots=[ { root=1037 default-colormap=61 white-pixel=16777215 black-pixel=0 current-input-masks=KeyPress,StructureNotify,SubstructureNotify,SubstructureRedirect,PropertyChange,ColormapChange width-in-pixels=1680 height-in-pixels=1050 width-in-millimeters=444 height-in-millimeters=277 min-installed-maps=1 max-installed-maps=1 root-visual=62 backing-stores=WhenMapped save-unders=False root-depth=24 allowed-depths=[ { depth=24 visuals=[ { visual-id=62 class=TrueColor bits-per-rgb-value=8 colormap-entries=256 red-mask=0x00ff0000 green-mask=0x0000ff00 blue-mask=0x000000ff } ... ] } ] } ] }
```
After a connection is accepted, we enter into the normal phase of [request](#requests)/[reply](#replies)/[event](#events)/[error](#errors) communication.

#### [Requests]
Client prompts for server behavior.

Core protocol requests will have a single opcode:
```
C000:0008B:s<c:S00001: Request GetWindowAttributes(3): { window=0 }
```
whereas extension requests will have both a major opcode that represents the extension, plus a minor opcode for the actual request:
```
C000:0004B:s<c:S00002: Request BIG-REQUESTS(133)-BigReqEnable(0): { }
```

#### [Replies]
Direct server responses to requests (only some requests expect replies.)

`xtracepp` identifies the relevant request by connection and sequence number, and includes its name and opcode(s) in the message prefix:
```
C000:0032B:s<c:S00001: Reply to GetProperty(20): { format=8 type=31("STRING") bytes-after=0 value="" }
```
```
C000:0032B:s<c:S00002: Reply to BIG-REQUESTS(133)-BigReqEnable(0): { maximum-request-length=4194303 }
```

#### [Events]
Server reports of state changes, often triggered by requests, but not directly tied to them.

Log line prefix will identify events by their event code:
```
C000:0032B:s>c:S00082: Event Expose(12): { window=14680075 x=0 y=0 width=164 height=164 count=0 }
```
<!-- TBD extension events -->

> [!NOTE]
> Request [SendEvent] field `event` will format an event in the same way, but without a prefix:
> ```
> C000:0044B:s<c:S00001: Request SendEvent(25): { propagate=False destination=PointerWindow event-mask=StructureNotify event=KeyRelease(3) { detail=0 time=0x00000000 root=0 event=0 child=None root-x=0 root-y=0 event-x=0 event-y=0 state=0x0000 same-screen=False } }
> ```

#### [Errors]
Server responses to failed requests.

Log line prefix will identify errors by their error code:
```
C000:0032B:s>c:S00001: Error Value(2): { (bad value)=8 (minor opcode)=0 (major opcode)=35 }
```
<!-- TBD extension errors -->

</details>


## Added Features

### Extension Filtering by Name
Protocol extensions can be filtered:
- postively with one or more uses of `--denyextensions`(`-e`)` extension_name` (named extensions are excluded, or `all` to disable all)
- negatively with one or more uses of `--allowextensions`(`-e`)` extension_name` (unnamed extensions excluded)

Any unsupported extensions are automatically denied. Extension support:
| `xtracepp` version | extension |
|------------|-----------|
| 1.0+ | [BIG-REQUESTS] |

### Multiline Log Entries
Enabled with `--multiline`(`-m`,) log entries are formatted for greater readability by setting data fields each on their own line. Indenting, brackets and braces will be formatted to visualize nesting hierarchy. Some structures/lists are exceptional and will always be single-line formatted. Can be combined with all other format options.

For example, this `StoreColors` request would go from:
```
C000:0032B:s<c:S00001: Request StoreColors(89): { cmap=0 items=[ { pixel=1 red=2 green=3 blue=4 (do rgb mask)=do-green } { pixel=5 red=6 green=7 blue=8 (do rgb mask)=do-red,do-blue } ] }
```
to this when using `--multiline` (`items` list is comprised of `COLORITEM` protocol structs which are always formatted singleline):
```
C000:0032B:s<c:S00001: Request StoreColors(89): {
  cmap  = 0
  items = [
    { pixel=1 red=2 green=3 blue=4 (do rgb mask)=do-green }
    { pixel=5 red=6 green=7 blue=8 (do rgb mask)=do-red,do-blue }
  ]
}
```

### Variable Verbosity
Enabled with `--verbose`(`-v`,) log entries are formatted with more complete information about the actual message encoding:
- some data fields normally omitted are now logged (eg `opcode` and `(total aligned units)` in example below)
- data fields appear as `name=value_hex(value)` (hex at the appropriate width for the encoded integer size)

Can be combined with all other format options.

Using the previous example, this `StoreColors` request would go from:
```
C000:0032B:s<c:S00001: Request StoreColors(89): { cmap=0 items=[ { pixel=1 red=2 green=3 blue=4 (do rgb mask)=do-green } { pixel=5 red=6 green=7 blue=8 (do rgb mask)=do-red,do-blue } ] }
```
to this with `--verbose`:
```
C000:0032B:s<c:S00001: Request StoreColors(89): { opcode=0x59(89) (total aligned units)=0x0008(8) cmap=0x00000000(0) items=[ { pixel=0x00000001(1) red=0x0002(2) green=0x0003(3) blue=0x0004(4) (do rgb mask)=0x02(do-green) } { pixel=0x00000005(5) red=0x0006(6) green=0x0007(7) blue=0x0008(8) (do rgb mask)=0x05(do-red,do-blue) } ] }
```

### `TIMESTAMP`s Formatted to Wall Clock Time
Enabled with `--systemtimeformat`(`-s`,) `TIMESTAMP` data fields are interpreted as human-readable wall-clock time as would be shown by `date +'%Y-%m-%dT%H:%M:%SZ%Z'`. Can be combined with other formatting options.

For example, `time` in this `PropertyNotify` event is normally not interpreted:
```
C000:0032B:s>c:S00078: Event PropertyNotify(28): { window=12582922 atom=314(unrecognized atom) time=0x25405193 state=NewValue }
```
but with `--systemtimestamps`:
```
C000:0032B:s>c:S00078: Event PropertyNotify(28): { window=12582922 atom=314(unrecognized atom) time=0x25405193(2026-02-09T:07:44:34ZUTC) state=NewValue }
```

### Scan Server Interned `ATOM`s
Enabled with `--prefetchatoms`(`-p`,) the chance that `ATOM` strings will be logged as they are stored in the server is greatly increased. (By default, any `ATOM` value not predefined by the protocol will appear as `(unrecognized atom)`.)

Using the previous `PropertyNotify` example, `atom` is greater than the core protocol predefined max of 68, and so is unrecognized:
```
C000:0032B:s>c:S00078: Event PropertyNotify(28): { window=12582922 atom=314(unrecognized atom) time=0x25698a81 state=NewValue }
```
but with `--prefetchatoms` we can know the contents of the interned string:
```
C000:0032B:s>c:S00078: Event PropertyNotify(28): { window=12582922 atom=314("WM_STATE") time=0x25698a81 state=NewValue }
```

## Thanks/Credits
- [Bernhard Link] and the developers of the original [xtrace]
- [Qiang Yu] for their [fork] of `xtrace` and development of [DRI3] support


[Wayland]: https://wiki.archlinux.org/title/Wayland
[Xwayland]: https://wayland.freedesktop.org/docs/html/ch05.html
[X11 protocol]: https://www.x.org/releases/X11R7.7/doc/
[strace]: https://github.com/strace/strace
[xtrace]: https://salsa.debian.org/debian/xtrace
[C++17]: https://en.cppreference.com/w/cpp/17.html
[display name]: https://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html#Opening_the_Display
[libxcb]: https://gitlab.freedesktop.org/xorg/lib/libxcb/-/blob/e81b999a/src/xcb_util.c#L160
[Connection Setup]: https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#Connection_Setup
[Requests]: https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#request_format
[Replies]: https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#request_format
[Events]: https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#event_format
[SendEvent]: https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#requests:SendEvent
[Errors]: https://www.x.org/releases/X11R7.7/doc/xproto/x11protocol.html#error_format
[Bernhard Link]: https://salsa.debian.org/brlink
[Qiang Yu]: https://github.com/yuq
[fork]: https://github.com/yuq/xtrace

<!-- [extensions]: https://www.x.org/releases/X11R7.7/doc/ -->
<!-- X11 extensions -->
[Generic Event Extension]: https://www.x.org/releases/X11R7.7/doc/xextproto/geproto.html
[SHAPE]: https://www.x.org/releases/X11R7.7/doc/xextproto/shape.html
[MIT-SHM]: https://www.x.org/releases/X11R7.7/doc/xextproto/shm.html
[XInputExtension]: https://www.x.org/releases/X11R7.7/doc/inputproto/XI2proto.txt
[XTEST]: https://www.x.org/releases/X11R7.7/doc/xextproto/xtest.html
[BIG-REQUESTS]: https://www.x.org/releases/X11R7.7/doc/bigreqsproto/bigreq.html
[SYNC]: https://www.x.org/releases/X11R7.7/doc/libXext/synclib.html
[XKEYBOARD]: https://www.x.org/releases/X11R7.7/doc/kbproto/xkbproto.html
[XC-MISC]: https://www.x.org/releases/X11R7.7/doc/xcmiscproto/xc-misc.html
[XFIXES]: https://www.x.org/releases/X11R7.7/doc/fixesproto/fixesproto.txt
[RENDER]: https://www.x.org/releases/X11R7.7/doc/renderproto/renderproto.txt
[RANDR]: https://www.x.org/releases/X11R7.7/doc/randrproto/randrproto.txt
[XINERAMA]: https://gitlab.freedesktop.org/xorg/proto/xineramaproto/
[Composite]: https://www.x.org/releases/X11R7.7/doc/compositeproto/compositeproto.txt
[DAMAGE]: https://www.x.org/releases/X11R7.7/doc/damageproto/damageproto.txt
[DOUBLE-BUFFER]: https://www.x.org/releases/X11R7.7/doc/xextproto/dbe.html
[RECORD]: https://www.x.org/releases/X11R7.7/doc/recordproto/record.html
[Present]: https://gitlab.freedesktop.org/xorg/proto/presentproto/-/blob/master/presentproto.txt
[DRI2]: https://www.x.org/releases/X11R7.7/doc/dri2proto/dri2proto.txt
[DRI3]: https://cgit.freedesktop.org/xorg/proto/dri3proto/tree/dri3proto.txt
<!-- mirror: https://gitlab.freedesktop.org/xorg/proto/dri3proto/-/blob/master/dri3proto.txt -->
<!-- also: https://keithp.com/blogs/dri3_extension/ -->
[X-Resource]: https://www.x.org/releases/X11R7.7/doc/resourceproto/resproto.txt
[XVideo]: https://www.x.org/releases/X11R7.7/doc/videoproto/xv-protocol-v2.txt
[GLX]: https://gitlab.freedesktop.org/xorg/proto/glproto/
[XFree86-VidModeExtension]: https://gitlab.freedesktop.org/xorg/proto/xf86vidmodeproto/
