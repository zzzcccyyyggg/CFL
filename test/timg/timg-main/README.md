<img src="img/logo.svg" align="left" width="165" alt="timg logo" title="timg">
<!-- github markdown eats style, otherwise we could use em-units for width. -->

Terminal Image and Video Viewer
===============================

[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://github.com/hzeller/timg/blob/main/LICENSE) &nbsp;
[![Ubuntu Build](../../workflows/CI/badge.svg)](../../actions/workflows/ubuntu.yml)
[![macOS Build](../../workflows/macOS%20Build/badge.svg)](../../actions/workflows/macos.yml)
[![macOS Brew Building HEAD](../../workflows/macOS%20Brew%20Building%20HEAD/badge.svg)](../../actions/workflows/macos-brew.yml)


### https://timg.sh/

A user-friendly terminal image viewer that uses graphic capabilities of
terminals (Sixel, Kitty or iterm2), or 24-Bit color capabilities and unicode
character blocks if these are not available.

On terminals that implement the [sixel] protocol, the
[Kitty Graphics Protocol], or the [iTerm2 Graphics Protocol] this displays
images in full resolution.

But even the fallback block display is usable.
![](./img/sunflower-term.png)

Displays regular images, plays animated gifs, scrolls static images and
plays videos.

Useful if you want to have a quick visual check without leaving the comfort
of your shell and having to start a bulky image viewer. Sometimes this is the
only way if your terminal is connected remotely via ssh. And of course if you
don't need the resolution. While icons typically fit pixel-perfect, larger
images are scaled down to match the resolution.

The command line accepts any number of image/video filenames that it shows
in sequence one per page or in a grid in multiple columns, depending on your
choice of `--grid`. The output is emitted in-line with minimally messing
with your terminal, so you can simply go back in history using your terminals'
scroll-bar (Or redirecting the output to a file allows you to later
simply `cat` that file to your terminal. Even `less -R` seems to be happy with
it).

#### Pixelation
On a regular terminal, block-characters are used to output images. Half blocks
present pixels color-accurately, and quarter blocks provide a higher spatial
resolution at the expense of slightly worse color accuracy. These modes should
be compatible with most common terminals that support UTF8 and 24Bit color.

If you are on a [Kitty][Kitty Graphics Protocol], [iTerm2], or [wezterm]
terminal, or a terminal that supports the sixel protocol, images can be shown
in full resolution.

-p kitty,  -p iterm2, or -p sixel | -p quarter | -p half |
----------------------------------|------------|---------|
 ![](img/pixelation-kitty.png) | ![](img/pixelation-quarter.png) | ![](img/pixelation-half.png) |

#### Grid display

Images can be shown in a grid, which is very useful if you quickly want to
browse through a lot of images. You can choose to show the filename as title,
so it is easy to find exactly the filename you're looking for (The following
grid uses `--grid=2` and is pixelated `-p iterm2`).

![Grid view of 4 pictures](img/grid-timg-wezterm.png)
<details>
<summary>This is how the same grid looks if no high-res mode is available with - `-p quarter` ...</summary>

![](./img/grid-timg.png)

</details>

### Synopsis

```
usage: timg [options] <image/video> [<image/video>...]
Options (most common first):
        -p<pixelation> : Pixelation: 'h' = half blocks    'q' = quarter blocks
                                     'k' = kitty graphics 'i' = iTerm2 graphics
                                     's' = sixel graphics
                         Default: Auto-detect graphics, otherwise 'quarter'.
        --grid=<cols>[x<rows>] : Arrange images in a grid ("contact sheet").
        -C, --center   : Center image horizontally in available cell.
        --title[=<fmt_str>]: Print title above each image. Accepts the following
                         format parameters: %f = full filename; %b = basename
                                            %w = image width; %h = height
                                            %D = internal decoder used
                         If no parameter is given, defaults to "%f"
                         Env-var override with TIMG_DEFAULT_TITLE
        -f<filelist>   : Read newline-separated list of image files to show.
                         Relative filenames are relative to current directory.
                         (-f and -F can be provided multiple times.)
        -F<filelist>   : like -f, but relative filenames considered relative
                         to the directory containing the filelist.
        -b<str>        : Background color to use behind alpha channel. Format
                         color name like 'yellow', '#rrggbb', 'auto' or 'none'.
                        'auto' is terminal background color. (default 'auto').
        -B<str>        : Checkerboard pattern color to use on alpha.
        --pattern-size=<n> : Integer factor scale of the checkerboard pattern.
        --auto-crop[=<pre-crop>] : Crop away all same-color pixels around image.
                         The optional pre-crop is the width of border to
                         remove beforehand to get rid of an uneven border.
        --rotate=<exif|off> : Rotate according to included exif orientation.
                              or 'off'. Default: exif.
        -W, --fit-width: Scale to fit width of available space, even if it
                         exceeds height.
        -U, --upscale[=i]: Allow Upscaling. If an image is smaller than the
                         available frame (e.g. an icon), enlarge it to fit.
                         Optional parameter 'i' only enlarges in integer steps.
        --clear[=every]: Clear draw area first. Optional argument 'every' will
                         clear before every image (useful with -w/-wr,
                         but not with --grid)
        -V             : Directly use Video subsystem. Don't probe image
                         decoding first (useful, if you stream video from stdin)
        -I             : Only  use Image subsystem. Don't attempt video decoding
        -w<seconds>    : Wait time between images (default: 0.0).
        -wr<seconds>   : like above, but wait time between rows in grid.
        -a             : Switch off anti-aliasing (default: on).
        -g<w>x<h>      : Output geometry in character cells. Partial geometry
                         leaving out one value -g<w>x or -gx<h> is possible,
                         the other value it then derived from the terminal size.
                         Default derived from terminal size is 160x50
        -o<outfile>    : Write to <outfile> instead of stdout.
        -E             : Don't hide the cursor while showing images.
        --compress[=level]: Only for -pk or -pi: Compress image data. More
                         CPU use, but less used bandwidth. (default: 1)
        --threads=<n>  : Run image decoding in parallel with n threads
                         (Default 3, 3/4 #cores on this machine)
        --color8       : Choose 8 bit color mode for -ph or -pq
        --version      : Print detailed version including used libraries.
                         (v1.5.3+)
        --verbose      : Print some stats after images shown.
        -h             : Print this help and exit.
        --help         : Page through detailed manpage-like help and exit.

  Scrolling
        --scroll[=<ms>]       : Scroll horizontally (optionally: delay ms (60)).
        --delta-move=<dx:dy>  : delta x and delta y when scrolling (default:1:0)

  For Animations, Scrolling, or Video
  These options influence how long/often and what is shown.
        --loops=<num> : Number of runs through a full cycle. -1 means 'forever'.
                        If not set, videos loop once, animated images forever
                        unless there is more than one file to show.
        --frames=<num>: Only show first num frames (if looping, loop only these)
        --frame-offset=<num>: Start animation/video at this frame
        -t<seconds>   : Stop after this time, independent of --loops or --frames
```

### Examples
```bash
timg some-image.jpg                # display a static image
timg -g50x50 some-image.jpg        # display image fitting in box of 50x50 pixel

# Multiple images
timg *.jpg                         # display all *.jpg images
timg --title *.jpg                 # .. show name in title (short option -F)
timg --title="%b (%wx%h)" *.jpg    # show short filename and image size as title
timg --grid=3x2 *.jpg              # arrange in 3 columns, 2 rows in terminal
timg --fit-width --grid=3 *.jpg    # maximize use of column width (short: -W)
timg --grid=3 -t5 *.gif            # Load gifs one by one in grid. Play each for 5sec.

# Putting it all together; making an alias to list images; let's call it ils = 'image ls'
# This prints images two per row with a filename title. Only showing one frame
# so for animated gifs only the first frame is shown statically.
# With hi-res iTerm or Kitty terminals, consider more columns, e.g --grid=4x1
# Put this line in your ~/.bashrc
alias ils='timg --grid=2x1 --upscale=i --center --title --frames=1 '

# ... using this alias on images outputs a useful column view
ils *.jpg *.gif

# Read the list of images to load from a file. One filename per line.
locate "*.jpg" > /tmp/allimg.txt ; timg -f /tmp/allimg.txt

# Show a PDF document, use full width of terminal, trim away empty border
timg -W --auto-crop some-document.pdf
timg --frames=1 some-document.pdf    # Show a PDF, but only first page

# Reading images from a pipe. The filename '-' means 'read from stdin.
# In this example generating a QR code and have timg display it:
qrencode -s1 -m2 "http://timg.sh/" -o- | timg -

# Here, using gnuplot output right in the shell
echo "set terminal png; plot sin(x);" | gnuplot | timg -

# Open an image from a URL. URLs are internally actually handled by the
# video subsystem, so it is treated as a single-frame 'film', nevertheless,
# many image-URLs just work. But some image-specific features, such as trimming
# or scrolling, won't work.
timg --center https://i.kym-cdn.com/photos/images/newsfeed/000/406/282/2b8.jpg

# Sometimes, it is necessary to manually crop a few pixels from an
# uneven border before the auto-crop finds uniform color all-around to remove.
# For example with --auto-crop=7 we'd remove first seven pixels around an image,
# then do the regular auto-cropping.
#
# The following example loads an image from a URL; --auto-crop does not work with
# that, so we have to get the content manually, e.g. with wget. Piping to
# stdin works; in the following example the stdin input is designated with the
# special filename '-'.
#
# For the following image, we need to remove 3 pixels all around before
# auto-crop can take over removing the remaining whitespace successfully:
wget -qO- https://imgs.xkcd.com/comics/a_better_idea.png | timg --auto-crop=3 -

timg multi-resolution.ico   # See all the bitmaps in multi-resolution icons-file
timg --frames=1 multi-resolution.ico  # See only the first bitmap in that file

timg some-video.mp4         # Watch a video.

# Play content of webcam (This assumes video4linux2, but whatever input devices
# are supported on your system with libavdevice-dev)
timg /dev/video0

# If you read a video from a pipe, it is necessary to skip attempting the
# image decode first as this will consume bytes from the pipe. Use -V option.
youtube-dl -q -o- -f'[height<480]' 'https://youtu.be/dQw4w9WgXcQ' | timg -V -

# Show animated gif, possibly limited by timeout, loops or frame-count
timg some-animated.gif      # show an animated gif forever (stop with Ctrl-C)
timg -t5 some-animated.gif                   # show animated gif for 5 seconds
timg --loops=3 some-animated.gif             # Loop animated gif 3 times
timg --frames=3 --loops=1 some-animated.gif  # Show only first three frames
timg --frames=1 some-animated.gif            # Show only first frame. Static image.

# Scroll
timg --scroll some-image.jpg       # scroll a static image as banner (stop with Ctrl-C)
timg --scroll=100 some-image.jpg   # scroll with 100ms delay

# Create a text with the ImageMagick 'convert' tool and send to timg to scroll
convert -size 1000x60 xc:none -fill red -gravity center -pointsize 42 \
        -draw 'text 0,0 "Watchen the blinkenlights..."' -trim png:-   \
      | timg --scroll=20 -

# Scroll direction. Horizontally, vertically; how about diagonally ?
timg --scroll --delta-move=1:0 some-image.jpg  # scroll with dx=1 and dy=0, so horizontally.
timg --scroll --delta-move=-1:0 some-image.jpg # scroll horizontally in reverse direction.
timg --scroll --delta-move=0:2 some-image.jpg  # vertical, two pixels per step.
timg --scroll --delta-move=1:1 some-image.jpg  # diagonal, dx=1, dy=1

# Background color for transparent images (SVG-compatible strings are supported)
# and generally useful if you have a transparent PNG that is otherwise hard
# to see on your terminal background.
timg -b auto some-transparent-image.png  # use terminal background if possible
timg -b none some-transparent-image.png  # Don't use blending
timg -b lightgreen some-transparent-image.png
timg -b 'rgb(0, 255, 0)' some-transparent-image.png
timg -b '#0000ff' some-transparent-image.png

# Checkerboard/Photoshop-like background on transparent images
timg -b lightgray -B darkgray some-transparent-image.png

# .. with adjustable size.
timg -b lightgray -B darkgray --pattern-size=4 some-transparent-image.png
```

##### Partially transparent icon on champagne-colored terminal emulator

-b auto  | -b lightgreen | -b lightgreen -B yellow | -b none  |
---------|---------------|-------------------------|----------|
![](img/alpha-bauto.png) | ![](img/alpha-blightgreen.png)  | ![](img/alpha-blightgreen-Byellow.png) | ![](img/alpha-bnone.png)

--pattern-size=1                       | --pattern-size=4 |
---------------------------------------|--------------------------------------|
![](img/alpha-blightgreen-Byellow.png) | ![](img/alpha-blightgreen-Byellow-4.png) |

#### Include in file browsers

There are many terminal based file-browsers. Adding `timg` to their
configuration is usually straight forward.
```
# Another use: can run use this in a fzf preview window:
echo some-image.jpg | fzf --preview='timg -E --frames=1 --loops=1 -g $(( $COLUMNS / 2 - 4 ))x$(( $FZF_PREVIEW_LINES * 2 )) {}'

# Use in vifm. ~/.config/vifm/vifmrc
filextype *.avi,*.mp4,*.wmv,*.dat,*.3gp,*.ogv,*.mkv,*.mpg,*.mpeg,*.vob,
         \*.fl[icv],*.m2v,*.mov,*.webm,*.ts,*.mts,*.m4v,*.r[am],*.qt,*.divx,
         \*.as[fx]
        \ {View in timg}
        \ timg --title --center --clear %f,

filextype *.bmp,*.jpg,*.jpeg,*.png,*.gif,*.xpm
        \ {View in timg}
        \ timg --title --center --clear %f; read -n1 -s -r -p "Press any key to return",
```

#### Other fun things

```bash
# Also, you could store the output and cat later to your terminal...
timg -g80x40 some-image.jpg > /tmp/imageout.txt
cat /tmp/imageout.txt

# Of course, you can redirect the output to somewhere else. I am not suggesting
# that you rickroll some terminal by redirecting timg's output to a /dev/pts/*
# you have access to, but you certainly could...

# Of course, you can go really crazy by storing a cycle of an animation. Use xz
# for compression as it seems to deal with this kind of stuff really well:
timg -g60x30 --loops=10 nyan.gif | xz > /tmp/nyan.term.xz

# ..now, replay the generated ANSI codes on the terminal. Since it would
# rush through as fast as possible, we have to use a trick to wait between
# frames: Each frame has a 'move cursor up' escape sequence that contains
# an upper-case 'A'. We can latch on that to generate a delay between frames:
xzcat /tmp/nyan.term.xz | gawk '/\[.*A/ { system("sleep 0.1"); } { print $0 }'

# You can wrap all that in a loop to get an infinite repeat.
while : ; do xzcat... ; done

# (If you Ctrl-C that loop, you might need to use 'reset' for terminal sanity)
```

## Terminal considerations

This section contains some details that you only might need to ever look at if
the output is not as expected.

### Many terminals support direct hi-res image output

The [Kitty], [iTerm2], and [wezterm] terminals as well as other
modern terminals such as [Konsole] or the terminal in [vscode] allow to
directly display high-resolution pictures.

If `timg` is running in such a terminal, it will attempt to auto-detect that
feature and use that mode. If your terminal does support the feature but
can't be auto-detected, you can explicitly choose the pixelation option
in question with command line flag or environment variable (see `timg --help`).
(Please file an issue with `timg` if auto-detect does not work).

#### VSCode Terminal
The terminal in [vscode] to display images in high-resolution, you
need to enable the _Terminal > Integrated: Enable Images_ setting in vscode.
Otherwise you have to explicitly choose `-pq` to show the 'block' images.

#### In tmux
The terminal multiplexer `tmux` prevents high-resolution images as it filters
out the escape codes.
However, with some ... workarounds, `timg` can show such pictures
in `tmux` >= version 3.3 iff in a [kitty]-terminal.

So if you need hi-res pictures in tmux, use a kitty terminal; you
also have to explicitly set the pixelation to `-pk` (see `timg --help` for
details).

#### Sixel
Other terminals support an older high-resolution [sixel]-protocol, which you
can choose with `-ps`. Note, for this to work in xterm, you need to invoke it
with `xterm -ti vt340`.

#### High resolution and low bandwidth
If watching hi-res videos remotely is too slow (due to high bandwidth
requirements or simply because your terminal has to do more work), try
setting the environment variable `TIMG_ALLOW_FRAME_SKIP=1` to allow timg
leaving out frames to stay on track (see `timg --help`, environment variable
section). You can also attempt to set the `--compress` level higher.

### Half block and quarter block rendering

Terminals that do not support high-resolution image output can still show
images by virtue of showing colored blocks.

The half block pixelation (`-p half`) uses the the unicode
character [▄](U+2584 - 'Lower Half Block')
_or_ [▀](U+2580 - 'Upper Half Block') (depending on the
[`TIMG_USE_UPPER_BLOCK`](#half-block-choice-of-rendering-block)
environment variable). If the top and bottom color is the same, a simple
space with background color is used.

The quarter block pixelation (`-p quarter`) uses eight different blocks.

With both of these pixelations, choosing the foreground color and background
24-bit color, `timg` can simulate 'pixels'. With the half-block pixelation,
this can assign the correct color to the two 'pixels' available in one
character cell, in the quarter pixelation, four 'pixels' have to share two
colors, so the color accuracy is slighlty worse but it allows for higher spatial
resolution.

The `-p` command line flag allows to choose between `-p half`, `-p quarter`,
also possible to just shorten to `-ph` and `-pq`. Default is `-pq`
(see [above](#pixelation) how this looks like).

Terminals that don't support Unicode or 24 bit color will probably not show
a very pleasent output. For terminals that only do 8 bit color, use the
`--color8` command line option.


#### Half block: Choice of rendering block

By default, `timg` uses the 'lower half block' to show the pixels in `-p half`
mode. Depending on the font the terminal is using, using the upper block might
look better, so it is possible to change the default with an environment
variable.
Play around with this value if the output looks poor on your terminal. I found
that on my system there is no difference for [`konsole`][konsole] or `xterm` but the
[`cool-retro-term`][cool-retro-term] looks better with the lower block, this is why it is the default.

In some terminals, such as [alacritty] (and only with certain font sizes),
there seems to be the opposite working better. To change, set this environment
variable:

```
export TIMG_USE_UPPER_BLOCK=1   # change default to use upper block.
```
(this only will work fully with `-p half`. In `-p quarter` mode, there are
additional blocks that can't be worked around)

##### What a problematic choice of block looks like

The image generally looks a bit 'glitchy' if the terminal leaves little
space between blocks, so that the wrong background color shows on a single
line between pixels.
This is likely not intended by the terminal emulator and possibly happening
on rounding issues of font height or similar.

Anyway, we can work around it (fully in `-p half`, partially in `-p quarter`
mode). In the following illustration you see how that looks like. If you
see that, change the `TIMG_USE_UPPER_BLOCK` environment variable.


Glitchy. Change TIMG_USE_UPPER_BLOCK| Looks good
------------------------------------|-------------------------------|
![](img/needs-block-change.png)     | ![](img/block-ok.png)|

#### Other artifacts

Some terminals leave one pixel of horizontal space between characters that
result in fine vertical lines in the image.
That can't be worked around, send a bug or better pull request to your terminal
emulator.

#### Wrong font aspect ratio

Usually, timg attempts to determine the font aspect ratio and apply some
correction if it is off from the nominal 1:2. But if you notice that the
image displayed is not quite the right aspect ratio because of the terminals
font used, you can set an environment variable `TIMG_FONT_WIDTH_CORRECT` with
a factor to make it look correctly.

Increasing the visual width by 10% would be setting the value to 1.1 for
instance.

```
export TIMG_FONT_WIDTH_CORRECT=1.1
timg myimage.jpg
```

This is an environment variable, so that you can set it once to best fit your
terminal emulator of choice and don't have to worry about later.

##### Example
Terminal font too narrow   | Correct. Here with `TIMG_FONT_WIDTH_CORRECT=1.375`
---------------------------|-------------------------------|
![](img/aspect-wrong.png)  | ![](img/aspect-right.png)|


## Installation
### Install pre-built package

<a href="https://repology.org/project/timg/versions">
<img src="https://repology.org/badge/vertical-allrepos/timg.svg" alt="Packaging status" align="right">
</a>

#### Debian-based systems

```bash
sudo apt install timg
```

#### NixOS or Nix package manager

```bash
nix-env -i timg
```

#### macOS

```bash
brew install timg
```

#### Use AppImage

The [timg release page](https://github.com/hzeller/timg/releases/latest) also
has a _minimal_ binary in the [AppImage package format][AppImage].
To keep the size small, it does _not_ include video decoding or some more
fancy image formats. It is good for many contexts or if you want to try out
`timg`, but for a full-featured binary, use one from your distribution or
build from source.

### Build and Install from source

#### Get dependencies on Debian/Ubuntu

```bash
sudo apt install cmake git g++ pkg-config
sudo apt install libgraphicsmagick++-dev libturbojpeg-dev libexif-dev libswscale-dev libdeflate-dev librsvg2-dev libcairo-dev # needed libs

# For sixel output.
sudo apt install libsixel-dev

# If you want to include video decoding, also install these additional libraries
sudo apt install libavcodec-dev libavformat-dev

sudo apt install libavdevice-dev # If you want to read from video devices such as v4l2

sudo apt install libopenslide-dev # If you want to add OpenSlide images support
sudo apt install libpoppler-glib-dev  # if WITH_POPPLER enabled.

sudo apt install pandoc  # If you want to recreate the man page
```

#### Get dependencies on NixOS or Nix package manager

The dependencies are set-up in the shell.nix, so you're ready to go opening
a nix shell

```bash
nix-shell
```

#### Get dependencies on fedora

```bash
sudo dnf install cmake git g++ pkg-config
sudo dnf install GraphicsMagick-c++-devel turbojpeg-devel libexif-devel libswscale-free-devel librsvg2-devel cairo-devel

# If you want to include video decoding, also install these additional libraries
sudo dnf install libavcodec-free-devel libavformat-free-devel libavdevice-free-devel openslide-devel
sudo dnf install pandoc  # If you want to recreate the man page
```

#### Get dependencies on macOS

```bash
# Homebrew needs to be available to install required dependencies
brew install cmake git GraphicsMagick webp jpeg-turbo libexif librsvg cairo  # needed libs

# If you want to include video decoding, install these additional libraries
brew install ffmpeg

# If you want to add OpenSlide images support
brew install openslide

brew install pandoc  # If you want to recreate the man page
```

#### Get repo and compile timg

In the script below you see that the build system allows for some
compile-time choices:

  * **`WITH_LIBSIXEL`** Use libsixel to output images in [sixel] graphics
     format. Default **ON**.
  * **`WITH_VIDEO_DECODING`** allow for video decoding. Requires ffmpeg-related
     libraries. You typically want this **ON** (default)
     * **`WITH_VIDEO_DEVICE`** this allows for accessing connected video
     devices, e.g. you can watch your webcam input (requires
     `WITH_VIDEO_DECODING`).
  * **`WITH_GRAPHICSMAGICK`** This is the main image loading library so you
    typically want this **ON** (default).
  * **`WITH_TURBOJPEG`** If enabled, uses this for faster jpeg file loading.
     You typically want this **ON** (default).
  * **`WITH_POPPLER`** High-quality and faster PDF renderer. Needs poppler
    and cairo.
    If not compiled-in, will fallback to GraphicsMagick, but that typically
    results in lower quality renderings. Typically want this **ON** (default).
  * **`WITH_RSVG`** High-quality SVG renderer. Needs librsvg and cairo.
    If not compiled-in, will fallback to GraphicsMagick, but that typically
    results in lower quality renderings. Typically want this **ON** (default).
  * **`WITH_OPENSLIDE_SUPPORT`** Openslide is an image format used in scientific
    applications. Rarely used, so default off, switch ON if needed.
  * **`WITH_QOI_IMAGE`** Allow decoding of Quite Ok Image format [QOI]. Small
     and simple, default **ON**.
  * **`WITH_STB_IMAGE`** Compile the simpler STB image library directly into
    the timg binary in cases where Graphicsmagick is not suitable for
    dependency pruning reasons; output can be slower and of less quality.
    It is default **ON** (default) but will always only attempted
    after other image loading fails. Turn off if you can use GraphicsMagick and
    want to reduce potential security vectors.

You can choose these options by providing `-D<option>=ON` on the cmake command
line, see below.

```bash
git clone https://github.com/hzeller/timg.git  # Get repo
cd timg                     # Enter the checked out repository directory.
mkdir build                 # Generate a dedicated build directory.
cd build
# cmake with your desired options, see descriptions above
cmake ../ -DWITH_OPENSLIDE_SUPPORT=On
make

# After compilation, you can run from build/src/timg or install on your system with
sudo make install
```

[24-bit-term]: https://gist.github.com/XVilka/8346728
[cool-retro-term]: https://github.com/Swordfish90/cool-retro-term
[konsole]: https://konsole.kde.org/
[alacritty]: https://github.com/alacritty/alacritty
[Kitty]: https://sw.kovidgoyal.net/kitty/
[Kitty Graphics Protocol]: https://sw.kovidgoyal.net/kitty/graphics-protocol.html
[iTerm2]: https://iterm2.com/
[iTerm2 Graphics Protocol]: https://iterm2.com/documentation-images.html
[wezterm]: https://wezfurlong.org/wezterm/
[sixel]: https://en.wikipedia.org/wiki/Sixel
[QOI]: https://qoiformat.org/
[vscode]: https://code.visualstudio.com/
[AppImage]: https://appimage.org/
