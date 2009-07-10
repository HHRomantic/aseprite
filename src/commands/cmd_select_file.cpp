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

#include <assert.h>
#include <allegro/debug.h>
#include <allegro/unicode.h>

#include "jinete/jinete.h"

#include "ase/ui_context.h"
#include "commands/commands.h"
#include "core/app.h"
#include "modules/sprites.h"
#include "raster/sprite.h"

static bool cmd_select_file_enabled(const char *argument)
{
  /* with argument, the argument specified the ID of the GfxObj */
  if (argument) {
    int sprite_id = ustrtol(argument, NULL, 10);
    GfxObj *gfxobj = gfxobj_find(sprite_id);
    return gfxobj && gfxobj->type == GFXOBJ_SPRITE;
  }
  /* argument=NULL, means the select "Nothing" option  */
  else
    return TRUE;
}

static bool cmd_select_file_checked(const char *argument)
{
  const CurrentSpriteReader sprite;

  if (argument) {
    int sprite_id = ustrtol(argument, NULL, 10);
    GfxObj *gfxobj = gfxobj_find(sprite_id);
    return
      gfxobj && gfxobj->type == GFXOBJ_SPRITE &&
      sprite == (Sprite *)gfxobj;
  }
  else
    return sprite == NULL;
}

static void cmd_select_file_execute(const char *argument)
{
  UIContext* context = UIContext::instance();

  if (argument) {
    int sprite_id = ustrtol(argument, NULL, 10);
    GfxObj* gfxobj = gfxobj_find(sprite_id);
    assert(gfxobj != NULL);

    context->show_sprite((Sprite*)gfxobj);
  }
  else {
    context->show_sprite(NULL);
  }
}

Command cmd_select_file = {
  CMD_SELECT_FILE,
  cmd_select_file_enabled,
  cmd_select_file_checked,
  cmd_select_file_execute,
};
