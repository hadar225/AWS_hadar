/* hcidump.h
 *
 * $Id:dvbtsdump.h 2004/09/01 $
 *
 * Copyright (c) 2004 by Lutz Findeisen <lfindeis@xxxxxxxxxxxxxx>
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef __DVBTSDUMP_H__
#define __DVBTSDUMP_H__

int dvbtsdump_open(wtap *wth, int *err, gchar **err_info _U_);

#endif
