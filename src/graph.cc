/*
 * Copyright (C) 2007 Johan MATHE - johan.mathe@tremplin-utc.net - Centre
 * Pompidou - IRI This library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either version
 * 2.1 of the License, or (at your option) any later version. This
 * library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details. You should have received a copy of the GNU
 * Lesser General Public License along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA $Id: graph.cpp 131 2007-03-22 15:46:52Z
 * johmathe $ $Date: 2010-10-01 01:02:58 +0200 (Fri, 01 Oct 2010) $
 */
#include "src/graph.h"

using namespace std;


float
graph::MAX (float a, float b, float c)
{
  if ((a >= b) && (a >= c)) {
    return a;
  } else if ((b >= a) && (b >= c)) {
    return b;
  } else {
    return c;
  }
}

float
graph::MIN (float a, float b, float c)
{
  if ((a <= b) && (a <= c)) {
    return a;
  } else if ((b <= a) && (b <= c)) {
    return b;
  } else {
    return c;
  }
}

graph::graph (int x, int y, string path, int th, film * farg)
{
  this->f = farg;
  this->xsize = 600;
  this->ysize = y;
  this->xaxis_offset = 250;
  this->xoffset = 10;
  this->yoffset = 10;
  this->size_graduation = 3;
  this->grid_size = 10;
  this->threshold = th;
  this->global_path = path;
}

graph::~graph ()
{


}


void
graph::save ()
{
  gdImagePng (im_colors, fdgraph_colors);
  gdImagePng (im_motion_qty, fdmotion_qty);
  gdImagePng (im_hsv, fdgraph_hsv);
  gdImageDestroy (im_motion_qty);
  gdImageDestroy (im_colors);
  gdImageDestroy (im_hsv);
  fclose (fdgraph_hsv);
  fclose (fdgraph_colors);
  fclose (fdmotion_qty);
}

void
graph::init_gd ()
{
  graph_color canvas_graph_colors;
  graph_color motion_qty_graph_colors;
  graph_color rgb_graph_colors;
  graph_color hsv_graph_colors;

  if (data.size () > xsize) {
    xsize = data.size () + 20;
  }

  /*
   * Create image buffers
   *   Motion quantity graph: indexed colors
   *   RGB color graph: indexed colors
   *   HSV graph: true color
   */
  im_motion_qty = gdImageCreate (xsize, ysize);
  im_colors = gdImageCreate (xsize, ysize);
  im_hsv = gdImageCreateTrueColor (xsize, ysize);

  /*
   * Declare color indexes used for all images
   * NOTE: using "im_motion_qty" here is a hack: all common colors are indexed in
   *       the same order for each image. This is necessary to avoid conflicting 
   *       color-indexes.
   */
  canvas_graph_colors.background = gdImageColorAllocate (im_motion_qty, 255, 255, 255);
  canvas_graph_colors.line = gdImageColorAllocate (im_motion_qty, 0, 0, 0);
  canvas_graph_colors.title = gdImageColorAllocate (im_motion_qty, 0, 0, 0);
  canvas_graph_colors.grid = gdImageColorAllocate (im_motion_qty, 0, 0, 0);
  canvas_graph_colors.timecode = gdImageColorAllocate (im_motion_qty, 0, 0, 0);
  set_color(canvas_graph_colors);  // Add colorset. Index=0

  /*
   * Declare color indexes for motion quantity graph
   */
  motion_qty_graph_colors.background = gdImageColorAllocate (im_motion_qty, 255, 255, 255);
  motion_qty_graph_colors.line = gdImageColorAllocate (im_motion_qty, 0, 0, 0);
  motion_qty_graph_colors.title = gdImageColorAllocate (im_motion_qty, 0, 0, 0);
  motion_qty_graph_colors.grid = gdImageColorAllocate (im_motion_qty, 0, 0, 0);
  motion_qty_graph_colors.timecode = gdImageColorAllocate (im_motion_qty, 0, 0, 0);
  motion_qty_graph_colors.threshold = gdImageColorAllocate (im_motion_qty, 255, 0, 0);
  set_color(motion_qty_graph_colors);  // Add colorset. Index=1

  /*
   * Declare color indexes for the RGB color graph
   */
  rgb_graph_colors.background = gdImageColorAllocate (im_colors, 255, 255, 255);
  rgb_graph_colors.line = gdImageColorAllocate (im_colors, 0, 0, 0);
  rgb_graph_colors.title = gdImageColorAllocate (im_colors, 0, 0, 0);
  rgb_graph_colors.grid = gdImageColorAllocate (im_colors, 0, 0, 0);
  rgb_graph_colors.timecode = gdImageColorAllocate (im_colors, 0, 0, 0);
  rgb_graph_colors.red = gdImageColorAllocate (im_colors, 255, 0, 0);
  rgb_graph_colors.green = gdImageColorAllocate (im_colors, 0, 255, 0);
  rgb_graph_colors.blue = gdImageColorAllocate (im_colors, 0, 0, 255);
  set_color(rgb_graph_colors);  // Add colorset. Index=2

  /*
   * Declare color indexes for the HSV graph
   */
  hsv_graph_colors.background = gdTrueColor (255, 255, 255);
  hsv_graph_colors.line = gdTrueColor (0, 0, 0);
  hsv_graph_colors.title = gdTrueColor (0, 0, 0);
  hsv_graph_colors.grid = gdTrueColor (0, 0, 0);
  hsv_graph_colors.timecode = gdTrueColor (0, 0, 0);
  set_color(hsv_graph_colors);  // Add colorset. Index=3


  /*
   * Open file descriptors (POSIX C)
   */
  filename_colors = global_path + "/colors.png";
  filename_motion_qty = global_path + "/motion_qty.png";
  filename_hsv = global_path + "/hsv.png";
  fdgraph_hsv = fopen (filename_hsv.c_str (), "wb");
  fdgraph_colors = fopen (filename_colors.c_str (), "wb");
  fdmotion_qty = fopen (filename_motion_qty.c_str (), "wb");
}

void
graph::set_title (string title)
{

}


void
graph::draw_all_canvas ()
{

  printf("draw_all_canvas()\n"); //DELME
  /*
   * Create canvas for the graphs
   */
  string str = "RGB colors";
  draw_canvas (im_colors, str);
  str = "Quantity of movement";
  draw_canvas (im_motion_qty, str);


  gdImageFilledRectangle (im_hsv, 0, 0, xsize, ysize, graph_colors[IM_HSV_COLORS].background);


  string title = "HSV colorspace";
  gdImageString (im_hsv, gdFontGetLarge (), im_hsv->sx / 2 - (title.size () * gdFontGetLarge ()->w / 2), 10, (unsigned char *) title.c_str (), graph_colors[IM_HSV_COLORS].title);

  /*
   * Yaxis
   */
  gdImageLine (im_hsv, xoffset, yoffset, xoffset, ysize - yoffset, graph_colors[IM_HSV_COLORS].grid);

  /*
   * graduation
   */
  for (int i = yoffset; i < ysize - yoffset; i++) {
    if (!(i % grid_size)) {
      gdImageLine (im_hsv, xoffset - 2, i, xoffset + 2, i, graph_colors[IM_HSV_COLORS].grid);
    }

  }


  /*
   * Xaxis
   */
  printf("1"); //DELME
  gdImageLine (im_hsv, xoffset, xaxis_offset, xsize - xoffset, xaxis_offset, graph_colors[IM_CANVAS].line);

  for (int i = xoffset; i < xsize - xoffset; i++) {
    if (!(i % grid_size)) {
      gdImageLine (im_hsv, i, xaxis_offset - 2, i, xaxis_offset + 2, graph_colors[IM_CANVAS].grid);
    }
  }
}

void
graph::draw_canvas (gdImagePtr im, string title)
{
  int fps = this->f->get_fps();
  int tick_length;
  bool is_second, is_minute, is_hour;
  int hour, minute, second, frame;
  char timecode[10];
  int frames;

  printf("2"); //DELME
  gdImageString (im, gdFontGetLarge (), im->sx / 2 - (title.size () * gdFontGetLarge ()->w / 2), 10, (unsigned char *) title.c_str (), graph_colors[IM_CANVAS].title);

  //  Yaxis
  printf("3"); //DELME
  gdImageLine (im, xoffset, yoffset, xoffset, ysize - yoffset, graph_colors[IM_CANVAS].grid);

  // graduation
  for (int i = yoffset; i < ysize - yoffset; i++) {
    if (!(i % grid_size)) {
      gdImageLine (im, xoffset - 2, i, xoffset + 2, i, graph_colors[IM_CANVAS].grid);
    }
  }

  // Xaxis
  printf("4"); //DELME
  gdImageLine (im, xoffset, xaxis_offset, xsize - xoffset, xaxis_offset, graph_colors[IM_CANVAS].grid);

  for (int x = xoffset; x < xsize - xoffset; x++) {
    // Calculate video position information:
    frames = x - xoffset + 1;
    frame = (frames % fps);
    is_second = !frame;
    is_minute = false;
    is_hour = false;

    tick_length = 2;
    if (is_second) {
      second = (frames / fps) % 60;
      minute = (frames / fps / 60) % 60;
      hour = (int)(frames / fps / 60 / 60);

      is_minute = !second;
      is_hour = !minute;

      // Do something special every 10th second:
      if (!(second % 10)) {
        tick_length = 10;

        // Write video position as grid label:
        //printf("%d:%02d:%02d.%02d\n", hour, minute, second, frame);
        if (this->f->show_timecode == true) {
        sprintf(timecode, "%d:%02d:%02d.%02d", hour, minute, second, frame);

        gdImageString (im,
          gdFontGetLarge(),
          x - (strlen(timecode) * gdFontGetLarge()->w / 2),
          xaxis_offset + 8,
          (unsigned char *) timecode,
          graph_colors[IM_CANVAS].timecode
        );
        }
      }
      //if (!(i % grid_size)) //CF
      gdImageLine (im, x, xaxis_offset - 1, x, xaxis_offset + tick_length, graph_colors[IM_CANVAS].grid);
    }
  }
}


void
graph::draw_datas () {
  /*
   * Loop for creation of data axes in the graph images
   */
  for (int i = 1; i < data.size () - 1; i++) {
    gdImageLine (im_motion_qty, i - 1 + xoffset, (-data[i - 1].global) +xaxis_offset, i + xoffset, (-data[i].global) +xaxis_offset, graph_colors[IM_CANVAS].line);
  }
  /*
   * Draw the threshold line
   */
  gdImageLine (im_motion_qty, xoffset, xaxis_offset - threshold, xsize - xoffset, xaxis_offset - threshold, graph_colors[IM_MOTION_QTY].threshold);
}



void
graph::hsv_to_rgb (float *r, float *g, float *b, float h, float s, float v)
{
  int i;
  float f, p, q, t;

  if (s == 0) {
    // achromatic (grey)
    *r = *g = *b = v;
    return;
  }

  h /= 60;			// sector 0 to 5
  i = int (h);
  f = h - i;			// factorial part of h
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));

  switch (i) {
  case 0:
    *r = v;
    *g = t;
    *b = p;
    break;
  case 1:
    *r = q;
    *g = v;
    *b = p;
    break;
  case 2:
    *r = p;
    *g = v;
    *b = t;
    break;
  case 3:
    *r = p;
    *g = q;
    *b = v;
    break;
  case 4:
    *r = t;
    *g = p;
    *b = v;
    break;
  default:			// case 5:
    *r = v;
    *g = p;
    *b = q;
    break;
  }

}



void
graph::rgb_to_hsv (float r, float g, float b, float *h, float *s, float *v)
{
  float min, max, delta;

  min = MIN (r, g, b);
  max = MAX (r, g, b);
  *v = max;			// v

  delta = max - min;

  if (max != 0)
    *s = delta / max;		// s
  else {
    // r = g = b = 0 // s = 0, v is undefined
    *s = 0;
    *h = -1;
    return;
  }

  if (r == max)
    *h = (g - b) / delta;	// between yellow & magenta
  else if (g == max)
    *h = 2 + (b - r) / delta;	// between cyan & yellow
  else
    *h = 4 + (r - g) / delta;	// between magenta & cyan

  *h *= 60;			// degrees
  if (*h < 0)
    *h += 360;

}





void
graph::draw_color_datas ()
{
  int hsv_color;
  float r, g, b;
  /*
   * Mise en place du rouge
   */
  for (int i = 1; i < data.size () - 1; i++) {
    gdImageLine (im_colors, i - 1 + xoffset, (-data[i - 1].red) + xaxis_offset, i + xoffset, (-data[i].red) + xaxis_offset, graph_colors[IM_RGB_COLORS].red);
    gdImageLine (im_colors, i - 1 + xoffset, (-data[i - 1].green) + xaxis_offset, i + xoffset, (-data[i].green) + xaxis_offset, graph_colors[IM_RGB_COLORS].green);
    gdImageLine (im_colors, i - 1 + xoffset, (-data[i - 1].blue) + xaxis_offset, i + xoffset, (-data[i].blue) + xaxis_offset, graph_colors[IM_RGB_COLORS].blue);

    hsv_to_rgb (&r, &g, &b, data[i].hue, float (1), float (1));
    hsv_color = gdTrueColor (int (r * 255), int (g * 255), int (b * 255));
    gdImageLine (im_hsv, i + xoffset, (0) + xaxis_offset - 1, i + xoffset, (int ((-data[i].saturation) * 255)) +xaxis_offset - 1, hsv_color);
  }

}

void
graph::write_xml (string filename)
{
  fd_xml = fopen (filename.c_str (), "w");
  fprintf (fd_xml,
           "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<iri>\n<frame>\n"
           "<!-- m=movement, r/g/b=RGB values, s=saturation -->\n"
          );

  // Write video measurement data for every second:
  for (int i = 0; int (double (i) * (f->fps)) < data.size () - 1; i++) {
    int index = int (double (i) * (f->fps));

    float r;
    float g;
    float b;

    hsv_to_rgb (&r, &g, &b, data[index].hue, float (1), float (1));

    fprintf (fd_xml, "<v m=\"%d\" r=\"%d\" g=\"%d\" b=\"%d\" s=\"%d\" />\n",
             data[index].global,
             int (r * 255),
             int (g * 255),
             int (b * 255),
             int (data[index].saturation * 100)
            );
  }
  fprintf (fd_xml, "</frame>\n</iri>");
  fclose (fd_xml);

}
