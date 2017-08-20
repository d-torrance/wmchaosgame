/* wmchaosgamae - Window Maker dockapp displaying a chaos game
 * Copyright (C) 2017 Doug Torrance <dtorrance@piedmont.edu>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#include <math.h>
#include <time.h>
#include <X11/Xlib.h>
#include <libdockapp/dockapp.h>

#define POINT_COLOR "light sea green"
#define DOCKAPP_WIDTH 56
#define DOCKAPP_HEIGHT 56
#define NUM_VERTICES 3
#define FRACTION 0.5
#define TIMEOUT 250
#define MAXPOINTS 2500

GC point_gc;
Pixmap pixmap;
int n = NUM_VERTICES;
float r = FRACTION;
int timeout = TIMEOUT;
int maxpoints = MAXPOINTS;
XPoint *vertices;
XPoint point;

void next_point(void);

int main(int argc, char **argv)
{
	int i;
	char *r_string = NULL;
	XGCValues values;
	DACallbacks eventCallbacks = {NULL, NULL, NULL, NULL, NULL, NULL,
				      next_point};
	DAProgramOption options[] = {
		{"-n", "--vertices",
		 "number of vertices (default: 3)",
		 DONatural, False, {&n}},
		{"-r", "--fraction",
		 "fraction of distance for next pt (default: 0.5)",
		 DOString, False, {&r_string}},
		{"-t", "--time",
		 "time (ms) before drawing next pt (default: 250)",
		 DONatural, False, {&timeout}},
		{"-m", "--maxpoints",
		 "max # of points before restarting (default: 2500)",
		 DONatural, False, {&maxpoints}},
	};

	srand(time(NULL));

	DAParseArguments(argc, argv, options, 4,
			 "Window Maker dockapp displaying a chaos game",
			 PACKAGE_STRING);
	DAInitialize(NULL, PACKAGE_NAME, DOCKAPP_WIDTH, DOCKAPP_HEIGHT,
		     argc, argv);
	DASetCallbacks(&eventCallbacks);
	DASetTimeout(timeout);

	if (r_string)
		r = atof(r_string);

	if (r <= 0 || r >= 1) {
		DAWarning("%f not between 0 and 1, using %f.", r, FRACTION);
		r = FRACTION;
	}

	pixmap = DAMakePixmap();

	XFillRectangle(DADisplay, pixmap, DAGC, 0, 0,
		       DOCKAPP_WIDTH, DOCKAPP_HEIGHT);

	values.foreground = DAGetColor(POINT_COLOR);
	point_gc = XCreateGC(DADisplay, pixmap, GCForeground, &values);

	vertices = malloc(n * sizeof(XPoint));
	for (i = 0; i < n; i++) {
		vertices[i].x = round(DOCKAPP_WIDTH/2 *
			(1 + cos(M_PI * (0.5 + 2.0 * i / n))));
		vertices[i].y = round(DOCKAPP_HEIGHT/2 *
			(1 - sin(M_PI * (0.5 + 2.0 * i / n))));
	}

	point.x = rand() % DOCKAPP_WIDTH;
	point.y = rand() % DOCKAPP_HEIGHT;

	XFillRectangle(DADisplay, pixmap, point_gc, point.x, point.y, 1, 1);
	DASetPixmap(pixmap);

	DAShow();
	DAEventLoop();

	return 0;
}

void next_point(void)
{
	int i;
	static int count = 0;

	i = rand() % n;
	point.x = round(point.x + r * (vertices[i].x - point.x));
	point.y = round(point.y + r * (vertices[i].y - point.y));

	XFillRectangle(DADisplay, pixmap, point_gc, point.x, point.y, 1, 1);
	DASetPixmap(pixmap);

	count++;
	if (count == maxpoints) {
		XFillRectangle(DADisplay, pixmap, DAGC, 0, 0,
			       DOCKAPP_WIDTH, DOCKAPP_HEIGHT);
		count = 0;
	}
}
