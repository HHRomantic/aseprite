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

#include "jinete/jinete.h"

#include "commands/commands.h"
#include "modules/gui.h"
#include "raster/layer.h"
#include "undoable.h"

static bool cmd_layer_from_background_enabled(const char *argument)
{
  const CurrentSpriteReader sprite;
  return
    sprite != NULL &&
    sprite->layer != NULL &&
    layer_is_image(sprite->layer) &&
    layer_is_readable(sprite->layer) &&
    layer_is_writable(sprite->layer) &&
    layer_is_background(sprite->layer);
}

static void cmd_layer_from_background_execute(const char *argument)
{
  CurrentSpriteWriter sprite;
  {
    Undoable undoable(sprite, "Layer from Background");
    undoable.layer_from_background();
    undoable.commit();
  }
  update_screen_for_sprite(sprite);
}

Command cmd_layer_from_background = {
  CMD_LAYER_FROM_BACKGROUND,
  cmd_layer_from_background_enabled,
  NULL,
  cmd_layer_from_background_execute,
};
