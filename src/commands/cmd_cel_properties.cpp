/* ASE - Allegro Sprite Editor
 * Copyright (C) 2001-2009  David Capello
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "config.h"

#include <allegro/unicode.h>

#include "jinete/jinete.h"

#include "commands/commands.h"
#include "core/app.h"
#include "core/core.h"
#include "modules/gui.h"
#include "modules/sprites.h"
#include "raster/cel.h"
#include "raster/image.h"
#include "raster/layer.h"
#include "raster/sprite.h"
#include "raster/stock.h"
#include "raster/undo.h"

static bool cmd_cel_properties_enabled(const char *argument)
{
  const CurrentSpriteReader sprite;
  return sprite && sprite->layer;
}

static void cmd_cel_properties_execute(const char *argument)
{
  JWidget label_frame, label_pos, label_size;
  JWidget slider_opacity, button_ok;
  Layer *layer;
  Cel *cel;
  char buf[1024];
  int memsize;

  /* get current sprite */
  const CurrentSpriteReader sprite;
  if (!sprite)
    return;

  /* get selected layer */
  layer = sprite->layer;
  if (!layer)
    return;

  /* get current cel (can be NULL) */
  cel = layer_get_cel(layer, sprite->frame);

  JWidgetPtr window(load_widget("celprop.jid", "cel_properties"));
  get_widgets(window,
	      "frame", &label_frame,
	      "pos", &label_pos,
	      "size", &label_size,
	      "opacity", &slider_opacity,
	      "ok", &button_ok, NULL);

  /* if the layer isn't writable */
  if (!layer_is_writable(layer)) {
    jwidget_set_text(button_ok, _("Locked"));
    jwidget_disable(button_ok);
  }

  usprintf(buf, "%d/%d", sprite->frame+1, sprite->frames);
  jwidget_set_text(label_frame, buf);

  if (cel != NULL) {
    /* position */
    usprintf(buf, "%d, %d", cel->x, cel->y);
    jwidget_set_text(label_pos, buf);

    /* dimension (and memory size) */
    memsize =
      image_line_size(sprite->stock->image[cel->image],
		      sprite->stock->image[cel->image]->w)*
      sprite->stock->image[cel->image]->h;

    usprintf(buf, "%dx%d (",
	     sprite->stock->image[cel->image]->w,
	     sprite->stock->image[cel->image]->h);
    get_pretty_memsize(memsize,
		       buf+ustrsize(buf),
		       sizeof(buf)-ustrsize(buf));
    ustrcat(buf, ")");

    jwidget_set_text(label_size, buf);

    /* opacity */
    jslider_set_value(slider_opacity, cel->opacity);
    if (layer_is_background(layer)) {
      jwidget_disable(slider_opacity);
      jwidget_add_tooltip_text(slider_opacity, "The `Background' layer is opaque,\n"
					       "you can't change its opacity.");
    }
  }
  else {
    jwidget_set_text(label_pos, "None");
    jwidget_set_text(label_size, "Empty (0 bytes)");
    jslider_set_value(slider_opacity, 0);
    jwidget_disable(slider_opacity);
  }

  jwindow_open_fg(window);

  if (jwindow_get_killer(window) == button_ok) {
    int new_opacity = jslider_get_value(slider_opacity);

    /* the opacity was changed? */
    if (cel != NULL &&
	cel->opacity != new_opacity) {
      if (undo_is_enabled(sprite->undo)) {
	undo_set_label(sprite->undo, "Cel Opacity Change");
	undo_int(sprite->undo, (GfxObj *)cel, &cel->opacity);
      }

      /* change cel opacity */
      cel_set_opacity(cel, new_opacity);

      update_screen_for_sprite(sprite);
    }
  }
}

Command cmd_cel_properties = {
  CMD_CEL_PROPERTIES,
  cmd_cel_properties_enabled,
  NULL,
  cmd_cel_properties_execute,
};
