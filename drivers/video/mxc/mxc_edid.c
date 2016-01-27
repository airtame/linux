/*
 * Copyright 2009-2014 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

/*!
 * @defgroup Framebuffer Framebuffer Driver for SDC and ADC.
 */

/*!
 * @file mxc_edid.c
 *
 * @brief MXC EDID driver
 *
 * @ingroup Framebuffer
 */

/*!
 * Include files
 */
#include <linux/i2c.h>
#include <linux/fb.h>
#include <video/mxc_edid.h>
#include "../edid.h"



#undef DEBUG  /* define this for verbose EDID parsing output */
#ifdef DEBUG
#define DPRINTK(fmt, args...) printk(fmt, ## args)
#else
#define DPRINTK(fmt, args...)
#endif

 /*
 *  Standard video mode definitions (taken from XFree86)
 */

const struct fb_videomode mxc_standard_modes[MXC_STANDARD_MODEDB_SIZE] = {
	{ NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0 },


	/* 640x400 @ 70 Hz, 31.5 kHz hsync */
	{ NULL, 70, 640, 400, 39721, 40, 24, 39, 9, 96, 2, 0,
		FB_VMODE_NONINTERLACED },

	/* 640x480 @ 60 Hz, 31.5 kHz hsync */
	{ NULL, 60, 640, 480, 39721, 40, 24, 32, 11, 96, 2,	0,
		FB_VMODE_NONINTERLACED },

	/* 800x600 @ 56 Hz, 35.15 kHz hsync */
	{ NULL, 56, 800, 600, 27777, 128, 24, 22, 1, 72, 2,	0,
		FB_VMODE_NONINTERLACED },

	/* 1024x768 @ 87 Hz interlaced, 35.5 kHz hsync */
	{ NULL, 87, 1024, 768, 22271, 56, 24, 33, 8, 160, 8, 0,
		FB_VMODE_INTERLACED },

	/* 640x400 @ 85 Hz, 37.86 kHz hsync */
	{ NULL, 85, 640, 400, 31746, 96, 32, 41, 1, 64, 3,
		FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED },

	/* 640x480 @ 72 Hz, 36.5 kHz hsync */
	{ NULL, 72, 640, 480, 31746, 144, 40, 30, 8, 40, 3, 0,
		FB_VMODE_NONINTERLACED },

	/* 640x480 @ 75 Hz, 37.50 kHz hsync */
	{ NULL, 75, 640, 480, 31746, 120, 16, 16, 1, 64, 3,	0,
		FB_VMODE_NONINTERLACED },

	/* 800x600 @ 60 Hz, 37.8 kHz hsync */
	{ NULL, 60, 800, 600, 25000, 88, 40, 23, 1, 128, 4,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED },

	/* 640x480 @ 85 Hz, 43.27 kHz hsync */
	{ NULL, 85, 640, 480, 27777, 80, 56, 25, 1, 56, 3, 0,
		FB_VMODE_NONINTERLACED },

	/* 1152x864 @ 89 Hz interlaced, 44 kHz hsync */
	{ NULL, 89, 1152, 864, 15384, 96, 16, 110, 1, 216, 10, 0,
		FB_VMODE_INTERLACED },
	/* 800x600 @ 72 Hz, 48.0 kHz hsync */
	{ NULL, 72, 800, 600, 20000, 64, 56, 23, 37, 120, 6,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED },

	/* 1024x768 @ 60 Hz, 48.4 kHz hsync */
	{ NULL, 60, 1024, 768, 15384, 168, 8, 29, 3, 144, 6, 0,
		FB_VMODE_NONINTERLACED },

	/* 640x480 @ 100 Hz, 53.01 kHz hsync */
	{ NULL, 100, 640, 480, 21834, 96, 32, 36, 8, 96, 6,	0,
		FB_VMODE_NONINTERLACED },

	/* 1152x864 @ 60 Hz, 53.5 kHz hsync */
	{ NULL, 60, 1152, 864, 11123, 208, 64, 16, 4, 256, 8, 0,
		FB_VMODE_NONINTERLACED },

	/* 800x600 @ 85 Hz, 55.84 kHz hsync */
	{ NULL, 85, 800, 600, 16460, 160, 64, 36, 16, 64, 5, 0,
		FB_VMODE_NONINTERLACED },

	/* 1024x768 @ 70 Hz, 56.5 kHz hsync */
	{ NULL, 70, 1024, 768, 13333, 144, 24, 29, 3, 136, 6, 0,
		FB_VMODE_NONINTERLACED },

	/* 1280x1024 @ 87 Hz interlaced, 51 kHz hsync */
	{ NULL, 87, 1280, 1024, 12500, 56, 16, 128, 1, 216, 12,	0,
		FB_VMODE_INTERLACED },

	/* 800x600 @ 100 Hz, 64.02 kHz hsync */
	{ NULL, 100, 800, 600, 14357, 160, 64, 30, 4, 64, 6, 0,
		FB_VMODE_NONINTERLACED },

	/* 1024x768 @ 76 Hz, 62.5 kHz hsync */
	{ NULL, 76, 1024, 768, 11764, 208, 8, 36, 16, 120, 3, 0,
		FB_VMODE_NONINTERLACED },

	/* 1152x864 @ 70 Hz, 62.4 kHz hsync */
	{ NULL, 70, 1152, 864, 10869, 106, 56, 20, 1, 160, 10, 0,
		FB_VMODE_NONINTERLACED },

	/* 1280x1024 @ 61 Hz, 64.2 kHz hsync */
	{ NULL, 61, 1280, 1024, 9090, 200, 48, 26, 1, 184, 3, 0,
		FB_VMODE_NONINTERLACED },

	/* 1400x1050 @ 60Hz, 63.9 kHz hsync */
	{ NULL, 60, 1400, 1050, 9259, 136, 40, 13, 1, 112, 3, 0,
		FB_VMODE_NONINTERLACED },

	/* 1400x1050 @ 75,107 Hz, 82,392 kHz +hsync +vsync*/
	{ NULL, 75, 1400, 1050, 7190, 120, 56, 23, 10, 112, 13,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED },

	/* 1400x1050 @ 60 Hz, ? kHz +hsync +vsync*/
	{ NULL, 60, 1400, 1050, 9259, 128, 40, 12, 0, 112, 3,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED },

	/* 1024x768 @ 85 Hz, 70.24 kHz hsync */
	{ NULL, 85, 1024, 768, 10111, 192, 32, 34, 14, 160, 6, 0,
		FB_VMODE_NONINTERLACED },

	/* 1152x864 @ 78 Hz, 70.8 kHz hsync */
	{ NULL, 78, 1152, 864, 9090, 228, 88, 32, 0, 84, 12, 0,
		FB_VMODE_NONINTERLACED },

	/* 1280x1024 @ 70 Hz, 74.59 kHz hsync */
	{ NULL, 70, 1280, 1024, 7905, 224, 32, 28, 8, 160, 8, 0,
		FB_VMODE_NONINTERLACED },

	/* 1600x1200 @ 60Hz, 75.00 kHz hsync */
	{ NULL, 60, 1600, 1200, 6172, 304, 64, 46, 1, 192, 3,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED },

	/* 1152x864 @ 84 Hz, 76.0 kHz hsync */
	{ NULL, 84, 1152, 864, 7407, 184, 312, 32, 0, 128, 12, 0,
		FB_VMODE_NONINTERLACED },

	/* 1280x1024 @ 74 Hz, 78.85 kHz hsync */
	{ NULL, 74, 1280, 1024, 7407, 256, 32, 34, 3, 144, 3, 0,
		FB_VMODE_NONINTERLACED },

	/* 1024x768 @ 100Hz, 80.21 kHz hsync */
	{ NULL, 100, 1024, 768, 8658, 192, 32, 21, 3, 192, 10, 0,
		FB_VMODE_NONINTERLACED },

	/* 1280x1024 @ 76 Hz, 81.13 kHz hsync */
	{ NULL, 76, 1280, 1024, 7407, 248, 32, 34, 3, 104, 3, 0,
		FB_VMODE_NONINTERLACED },

	/* 1600x1200 @ 70 Hz, 87.50 kHz hsync */
	{ NULL, 70, 1600, 1200, 5291, 304, 64, 46, 1, 192, 3, 0,
		FB_VMODE_NONINTERLACED },

	/* 1152x864 @ 100 Hz, 89.62 kHz hsync */
	{ NULL, 100, 1152, 864, 7264, 224, 32, 17, 2, 128, 19, 0,
		FB_VMODE_NONINTERLACED },

	/* 1280x1024 @ 85 Hz, 91.15 kHz hsync */
	{ NULL, 85, 1280, 1024, 6349, 224, 64, 44, 1, 160, 3,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED },

	/* 1600x1200 @ 75 Hz, 93.75 kHz hsync */
	{ NULL, 75, 1600, 1200, 4938, 304, 64, 46, 1, 192, 3,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED },

	/* 1680x1050 @ 60 Hz, 65.191 kHz hsync */
	{ NULL, 60, 1680, 1050, 6848, 280, 104, 30, 3, 176, 6,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED },

	/* 1600x1200 @ 85 Hz, 105.77 kHz hsync */
	{ NULL, 85, 1600, 1200, 4545, 272, 16, 37, 4, 192, 3,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED },

	/* 1280x1024 @ 100 Hz, 107.16 kHz hsync */
	{ NULL, 100, 1280, 1024, 5502, 256, 32, 26, 7, 128, 15, 0,
		FB_VMODE_NONINTERLACED },

	/* 1800x1440 @ 64Hz, 96.15 kHz hsync  */
	{ NULL, 64, 1800, 1440, 4347, 304, 96, 46, 1, 192, 3,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED },

	/* 1800x1440 @ 70Hz, 104.52 kHz hsync  */
	{ NULL, 70, 1800, 1440, 4000, 304, 96, 46, 1, 192, 3,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED },

	/* 512x384 @ 78 Hz, 31.50 kHz hsync */
	{ NULL, 78, 512, 384, 49603, 48, 16, 16, 1, 64, 3, 0,
		FB_VMODE_NONINTERLACED },

	/* 512x384 @ 85 Hz, 34.38 kHz hsync */
	{ NULL, 85, 512, 384, 45454, 48, 16, 16, 1, 64, 3, 0,
		FB_VMODE_NONINTERLACED },

	/* 320x200 @ 70 Hz, 31.5 kHz hsync, 8:5 aspect ratio */
	{ NULL, 70, 320, 200, 79440, 16, 16, 20, 4, 48, 1, 0,
		FB_VMODE_DOUBLE },

	/* 320x240 @ 60 Hz, 31.5 kHz hsync, 4:3 aspect ratio */
	{ NULL, 60, 320, 240, 79440, 16, 16, 16, 5, 48, 1, 0,
		FB_VMODE_DOUBLE },

	/* 320x240 @ 72 Hz, 36.5 kHz hsync */
	{ NULL, 72, 320, 240, 63492, 16, 16, 16, 4, 48, 2, 0,
		FB_VMODE_DOUBLE },

	/* 400x300 @ 56 Hz, 35.2 kHz hsync, 4:3 aspect ratio */
	{ NULL, 56, 400, 300, 55555, 64, 16, 10, 1, 32, 1, 0,
		FB_VMODE_DOUBLE },

	/* 400x300 @ 60 Hz, 37.8 kHz hsync */
	{ NULL, 60, 400, 300, 50000, 48, 16, 11, 1, 64, 2, 0,
		FB_VMODE_DOUBLE },

	/* 400x300 @ 72 Hz, 48.0 kHz hsync */
	{ NULL, 72, 400, 300, 40000, 32, 24, 11, 19, 64, 3,	0,
		FB_VMODE_DOUBLE },

	/* 480x300 @ 56 Hz, 35.2 kHz hsync, 8:5 aspect ratio */
	{ NULL, 56, 480, 300, 46176, 80, 16, 10, 1, 40, 1, 0,
		FB_VMODE_DOUBLE },

	/* 480x300 @ 60 Hz, 37.8 kHz hsync */
	{ NULL, 60, 480, 300, 41858, 56, 16, 11, 1, 80, 2, 0,
		FB_VMODE_DOUBLE },

	/* 480x300 @ 63 Hz, 39.6 kHz hsync */
	{ NULL, 63, 480, 300, 40000, 56, 16, 11, 1, 80, 2, 0,
		FB_VMODE_DOUBLE },

	/* 480x300 @ 72 Hz, 48.0 kHz hsync */
	{ NULL, 72, 480, 300, 33386, 40, 24, 11, 19, 80, 3, 0,
		FB_VMODE_DOUBLE },

	/* 1920x1200 @ 60 Hz, 74.5 Khz hsync */
	{ NULL, 60, 1920, 1200, 5177, 128, 336, 1, 38, 208, 3,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED },

	/* 1152x768, 60 Hz, PowerBook G4 Titanium I and II */
	{ NULL, 60, 1152, 768, 14047, 158, 26, 29, 3, 136, 6,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED },

	/* 1366x768, 60 Hz, 47.403 kHz hsync, WXGA 16:9 aspect ratio */
	{ NULL, 60, 1366, 768, 13806, 120, 10, 14, 3, 32, 5, 0,
		FB_VMODE_NONINTERLACED },

	/* 1280x800, 60 Hz, 47.403 kHz hsync, WXGA 16:10 aspect ratio */
	{ NULL, 60, 1280, 800, 12048, 200, 64, 24, 1, 136, 3, 0,
		FB_VMODE_NONINTERLACED },

	/*1280x800-60Hz */
	 { NULL, 60, 1280, 800, 11976, 200, 72, 22, 3, 128, 6,
	  FB_SYNC_HOR_HIGH_ACT,FB_VMODE_NONINTERLACED},

	/* 720x576i @ 50 Hz, 15.625 kHz hsync (PAL RGB) */
	{ NULL, 50, 720, 576, 74074, 64, 16, 39, 5, 64, 5, 0,
		FB_VMODE_INTERLACED },

	/* 800x520i @ 50 Hz, 15.625 kHz hsync (PAL RGB) */
	{ NULL, 50, 800, 520, 58823, 144, 64, 72, 28, 80, 5, 0,
		FB_VMODE_INTERLACED },

	/* 864x480 @ 60 Hz, 35.15 kHz hsync */
	{ NULL, 60, 864, 480, 27777, 1, 1, 1, 1, 0, 0,
		0, FB_VMODE_NONINTERLACED },
};

EXPORT_SYMBOL(mxc_standard_modes);


const struct fb_videomode mxc_vesa_modes[MXC_VESA_MODEDB_SIZE] = {
	/* 0 640x350-85 VESA */
	{ NULL, 85, 640, 350, 31746,  96, 32, 60, 32, 64, 3,
	  FB_SYNC_HOR_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA},
	/* 1 640x400-85 VESA */
	{ NULL, 85, 640, 400, 31746,  96, 32, 41, 01, 64, 3,
	  FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 2 720x400-85 VESA */
	{ NULL, 85, 721, 400, 28169, 108, 36, 42, 01, 72, 3,
	  FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 3 640x480-60 VESA */
	{ NULL, 60, 640, 480, 39682,  48, 16, 33, 10, 96, 2,
	  0, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 4 640x480-72 VESA */
	{ NULL, 72, 640, 480, 31746, 128, 24, 29, 9, 40, 2,
	  0, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 5 640x480-75 VESA */
	{ NULL, 75, 640, 480, 31746, 120, 16, 16, 01, 64, 3,
	  0, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 6 640x480-85 VESA */
	{ NULL, 85, 640, 480, 27777, 80, 56, 25, 01, 56, 3,
	  0, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 7 800x600-56 VESA */
	{ NULL, 56, 800, 600, 27777, 128, 24, 22, 01, 72, 2,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 8 800x600-60 VESA */
	{ NULL, 60, 800, 600, 25000, 88, 40, 23, 01, 128, 4,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 9 800x600-72 VESA */
	{ NULL, 72, 800, 600, 20000, 64, 56, 23, 37, 120, 6,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 10 800x600-75 VESA */
	{ NULL, 75, 800, 600, 20202, 160, 16, 21, 01, 80, 3,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 11 800x600-85 VESA */
	{ NULL, 85, 800, 600, 17761, 152, 32, 27, 01, 64, 3,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
        /* 12 1024x768i-43 VESA */
	{ NULL, 43, 1024, 768, 22271, 56, 8, 41, 0, 176, 8,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_INTERLACED, FB_MODE_IS_VESA },
	/* 13 1024x768-60 VESA */
	{ NULL, 60, 1024, 768, 15384, 160, 24, 29, 3, 136, 6,
	  0, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 14 1024x768-70 VESA */
	{ NULL, 70, 1024, 768, 13333, 144, 24, 29, 3, 136, 6,
	  0, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 15 1024x768-75 VESA */
	{ NULL, 75, 1024, 768, 12690, 176, 16, 28, 1, 96, 3,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 16 1024x768-85 VESA */
	{ NULL, 85, 1024, 768, 10582, 208, 48, 36, 1, 96, 3,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 17 1152x864-75 VESA */
	{ NULL, 75, 1152, 864, 9259, 256, 64, 32, 1, 128, 3,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 18 1280x960-60 VESA */
	{ NULL, 60, 1280, 960, 9259, 312, 96, 36, 1, 112, 3,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 19 1280x960-85 VESA */
	{ NULL, 85, 1280, 960, 6734, 224, 64, 47, 1, 160, 3,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 20 1280x1024-60 VESA */
	{ NULL, 60, 1280, 1024, 9259, 248, 48, 38, 1, 112, 3,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 21 1280x1024-75 VESA */
	{ NULL, 75, 1280, 1024, 7407, 248, 16, 38, 1, 144, 3,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 22 1280x1024-85 VESA */
	{ NULL, 85, 1280, 1024, 6349, 224, 64, 44, 1, 160, 3,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 23 1600x1200-60 VESA */
	{ NULL, 60, 1600, 1200, 6172, 304, 64, 46, 1, 192, 3,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 24 1600x1200-65 VESA */
	{ NULL, 65, 1600, 1200, 5698, 304,  64, 46, 1, 192, 3,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 25 1600x1200-70 VESA */
	{ NULL, 70, 1600, 1200, 5291, 304, 64, 46, 1, 192, 3,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 26 1600x1200-75 VESA */
	{ NULL, 75, 1600, 1200, 4938, 304, 64, 46, 1, 192, 3,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 27 1600x1200-85 VESA */
	{ NULL, 85, 1600, 1200, 4357, 304, 64, 46, 1, 192, 3,
	  FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
	  FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 28 1792x1344-60 VESA */
	{ NULL, 60, 1792, 1344, 4882, 328, 128, 46, 1, 200, 3,
	  FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 29 1792x1344-75 VESA */
	{ NULL, 75, 1792, 1344, 3831, 352, 96, 69, 1, 216, 3,
	  FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 30 1856x1392-60 VESA */
	{ NULL, 60, 1856, 1392, 4580, 352, 96, 43, 1, 224, 3,
	  FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 31 1856x1392-75 VESA */
	{ NULL, 75, 1856, 1392, 3472, 352, 128, 104, 1, 224, 3,
	  FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 32 1920x1440-60 VESA */
	{ NULL, 60, 1920, 1440, 4273, 344, 128, 56, 1, 200, 3,
	  FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 33 1920x1440-75 VESA */
	{ NULL, 75, 1920, 1440, 3367, 352, 144, 56, 1, 224, 3,
	  FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 34 1920x1200-60 RB VESA */
	{ NULL, 60, 1920, 1200, 6493, 80, 48, 26, 3, 32, 6,
	  FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 35 1920x1200-60 VESA */
	{ NULL, 60, 1920, 1200, 5174, 336, 136, 36, 3, 200, 6,
	  FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 36 1920x1200-75 VESA */
	{ NULL, 75, 1920, 1200, 4077, 344, 136, 46, 3, 208, 6,
	  FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 37 1920x1200-85 VESA */
	{ NULL, 85, 1920, 1200, 3555, 352, 144, 53, 3, 208, 6,
	  FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 38 2560x1600-60 RB VESA */
	{ NULL, 60, 2560, 1600, 3724, 80, 48, 37, 3, 32, 6,
	  FB_SYNC_HOR_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 39 2560x1600-60 VESA */
	{ NULL, 60, 2560, 1600, 2869, 472, 192, 49, 3, 280, 6,
	  FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 40 2560x1600-75 VESA */
	{ NULL, 75, 2560, 1600, 2256, 488, 208, 63, 3, 280, 6,
	  FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 41 2560x1600-85 VESA */
	{ NULL, 85, 2560, 1600, 1979, 488, 208, 73, 3, 280, 6,
	  FB_SYNC_VERT_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
	/* 42 2560x1600-120 RB VESA */
	{ NULL, 120, 2560, 1600, 1809, 80, 48, 85, 3, 32, 6,
	  FB_SYNC_HOR_HIGH_ACT, FB_VMODE_NONINTERLACED, FB_MODE_IS_VESA },
};

/* Display Monitor Timings (DMT) are a list of VESA standard pre-defined timings
which are commonly used within the Computer industry (for computer monitors).*/
const struct dmt_videomode mxc_dmt_modes[MXC_DMT_MODEDB_SIZE] = {
	{ 0x01, 0x0000, 0x000000, &mxc_vesa_modes[0] },
	{ 0x02, 0x3119, 0x000000, &mxc_vesa_modes[1] },
	{ 0x03, 0x0000, 0x000000, &mxc_vesa_modes[2] },
	{ 0x04, 0x3140, 0x000000, &mxc_vesa_modes[3] },
	{ 0x05, 0x314c, 0x000000, &mxc_vesa_modes[4] },
	{ 0x06, 0x314f, 0x000000, &mxc_vesa_modes[5] },
	{ 0x07, 0x3159, 0x000000, &mxc_vesa_modes[6] },
	{ 0x08, 0x0000, 0x000000, &mxc_vesa_modes[7] },
	{ 0x09, 0x4540, 0x000000, &mxc_vesa_modes[8] },
	{ 0x0a, 0x454c, 0x000000, &mxc_vesa_modes[9] },
	{ 0x0b, 0x454f, 0x000000, &mxc_vesa_modes[10] },
	{ 0x0c, 0x4559, 0x000000, &mxc_vesa_modes[11] },
	{ 0x0d, 0x0000, 0x000000, NULL },
	{ 0x0e, 0x0000, 0x000000, NULL },
	{ 0x0f, 0x0000, 0x000000, &mxc_vesa_modes[12] },
	{ 0x10, 0x6140, 0x000000, &mxc_vesa_modes[13] },
	{ 0x11, 0x614a, 0x000000, &mxc_vesa_modes[14] },
	{ 0x12, 0x614f, 0x000000, &mxc_vesa_modes[15] },
	{ 0x13, 0x6159, 0x000000, &mxc_vesa_modes[16] },
	{ 0x14, 0x0000, 0x000000, NULL },
	{ 0x15, 0x714f, 0x000000, &mxc_vesa_modes[17] },
	{ 0x16, 0x0000, 0x7f1c21, NULL },
	{ 0x17, 0x0000, 0x7f1c28, NULL },
	{ 0x18, 0x0000, 0x7f1c44, NULL },
	{ 0x19, 0x0000, 0x7f1c62, NULL },
	{ 0x1a, 0x0000, 0x000000, NULL },
	{ 0x1b, 0x0000, 0x8f1821, NULL },
	{ 0x1c, 0x8100, 0x8f1828, NULL },
	{ 0x1d, 0x810f, 0x8f1844, NULL },
	{ 0x1e, 0x8119, 0x8f1862, NULL },
	{ 0x1f, 0x0000, 0x000000, NULL },
	{ 0x20, 0x8140, 0x000000, &mxc_vesa_modes[18] },
	{ 0x21, 0x8159, 0x000000, &mxc_vesa_modes[19] },
	{ 0x22, 0x0000, 0x000000, NULL },
	{ 0x23, 0x8180, 0x000000, &mxc_vesa_modes[20] },
	{ 0x24, 0x818f, 0x000000, &mxc_vesa_modes[21] },
	{ 0x25, 0x8199, 0x000000, &mxc_vesa_modes[22] },
	{ 0x26, 0x0000, 0x000000, NULL },
	{ 0x27, 0x0000, 0x000000, NULL },
	{ 0x28, 0x0000, 0x000000, NULL },
	{ 0x29, 0x0000, 0x0c2021, NULL },
	{ 0x2a, 0x9040, 0x0c2028, NULL },
	{ 0x2b, 0x904f, 0x0c2044, NULL },
	{ 0x2c, 0x9059, 0x0c2062, NULL },
	{ 0x2d, 0x0000, 0x000000, NULL },
	{ 0x2e, 0x9500, 0xc11821, NULL },
	{ 0x2f, 0x9500, 0xc11828, NULL },
	{ 0x30, 0x950f, 0xc11844, NULL },
	{ 0x31, 0x9519, 0xc11868, NULL },
	{ 0x32, 0x0000, 0x000000, NULL },
	{ 0x33, 0xa940, 0x000000, &mxc_vesa_modes[23] },
	{ 0x34, 0xa945, 0x000000, &mxc_vesa_modes[24] },
	{ 0x35, 0xa94a, 0x000000, &mxc_vesa_modes[25] },
	{ 0x36, 0xa94f, 0x000000, &mxc_vesa_modes[26] },
	{ 0x37, 0xa959, 0x000000, &mxc_vesa_modes[27] },
	{ 0x38, 0x0000, 0x000000, NULL },
	{ 0x39, 0x0000, 0x0c2821, NULL },
	{ 0x3a, 0xb300, 0x0c2828, NULL },
	{ 0x3b, 0xb30f, 0x0c2844, NULL },
	{ 0x3c, 0xb319, 0x0c2868, NULL },
	{ 0x3d, 0x0000, 0x000000, NULL },
	{ 0x3e, 0xc140, 0x000000, &mxc_vesa_modes[28] },
	{ 0x3f, 0xc14f, 0x000000, &mxc_vesa_modes[29] },
	{ 0x40, 0x0000, 0x000000, NULL},
	{ 0x41, 0xc940, 0x000000, &mxc_vesa_modes[30] },
	{ 0x42, 0xc94f, 0x000000, &mxc_vesa_modes[31] },
	{ 0x43, 0x0000, 0x000000, NULL },
	{ 0x44, 0x0000, 0x572821, &mxc_vesa_modes[34] },
	{ 0x45, 0xd100, 0x572828, &mxc_vesa_modes[35] },
	{ 0x46, 0xd10f, 0x572844, &mxc_vesa_modes[36] },
	{ 0x47, 0xd119, 0x572862, &mxc_vesa_modes[37] },
	{ 0x48, 0x0000, 0x000000, NULL },
	{ 0x49, 0xd140, 0x000000, &mxc_vesa_modes[32] },
	{ 0x4a, 0xd14f, 0x000000, &mxc_vesa_modes[33] },
	{ 0x4b, 0x0000, 0x000000, NULL },
	{ 0x4c, 0x0000, 0x1f3821, &mxc_vesa_modes[38] },
	{ 0x4d, 0x0000, 0x1f3828, &mxc_vesa_modes[39] },
	{ 0x4e, 0x0000, 0x1f3844, &mxc_vesa_modes[40] },
	{ 0x4f, 0x0000, 0x1f3862, &mxc_vesa_modes[41] },
	{ 0x50, 0x0000, 0x000000, &mxc_vesa_modes[42] },
};
EXPORT_SYMBOL(mxc_dmt_modes);

/* set of default timings for TVs*/
const struct fb_videomode mxc_cea_mode[64] = {
	/* #1: 640x480p@59.94/60Hz 4:3 */
	[1] = {
		NULL, 60, 640, 480, 39722, 48, 16, 33, 10, 96, 2, 0,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_4_3, 0,
	},
	/* #2: 720x480p@59.94/60Hz 4:3 */
	[2] = {
		NULL, 60, 720, 480, 37037, 60, 16, 30, 9, 62, 6, 0,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_4_3, 0,
	},
	/* #3: 720x480p@59.94/60Hz 16:9 */
	[3] = {
		NULL, 60, 720, 480, 37037, 60, 16, 30, 9, 62, 6, 0,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_16_9, 0,
	},
	/* #4: 1280x720p@59.94/60Hz 16:9 */
	[4] = {
		NULL, 60, 1280, 720, 13468, 220, 110, 20, 5, 40, 5,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_16_9, 0
	},
	/* #5: 1920x1080i@59.94/60Hz 16:9 */
	[5] = {
		NULL, 60, 1920, 1080, 13763, 148, 88, 15, 2, 44, 5,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_INTERLACED | FB_VMODE_ASPECT_16_9, 0,
	},
	/* #6: 720(1440)x480iH@59.94/60Hz 4:3 */
	[6] = {
		NULL, 60, 1440, 480, 18554/*37108*/, 114, 38, 15, 4, 124, 3, 0,
		FB_VMODE_INTERLACED | FB_VMODE_ASPECT_4_3, 0,
	},
	/* #7: 720(1440)x480iH@59.94/60Hz 16:9 */
	[7] = {
		NULL, 60, 1440, 480, 18554/*37108*/, 114, 38, 15, 4, 124, 3, 0,
		FB_VMODE_INTERLACED | FB_VMODE_ASPECT_16_9, 0,
	},
	/* #8: 720(1440)x240pH@59.94/60Hz 4:3 */
	[8] = {
		NULL, 60, 1440, 240, 37108, 114, 38, 15, 4, 124, 3, 0,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_4_3, 0,
	},
	/* #9: 720(1440)x240pH@59.94/60Hz 16:9 */
	[9] = {
		NULL, 60, 1440, 240, 37108, 114, 38, 15, 4, 124, 3, 0,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_16_9, 0,
	},
	/* #14: 1440x480p@59.94/60Hz 4:3 */
	[14] = {
		NULL, 60, 1440, 480, 18500, 120, 32, 30, 9, 124, 6, 0,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_4_3, 0,
	},
	/* #15: 1440x480p@59.94/60Hz 16:9 */
	[15] = {
		NULL, 60, 1440, 480, 18500, 120, 32, 30, 9, 124, 6, 0,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_16_9, 0,
	},
	/* #16: 1920x1080p@60Hz 16:9 */
	[16] = {
		NULL, 60, 1920, 1080, 6734, 148, 88, 36, 4, 44, 5,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_16_9, 0,
	},
	/* #17: 720x576pH@50Hz 4:3 */
	[17] = {
		NULL, 50, 720, 576, 37037, 68, 12, 39, 5, 64, 5, 0,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_4_3, 0,
	},
	/* #18: 720x576pH@50Hz 16:9 */
	[18] = {
		NULL, 50, 720, 576, 37037, 68, 12, 39, 5, 64, 5, 0,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_16_9, 0,
	},
	/* #19: 1280x720p@50Hz */
	[19] = {
		NULL, 50, 1280, 720, 13468, 220, 440, 20, 5, 40, 5,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_16_9, 0,
	},
	/* #20: 1920x1080i@50Hz */
	[20] = {
		NULL, 50, 1920, 1080, 13480, 148, 528, 15, 5, 528, 5,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_INTERLACED | FB_VMODE_ASPECT_16_9, 0,
	},
	/* #23: 720(1440)x288pH@50Hz 4:3 */
	[23] = {
		NULL, 50, 1440, 288, 37037, 138, 24, 19, 2, 126, 3, 0,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_4_3, 0,
	},
	/* #24: 720(1440)x288pH@50Hz 16:9 */
	[24] = {
		NULL, 50, 1440, 288, 37037, 138, 24, 19, 2, 126, 3, 0,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_16_9, 0,
	},
	/* #29: 720(1440)x576pH@50Hz 4:3 */
	[29] = {
		NULL, 50, 1440, 576, 18518, 136, 24, 39, 5, 128, 5, 0,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_4_3, 0,
	},
	/* #30: 720(1440)x576pH@50Hz 16:9 */
	[30] = {
		NULL, 50, 1440, 576, 18518, 136, 24, 39, 5, 128, 5, 0,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_16_9, 0,
	},
	/* #31: 1920x1080p@50Hz */
	[31] = {
		NULL, 50, 1920, 1080, 6734, 148, 528, 36, 4, 44, 5,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_16_9, 0,
	},
	/* #32: 1920x1080p@23.98/24Hz */
	[32] = {
		NULL, 24, 1920, 1080, 13468, 148, 638, 36, 4, 44, 5,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_16_9, 0,
	},
	/* #33: 1920x1080p@25Hz */
	[33] = {
		NULL, 25, 1920, 1080, 13468, 148, 528, 36, 4, 44, 5,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_16_9, 0,
	},
	/* #34: 1920x1080p@30Hz */
	[34] = {
		NULL, 30, 1920, 1080, 13468, 148, 88, 36, 4, 44, 5,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_16_9, 0,
	},
	/* #41: 1280x720p@100Hz 16:9 */
	[41] = {
		NULL, 100, 1280, 720, 6734, 220, 440, 20, 5, 40, 5,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_16_9, 0
	},
	/* #47: 1280x720p@119.88/120Hz 16:9 */
	[47] = {
		NULL, 120, 1280, 720, 6734, 220, 110, 20, 5, 40, 5,
		FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		FB_VMODE_NONINTERLACED | FB_VMODE_ASPECT_16_9, 0
	},
};




/*
 * We have a special version of fb_mode_is_equal that ignores
 * pixclock, since for many CEA modes, 2 frequencies are supported
 * e.g. 640x480 @ 60Hz or 59.94Hz
 */
int mxc_edid_fb_mode_is_equal(bool use_aspect,
			const struct fb_videomode *mode1,
			const struct fb_videomode *mode2)
{
	u32 mask;

	if (use_aspect)
		mask = ~0;
	else
		mask = ~FB_VMODE_ASPECT_MASK;

	return (mode1->xres         == mode2->xres &&
		mode1->yres         == mode2->yres &&
		mode1->hsync_len    == mode2->hsync_len &&
		mode1->vsync_len    == mode2->vsync_len &&
		mode1->left_margin  == mode2->left_margin &&
		mode1->right_margin == mode2->right_margin &&
		mode1->upper_margin == mode2->upper_margin &&
		mode1->lower_margin == mode2->lower_margin &&
		mode1->sync         == mode2->sync &&
		/* refresh check, 59.94Hz and 60Hz have the same parameter
		 * in struct of mxc_cea_mode */
		abs(mode1->refresh - mode2->refresh) <= 1 &&
		(mode1->vmode & mask) == (mode2->vmode & mask));
}

static void get_detailed_timing(unsigned char *block,
				struct fb_videomode *mode)
{
	mode->xres = H_ACTIVE;
	mode->yres = V_ACTIVE;
	mode->pixclock = PIXEL_CLOCK;
	mode->pixclock /= 1000;
	mode->pixclock = KHZ2PICOS(mode->pixclock);
	mode->right_margin = H_SYNC_OFFSET;
	mode->left_margin = (H_ACTIVE + H_BLANKING) -
		(H_ACTIVE + H_SYNC_OFFSET + H_SYNC_WIDTH);
	mode->upper_margin = V_BLANKING - V_SYNC_OFFSET -
		V_SYNC_WIDTH;
	mode->lower_margin = V_SYNC_OFFSET;
	mode->hsync_len = H_SYNC_WIDTH;
	mode->vsync_len = V_SYNC_WIDTH;
	if (HSYNC_POSITIVE)
		mode->sync |= FB_SYNC_HOR_HIGH_ACT;
	if (VSYNC_POSITIVE)
		mode->sync |= FB_SYNC_VERT_HIGH_ACT;
	mode->refresh = PIXEL_CLOCK/((H_ACTIVE + H_BLANKING) *
				     (V_ACTIVE + V_BLANKING));
	if (INTERLACED) {
		mode->yres *= 2;
		mode->upper_margin *= 2;
		mode->lower_margin *= 2;
		mode->vsync_len *= 2;
		mode->vmode |= FB_VMODE_INTERLACED;
	}
	mode->flag = FB_MODE_IS_DETAILED;

	if ((H_SIZE / 16) == (V_SIZE / 9))
		mode->vmode |= FB_VMODE_ASPECT_16_9;
	else if ((H_SIZE / 4) == (V_SIZE / 3))
		mode->vmode |= FB_VMODE_ASPECT_4_3;
	else if ((mode->xres / 16) == (mode->yres / 9))
		mode->vmode |= FB_VMODE_ASPECT_16_9;
	else if ((mode->xres / 4) == (mode->yres / 3))
		mode->vmode |= FB_VMODE_ASPECT_4_3;

	if (mode->vmode & FB_VMODE_ASPECT_16_9)
		DPRINTK("Aspect ratio: 16:9\n");
	if (mode->vmode & FB_VMODE_ASPECT_4_3)
		DPRINTK("Aspect ratio: 4:3\n");
	DPRINTK("      %d MHz ",  PIXEL_CLOCK/1000000);
	DPRINTK("%d %d %d %d ", H_ACTIVE, H_ACTIVE + H_SYNC_OFFSET,
	       H_ACTIVE + H_SYNC_OFFSET + H_SYNC_WIDTH, H_ACTIVE + H_BLANKING);
	DPRINTK("%d %d %d %d ", V_ACTIVE, V_ACTIVE + V_SYNC_OFFSET,
	       V_ACTIVE + V_SYNC_OFFSET + V_SYNC_WIDTH, V_ACTIVE + V_BLANKING);
	DPRINTK("%sHSync %sVSync\n\n", (HSYNC_POSITIVE) ? "+" : "-",
	       (VSYNC_POSITIVE) ? "+" : "-");
}

int mxc_edid_parse_ext_blk(unsigned char *edid,
		struct mxc_edid_cfg *cfg,
		struct fb_monspecs *specs)
{
	char detail_timing_desc_offset;
	struct fb_videomode *mode, *m;
	unsigned char index = 0x0;
	unsigned char *block;
	int i, num = 0, revision;

	if (edid[index++] != 0x2) /* only support cea ext block now */
		return 0;
	revision = edid[index++];
	DPRINTK("cea extent revision %d\n", revision);
	mode = kzalloc(50 * sizeof(struct fb_videomode), GFP_KERNEL);
	if (mode == NULL)
		return -1;

	detail_timing_desc_offset = edid[index++];

	if (revision >= 2) {
		cfg->cea_underscan = (edid[index] >> 7) & 0x1;
		cfg->cea_basicaudio = (edid[index] >> 6) & 0x1;
		cfg->cea_ycbcr444 = (edid[index] >> 5) & 0x1;
		cfg->cea_ycbcr422 = (edid[index] >> 4) & 0x1;

		DPRINTK("CEA underscan %d\n", cfg->cea_underscan);
		DPRINTK("CEA basicaudio %d\n", cfg->cea_basicaudio);
		DPRINTK("CEA ycbcr444 %d\n", cfg->cea_ycbcr444);
		DPRINTK("CEA ycbcr422 %d\n", cfg->cea_ycbcr422);
	}

	if (revision >= 3) {
		/* short desc */
		DPRINTK("CEA Short desc timmings\n");
		index++;
		while (index < detail_timing_desc_offset) {
			unsigned char tagcode, blklen;

			tagcode = (edid[index] >> 5) & 0x7;
			blklen = (edid[index]) & 0x1f;

			DPRINTK("Tagcode %x Len %d\n", tagcode, blklen);

			switch (tagcode) {
			case 0x2: /*Video data block*/
				{
					int cea_idx;
					i = 0;
					while (i < blklen) {
						index++;
						cea_idx = edid[index] & 0x7f;
						if (cea_idx < ARRAY_SIZE(mxc_cea_mode) &&
								(mxc_cea_mode[cea_idx].xres)) {
							DPRINTK("Support CEA Format #%d\n", cea_idx);
							mode[num] = mxc_cea_mode[cea_idx];
							mode[num].flag |= FB_MODE_IS_STANDARD;
							num++;
						}
						i++;
					}
					break;
				}
			case 0x3: /*Vendor specific data*/
				{
					unsigned char IEEE_reg_iden[3];
					unsigned char deep_color;
					unsigned char latency_present;
					unsigned char I_latency_present;
					unsigned char hdmi_video_present;
					unsigned char hdmi_3d_present;
					unsigned char hdmi_3d_multi_present;
					unsigned char hdmi_vic_len;
					unsigned char hdmi_3d_len;
					unsigned char index_inc = 0;
					unsigned char vsd_end;

					vsd_end = index + blklen;

					IEEE_reg_iden[0] = edid[index+1];
					IEEE_reg_iden[1] = edid[index+2];
					IEEE_reg_iden[2] = edid[index+3];
					cfg->physical_address[0] = (edid[index+4] & 0xf0) >> 4;
					cfg->physical_address[1] = (edid[index+4] & 0x0f);
					cfg->physical_address[2] = (edid[index+5] & 0xf0) >> 4;
					cfg->physical_address[3] = (edid[index+5] & 0x0f);

					if ((IEEE_reg_iden[0] == 0x03) &&
							(IEEE_reg_iden[1] == 0x0c) &&
							(IEEE_reg_iden[2] == 0x00))
						cfg->hdmi_cap = 1;

					if (blklen > 5) {
						deep_color = edid[index+6];
						if (deep_color & 0x80)
							cfg->vsd_support_ai = true;
						if (deep_color & 0x40)
							cfg->vsd_dc_48bit = true;
						if (deep_color & 0x20)
							cfg->vsd_dc_36bit = true;
						if (deep_color & 0x10)
							cfg->vsd_dc_30bit = true;
						if (deep_color & 0x08)
							cfg->vsd_dc_y444 = true;
						if (deep_color & 0x01)
							cfg->vsd_dvi_dual = true;
					}

					DPRINTK("VSD hdmi capability %d\n", cfg->hdmi_cap);
					DPRINTK("VSD support ai %d\n", cfg->vsd_support_ai);
					DPRINTK("VSD support deep color 48bit %d\n", cfg->vsd_dc_48bit);
					DPRINTK("VSD support deep color 36bit %d\n", cfg->vsd_dc_36bit);
					DPRINTK("VSD support deep color 30bit %d\n", cfg->vsd_dc_30bit);
					DPRINTK("VSD support deep color y444 %d\n", cfg->vsd_dc_y444);
					DPRINTK("VSD support dvi dual %d\n", cfg->vsd_dvi_dual);

					if (blklen > 6)
						cfg->vsd_max_tmdsclk_rate = edid[index+7] * 5;
					DPRINTK("VSD MAX TMDS CLOCK RATE %d\n", cfg->vsd_max_tmdsclk_rate);

					if (blklen > 7) {
						latency_present = edid[index+8] >> 7;
						I_latency_present =  (edid[index+8] & 0x40) >> 6;
						hdmi_video_present = (edid[index+8] & 0x20) >> 5;
						cfg->vsd_cnc3 = (edid[index+8] & 0x8) >> 3;
						cfg->vsd_cnc2 = (edid[index+8] & 0x4) >> 2;
						cfg->vsd_cnc1 = (edid[index+8] & 0x2) >> 1;
						cfg->vsd_cnc0 = edid[index+8] & 0x1;

						DPRINTK("VSD cnc0 %d\n", cfg->vsd_cnc0);
						DPRINTK("VSD cnc1 %d\n", cfg->vsd_cnc1);
						DPRINTK("VSD cnc2 %d\n", cfg->vsd_cnc2);
						DPRINTK("VSD cnc3 %d\n", cfg->vsd_cnc3);
						DPRINTK("latency_present %d\n", latency_present);
						DPRINTK("I_latency_present %d\n", I_latency_present);
						DPRINTK("hdmi_video_present %d\n", hdmi_video_present);

					} else {
						index += blklen;
						break;
					}

					index += 9;

					/*latency present */
					if (latency_present) {
						cfg->vsd_video_latency = edid[index++];
						cfg->vsd_audio_latency = edid[index++];

						if (I_latency_present) {
							cfg->vsd_I_video_latency = edid[index++];
							cfg->vsd_I_audio_latency = edid[index++];
						} else {
							cfg->vsd_I_video_latency = cfg->vsd_video_latency;
							cfg->vsd_I_audio_latency = cfg->vsd_audio_latency;
						}

						DPRINTK("VSD latency video_latency  %d\n", cfg->vsd_video_latency);
						DPRINTK("VSD latency audio_latency  %d\n", cfg->vsd_audio_latency);
						DPRINTK("VSD latency I_video_latency  %d\n", cfg->vsd_I_video_latency);
						DPRINTK("VSD latency I_audio_latency  %d\n", cfg->vsd_I_audio_latency);
					}

					if (hdmi_video_present) {
						hdmi_3d_present = edid[index] >> 7;
						hdmi_3d_multi_present = (edid[index] & 0x60) >> 5;
						index++;
						hdmi_vic_len = (edid[index] & 0xe0) >> 5;
						hdmi_3d_len = edid[index] & 0x1f;
						index++;

						DPRINTK("hdmi_3d_present %d\n", hdmi_3d_present);
						DPRINTK("hdmi_3d_multi_present %d\n", hdmi_3d_multi_present);
						DPRINTK("hdmi_vic_len %d\n", hdmi_vic_len);
						DPRINTK("hdmi_3d_len %d\n", hdmi_3d_len);

						if (hdmi_vic_len > 0) {
							for (i = 0; i < hdmi_vic_len; i++) {
								cfg->hdmi_vic[i] = edid[index++];
								DPRINTK("HDMI_vic=%d\n", cfg->hdmi_vic[i]);
							}
						}

						if (hdmi_3d_len > 0) {
							if (hdmi_3d_present) {
								if (hdmi_3d_multi_present == 0x1) {
									cfg->hdmi_3d_struct_all = (edid[index] << 8) | edid[index+1];
									index_inc = 2;
								} else if (hdmi_3d_multi_present == 0x2) {
									cfg->hdmi_3d_struct_all = (edid[index] << 8) | edid[index+1];
									cfg->hdmi_3d_mask_all = (edid[index+2] << 8) | edid[index+3];
									index_inc = 4;
								} else
									index_inc = 0;
							}

							DPRINTK("HDMI 3d struct all =0x%x\n", cfg->hdmi_3d_struct_all);
							DPRINTK("HDMI 3d mask all =0x%x\n", cfg->hdmi_3d_mask_all);

							/* Read 2D vic 3D_struct */
							if ((hdmi_3d_len - index_inc) > 0) {
								DPRINTK("Support 3D video format\n");
								i = 0;
								while ((hdmi_3d_len - index_inc) > 0) {

									cfg->hdmi_3d_format[i].vic_order_2d = edid[index+index_inc] >> 4;
									cfg->hdmi_3d_format[i].struct_3d = edid[index+index_inc] & 0x0f;
									index_inc++;

									if (cfg->hdmi_3d_format[i].struct_3d ==  8) {
										cfg->hdmi_3d_format[i].detail_3d = edid[index+index_inc] >> 4;
										index_inc++;
									} else if (cfg->hdmi_3d_format[i].struct_3d > 8) {
										cfg->hdmi_3d_format[i].detail_3d = 0;
										index_inc++;
									}

									DPRINTK("vic_order_2d=%d, 3d_struct=%d, 3d_detail=0x%x\n",
											cfg->hdmi_3d_format[i].vic_order_2d,
											cfg->hdmi_3d_format[i].struct_3d,
											cfg->hdmi_3d_format[i].detail_3d);
									i++;
								}
							}
							index += index_inc;
						}
					}

					index = vsd_end;

					break;
				}
			case 0x1: /*Audio data block*/
				{
					u8 audio_format, max_ch, byte1, byte2, byte3;

					i = 0;
					cfg->max_channels = 0;
					cfg->sample_rates = 0;
					cfg->sample_sizes = 0;

					while (i < blklen) {
						byte1 = edid[index + 1];
						byte2 = edid[index + 2];
						byte3 = edid[index + 3];
						index += 3;
						i += 3;

						audio_format = byte1 >> 3;
						max_ch = (byte1 & 0x07) + 1;

						DPRINTK("Audio Format Descriptor : %2d\n", audio_format);
						DPRINTK("Max Number of Channels  : %2d\n", max_ch);
						DPRINTK("Sample Rates            : %02x\n", byte2);

						/* ALSA can't specify specific compressed
						 * formats, so only care about PCM for now. */
						if (audio_format == AUDIO_CODING_TYPE_LPCM) {
							if (max_ch > cfg->max_channels)
								cfg->max_channels = max_ch;

							cfg->sample_rates |= byte2;
							cfg->sample_sizes |= byte3 & 0x7;
							DPRINTK("Sample Sizes            : %02x\n",
								byte3 & 0x7);
						}
					}
					break;
				}
			case 0x4: /*Speaker allocation block*/
				{
					i = 0;
					while (i < blklen) {
						cfg->speaker_alloc = edid[index + 1];
						index += 3;
						i += 3;
						DPRINTK("Speaker Alloc           : %02x\n", cfg->speaker_alloc);
					}
					break;
				}
			case 0x7: /*User extended block*/
			default:
				/* skip */
				DPRINTK("Not handle block, tagcode = 0x%x\n", tagcode);
				index += blklen;
				break;
			}

			index++;
		}
	}

	/* long desc */
	DPRINTK("CEA long desc timmings\n");
	index = detail_timing_desc_offset;
	block = edid + index;
	while (index < (EDID_LENGTH - DETAILED_TIMING_DESCRIPTION_SIZE)) {
		if (!(block[0] == 0x00 && block[1] == 0x00)) {
			get_detailed_timing(block, &mode[num]);
			num++;
		}
		block += DETAILED_TIMING_DESCRIPTION_SIZE;
		index += DETAILED_TIMING_DESCRIPTION_SIZE;
	}

	if (!num) {
		kfree(mode);
		return 0;
	}

	m = kmalloc((num + specs->modedb_len) *
			sizeof(struct fb_videomode), GFP_KERNEL);
	if (!m)
		return 0;

	if (specs->modedb_len) {
		memmove(m, specs->modedb,
			specs->modedb_len * sizeof(struct fb_videomode));
		kfree(specs->modedb);
	}
	memmove(m+specs->modedb_len, mode,
		num * sizeof(struct fb_videomode));
	kfree(mode);

	specs->modedb_len += num;
	specs->modedb = m;

	return 0;
}
EXPORT_SYMBOL(mxc_edid_parse_ext_blk);

static int mxc_edid_readblk(struct i2c_adapter *adp,
		unsigned short addr, unsigned char *edid)
{
	int ret = 0, extblknum = 0;
	unsigned char regaddr = 0x0;
	struct i2c_msg msg[2] = {
		{
		.addr	= addr,
		.flags	= 0,
		.len	= 1,
		.buf	= &regaddr,
		}, {
		.addr	= addr,
		.flags	= I2C_M_RD,
		.len	= EDID_LENGTH,
		.buf	= edid,
		},
	};

	ret = i2c_transfer(adp, msg, ARRAY_SIZE(msg));
	if (ret != ARRAY_SIZE(msg)) {
		DPRINTK("unable to read EDID block\n");
		return -EIO;
	}

	if (edid[1] == 0x00)
		return -ENOENT;

	extblknum = edid[0x7E];

	if (extblknum) {
		regaddr = 128;
		msg[1].buf = edid + EDID_LENGTH;

		ret = i2c_transfer(adp, msg, ARRAY_SIZE(msg));
		if (ret != ARRAY_SIZE(msg)) {
			DPRINTK("unable to read EDID ext block\n");
			return -EIO;
		}
	}

	return extblknum;
}

static int mxc_edid_readsegblk(struct i2c_adapter *adp, unsigned short addr,
			unsigned char *edid, int seg_num)
{
	int ret = 0;
	unsigned char segment = 0x1, regaddr = 0;
	struct i2c_msg msg[3] = {
		{
		.addr	= 0x30,
		.flags	= 0,
		.len	= 1,
		.buf	= &segment,
		}, {
		.addr	= addr,
		.flags	= 0,
		.len	= 1,
		.buf	= &regaddr,
		}, {
		.addr	= addr,
		.flags	= I2C_M_RD,
		.len	= EDID_LENGTH,
		.buf	= edid,
		},
	};

	ret = i2c_transfer(adp, msg, ARRAY_SIZE(msg));
	if (ret != ARRAY_SIZE(msg)) {
		DPRINTK("unable to read EDID block\n");
		return -EIO;
	}

	if (seg_num == 2) {
		regaddr = 128;
		msg[2].buf = edid + EDID_LENGTH;

		ret = i2c_transfer(adp, msg, ARRAY_SIZE(msg));
		if (ret != ARRAY_SIZE(msg)) {
			DPRINTK("unable to read EDID block\n");
			return -EIO;
		}
	}

	return ret;
}

int mxc_edid_var_to_vic(struct fb_var_screeninfo *var)
{
	int i;
	struct fb_videomode m;

	for (i = 0; i < ARRAY_SIZE(mxc_cea_mode); i++) {
		fb_var_to_videomode(&m, var);
		if (mxc_edid_fb_mode_is_equal(false, &m, &mxc_cea_mode[i]))
			break;
	}

	if (i == ARRAY_SIZE(mxc_cea_mode))
		return 0;

	return i;
}
EXPORT_SYMBOL(mxc_edid_var_to_vic);

int mxc_edid_to_vic(const struct fb_videomode *mode, const int video_group) {
	int i;
	bool use_aspect = (mode->vmode & FB_VMODE_ASPECT_MASK);

	switch (video_group) {
		case CEA_GROUP_MODE:
		for (i = 0; i < ARRAY_SIZE(mxc_cea_mode); i++) {
			if (mxc_edid_fb_mode_is_equal(use_aspect, mode, &mxc_cea_mode[i]))
				return i;
		}
		break;

		case DMT_GROUP_MODE:
		for (i = 0; i < ARRAY_SIZE(mxc_dmt_modes); i++) {
			if (mxc_dmt_modes[i].mode != NULL)
				if (mxc_edid_fb_mode_is_equal(use_aspect, mode, mxc_dmt_modes[i].mode))
					return mxc_dmt_modes[i].dmt_id;
			}

		break;

		case STANDARD_GROUP_MODE:
		for (i = 0; i < ARRAY_SIZE(mxc_standard_modes); i++) {
				if (mxc_edid_fb_mode_is_equal(use_aspect, mode, &mxc_standard_modes[i]))
					return i;
			}
		break;

	}

	return 0;

}

EXPORT_SYMBOL(mxc_edid_to_vic);

/* make sure edid has 512 bytes*/
int mxc_edid_read(struct i2c_adapter *adp, unsigned short addr,
	unsigned char *edid, struct mxc_edid_cfg *cfg, struct fb_info *fbi)
{
	int ret = 0, extblknum;
	if (!adp || !edid || !cfg || !fbi)
		return -EINVAL;

	memset(edid, 0, EDID_LENGTH*4);
	memset(cfg, 0, sizeof(struct mxc_edid_cfg));

	extblknum = mxc_edid_readblk(adp, addr, edid);
	if (extblknum < 0)
		return extblknum;

	/* edid first block parsing */
	memset(&fbi->monspecs, 0, sizeof(fbi->monspecs));
	fb_edid_to_monspecs(edid, &fbi->monspecs);

	if (extblknum) {
		int i;

		/* FIXME: mxc_edid_readsegblk() won't read more than 2 blocks
		 * and the for-loop will read past the end of the buffer! :-( */
		if (extblknum > 3) {
			WARN_ON(extblknum > 3);
			return -EINVAL;
		}

		/* need read segment block? */
		if (extblknum > 1) {
			ret = mxc_edid_readsegblk(adp, addr,
				edid + EDID_LENGTH*2, extblknum - 1);
			if (ret < 0)
				return ret;
		}

		for (i = 1; i <= extblknum; i++)
			/* edid ext block parsing */
			mxc_edid_parse_ext_blk(edid + i*EDID_LENGTH,
					cfg, &fbi->monspecs);
	}

	return 0;
}
EXPORT_SYMBOL(mxc_edid_read);

