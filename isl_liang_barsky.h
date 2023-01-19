#ifndef ISL_LIANG_BARSKY_H_
#define ISL_LIANG_BARSKY_H_

/* isl_liang_barsky - v0.1 public domain Liang-Barsky line clipping algorithm
                              no warranty implied; use at your own risk
   
   Do this:
       #define ISL_LIANG_BARSKY_IMPLEMENTATION
   before you include this file in *one* C or C++ file to create the implementation.

   To static link also add:
       #define ISL_LIANG_BARSKY_STATIC

	 By default all calculations is done in floats (isllb_float type), to use doubles do:
       #define ISL_LIANG_BARSKY_DOUBLE

   LINKS:
	     Liang, Y. D., and Barsky, B. A New Concept and Method for Line Clipping // ACM Transactions on Graphics, 3(1):1–22, January 1984.
       Liang, Y. D., B. A., Barsky, and M. Slater. Some Improvements to a Parametric Line Clipping Algorithm // CSD-92-688, Computer Science Division, University of California, Berkeley, 1992.

   USAGE:
      //                                    clipping rectangle    line segment
      //                                   xmin ymin xmax ymax   x1  y1  x2   y2
      struct isllb_result res = isllb_clip(100, 100, 500, 500,  50, 50, 650, 650);
			// res.inside -- 0 if line segment is outside of the clipping rectangle, 1 otherwise
			// res.xn1, res.yn1, res.xn2, res.yn2 -- clipped line segment

   author: Ilya Kolbin (iskolbin@gmail.com)
   url: https://github.com/iskolbin/isl_liang_barsky
   git: git@github.com:iskolbin/isl_liang_barsky

   LICENSE:
     See end of file for license information.
*/

#ifndef ISLLB_DEF
#ifdef ISL_LIANG_BARSKY_STATIC
#define ISLLB_DEF static
#else
#define ISLLB_DEF extern
#endif
#endif

#ifdef ISL_LIANG_BARSKY_DOUBLE
#define isllb_float double
#else
#define isllb_float float
#endif

struct isllb_clip_result {
	int inside;
	isllb_float xn1;
	isllb_float yn1;
	isllb_float xn2;
	isllb_float yn2;
};

#ifdef __cplusplus
extern "C" {
#endif

ISLLB_DEF struct isllb_clip_result isllb_clip(isllb_float xmin, isllb_float ymin, isllb_float xmax, isllb_float ymax, isllb_float x1, isllb_float y1, isllb_float x2, isllb_float y2);

#ifdef __cplusplus
}
#endif
#endif // ISL_LIANG_BARSKY_H_

#ifdef ISL_LIANG_BARSKY_IMPLEMENTATION
#ifndef ISL_LIANG_BARSKY_IMPLEMENTATION_ONCE
#define ISL_LIANG_BARSKY_IMPLEMENTATION_ONCE
#else
#error "ISL_LIANG_BARSKY_IMPLEMENTATION should be defined once"
#endif

struct isllb_clip_result isllb_clip(isllb_float xmin, isllb_float ymin, isllb_float xmax, isllb_float ymax, isllb_float x1, isllb_float y1, isllb_float x2, isllb_float y2) {
	isllb_float p1 = -(x2 - x1), p2 = -p1, p3 = -(y2 - y1), p4 = -p3;
	isllb_float q1 = x1 - xmin, q2 = xmax - x1, q3 = y1 - ymin, q4 = ymax - y1;
	struct isllb_clip_result result = {0};

	if ((p1 == 0 && q1 < 0) || (p2 == 0 && q2 < 0) || (p3 == 0 && q3 < 0) || (p4 == 0 && q4 < 0)) {
		return result;
	}

	isllb_float rn1 = 0;
	isllb_float rn2 = 1;	

	if (p1 != 0) {
		isllb_float r1 = q1 / p1;
		isllb_float r2 = q2 / p2;
		if (p1 < 0) {
			if (r1 > rn1) rn1 = r1;
			if (r2 < rn2) rn2 = r2;
		} else {
			if (r2 > rn1) rn1 = r2;
			if (r1 < rn2) rn2 = r1;
		}
	}
	if (p3 != 0) {
		isllb_float r1 = q3 / p3;
		isllb_float r2 = q4 / p4;
		if (p3 < 0) {
			if (r1 > rn1) rn1 = r1;
			if (r2 < rn2) rn2 = r2;
		} else {
			if (r2 > rn1) rn1 = r2;
			if (r1 < rn2) rn2 = r1;
		}
	}

	if (rn1 > rn2) return result;

	result.inside = 1;
	result.xn1 = x1 + p2 * rn1;
	result.yn1 = y1 + p4 * rn1;
	result.xn2 = x1 + p2 * rn2;
	result.yn2 = y1 + p4 * rn2;

	return result;
}
/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2023 Ilya Kolbin
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
#endif // ISL_LIANG_BARSKY_IMPLEMENTATION
