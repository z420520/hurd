/* Unimplemented rpcs from <hurd/fsys.defs>
 
   Copyright (C) 1995, 1996 Free Software Foundation, Inc.
   Written by Michael I. Bushnell, p/BSG.

   This file is part of the GNU Hurd.

   The GNU Hurd is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   The GNU Hurd is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA. */

#include "netfs.h"
#include "fsys_S.h"

error_t
netfs_S_fsys_getfile (fsys_t cntl,
		      uid_t *uids, mach_msg_type_number_t nuids,
		      gid_t *gids, mach_msg_type_number_t ngids,
		      char *handle, mach_msg_type_number_t handlelen,
		      mach_port_t *file, mach_msg_type_name_t *filetype)
{
  return EOPNOTSUPP;
}

error_t
netfs_S_fsys_getpriv (fsys_t cntl,
		      mach_port_t *host, mach_msg_type_name_t *hosttp,
		      mach_port_t *dev, mach_msg_type_name_t *devtp,
		      mach_port_t *fs, mach_msg_type_name_t *fstp)
{
  return EOPNOTSUPP;
}

error_t
netfs_S_fsys_init (fsys_t cntl,
		   mach_port_t reply, mach_msg_type_name_t replytp,
		   mach_port_t proc, auth_t auth)
{
  return EOPNOTSUPP;
}

error_t
netfs_S_fsys_forward (fsys_t cntl,
		      mach_port_t request,
		      char *argv, mach_msg_type_number_t argvlen)
{
  return EOPNOTSUPP;
}

error_t
netfs_S_fsys_startup (mach_port_t bootstrap,
		      int flags,
		      mach_port_t contrl,
		      mach_port_t *realnod,
		      mach_msg_type_name_t *realnodetype)
{
  return EOPNOTSUPP;
}

error_t
netfs_S_fsys_goaway (mach_port_t cntl,
		     int flags)
{
  return EOPNOTSUPP;
}
