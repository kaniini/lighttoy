# lighttoy

A library (`libbulb`) and tool for managing smart lightbulbs.
Or, alternatively, making the Internet of Things (*groan*) work for *you*.

## the elevator pitch

Did you -waste- err, I mean, spend $100 on one of those fancy lightbulbs?  If you didn't, well,
you're probably more sane than the author of this, and this is probably boring to you
as well.  If you *did* and you *want* to make the lightbulbs do what you want *programatically*,
either `libbulb` or `lighttoy` is for you.

## `libbulb`

As previously mentioned, `libbulb` is a library that speaks (at present, just the LIFX protocol) to
lightbulbs.  It can turn them on/off, set their color, and later do some other things (like set their
label).

It is kinda, sorta, based on the LIFX C SDK, but not really because that thing is not very good.  But
the protocol headers definitely are based on that SDK.

Also included is a nifty `libbulb.pc` pkg-config module which can be used with pkgconf or pkg-config.

## `lighttoy`

This is an application which provides a CLI frontend to `libbulb`.  It exposes all `libbulb` functionality,
in the convenience of a command-line.  Imagine the shell-scripting possibilities!

It also has some cool built-in effects such as:

  * `--colorstrobe`, like a strobelight except it cycles colors instead of turning on/off.
