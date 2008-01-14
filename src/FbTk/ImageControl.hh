// ImageControl.hh for FbTk - Fluxbox Toolkit
// Copyright (c) 2001 - 2003 Henrik Kinnunen (fluxbox at users.sourceforge.net)
//
// from Image.hh for Blackbox - an X11 Window manager
// Copyright (c) 1997 - 2000 Brad Hughes (bhughes at tcac.net)
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef FBTK_IMAGECONTROL_HH
#define FBTK_IMAGECONTROL_HH

#include "Text.hh" // actually, Text is rather tool like, that's where orientation comes from
#include "Timer.hh"
#include "NotCopyable.hh"

#include <X11/Xlib.h> // for Visual* etc
#include <list>

namespace FbTk {

class Texture;

/// Holds screen info, color tables and caches textures
class ImageControl: private NotCopyable {
public:
    ImageControl(int screen_num, bool dither = false, int colors_per_channel = 4,
                  unsigned long cache_timeout = 300000l, unsigned long cache_max = 200l);
    virtual ~ImageControl();

    bool doDither() const { return m_dither; }
#ifdef NOT_USED
    int bitsPerPixel() const { return bits_per_pixel; }
#endif
    int depth() const { return m_screen_depth; }
    int colorsPerChannel() const { return m_colors_per_channel; }
    int screenNumber() const { return m_screen_num; }
    Visual *visual() const { return m_visual; }
    unsigned long getSqrt(unsigned int val) const;

    /**
       Render to pixmap
       @param width width of pixmap
       @param height height of pixmap
       @param src_texture texture type to render
       @return pixmap of the rendered image, on failure None
    */
    Pixmap renderImage(unsigned int width, unsigned int height,
                       const FbTk::Texture &src_texture,
                       Orientation orient = ROT0);

    void installRootColormap();
    void removeImage(Pixmap thepix);
    void colorTables(const unsigned char **, const unsigned char **, const unsigned char **,
                     int *, int *, int *, int *, int *, int *) const;
#ifdef NOT_USED
    void getXColorTable(XColor **, int *);
#endif
    void getGradientBuffers(unsigned int, unsigned int,
                            unsigned int **, unsigned int **);
    void setDither(bool d) { m_dither = d; }
#ifdef NOT_USED
    void setColorsPerChannel(int cpc);
#endif

    void cleanCache();
private:
    /** 
        Search cache for a specific pixmap
        @return None if no cache was found
    */
    Pixmap searchCache(unsigned int width, unsigned int height, const Texture &text, Orientation orient) const;

    void createColorTable();
    bool m_dither;
    Timer m_timer;

    Colormap m_colormap;

    Window m_root_window;

    XColor *m_colors; ///< color table
    unsigned int m_num_colors; ///< number of colors in color table

    Visual *m_visual;

    int bits_per_pixel, red_offset, green_offset, blue_offset,
        red_bits, green_bits, blue_bits;
    int m_colors_per_channel; ///< number of colors per channel
    int m_screen_depth; ///< bit depth of screen
    int m_screen_num;  ///< screen number
    unsigned char red_color_table[256];
    unsigned char green_color_table[256];
    unsigned char blue_color_table[256];

    unsigned int *grad_xbuffer, *grad_ybuffer, grad_buffer_width,
        grad_buffer_height;

    struct Cache;
    typedef std::list<Cache *> CacheList;

    mutable CacheList cache;
    unsigned long cache_max;
};

} // end namespace FbTk

#endif // FBTK_IMAGECONTROL_HH

