/* A server for local sockets, of type PF_LOCAL

   Copyright (C) 1995 Free Software Foundation, Inc.

   Written by Miles Bader <miles@gnu.ai.mit.edu>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2, or (at
   your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

#include <stdio.h>
#include <getopt.h>
#include <error.h>
#include <sys/stat.h>

#include <hurd/hurd_types.h>
#include <hurd/trivfs.h>

#include "sock.h"

/* Where to put the file-system ports. */
static struct port_bucket *pf_port_bucket;

/* Trivfs hooks */
int trivfs_fstype = FSTYPE_MISC;
int trivfs_fsid = 0;
int trivfs_support_read = 0;
int trivfs_support_write = 0;
int trivfs_support_exec = 0;
int trivfs_allow_open = 0;

/* Trivfs noise.  */
struct port_class *trivfs_protid_portclasses[1];
struct port_class *trivfs_cntl_portclasses[1];
int trivfs_protid_nportclasses = 1;
int trivfs_cntl_nportclasses = 1;

/* ---------------------------------------------------------------- */

#define USAGE "Usage: %s\n"

static void
usage(int status)
{
  if (status != 0)
    fprintf(stderr, "Try `%s --help' for more information.\n",
	    program_invocation_name);
  else
    {
      printf(USAGE, program_invocation_name);
    }

  exit(status);
}

#define SHORT_OPTIONS "&"

static struct option options[] =
{
  {"help", no_argument, 0, '&'},
  {0, 0, 0, 0}
};

/* ---------------------------------------------------------------- */

/* A demuxer to separate out pf-level operations on our node.  */
static int
pf_demuxer (mach_msg_header_t *inp, mach_msg_header_t *outp)
{
  extern int socket_server (mach_msg_header_t *inp, mach_msg_header_t *outp);
  return socket_server (inp, outp) || trivfs_demuxer (inp, outp);
}

void main(int argc, char *argv[])
{
  int opt;
  error_t err;
  mach_port_t bootstrap;

  while ((opt = getopt_long(argc, argv, SHORT_OPTIONS, options, 0)) != EOF)
    switch (opt)
      {
      case '&': usage(0);
      default:  usage(1);
      }

  if (argv[optind] != NULL)
    {
      fprintf(stderr, USAGE, program_invocation_name);
      usage(1);
    }

  task_get_bootstrap_port (mach_task_self (), &bootstrap);
  if (bootstrap == MACH_PORT_NULL)
    error(2, 0, "Must be started as a translator");
  
  pf_port_bucket = ports_create_bucket ();

  trivfs_cntl_portclasses[0] = ports_create_class (trivfs_clean_cntl, 0);
  trivfs_protid_portclasses[0] = ports_create_class (trivfs_clean_protid, 0);

  /* Prepare to create sockets.  */
  err = sock_global_init ();
  if (err)
    error(3, err, "Initializing");

  /* Reply to our parent */
  err =
    trivfs_startup(bootstrap,
		   trivfs_cntl_portclasses[0], pf_port_bucket,
		   trivfs_protid_portclasses[0], pf_port_bucket,
		   NULL);
  if (err)
    error(3, err, "Contacting parent");

  /* Launch. */
  ports_manage_port_operations_multithread (pf_port_bucket,
					    pf_demuxer,
					    30*1000, 5*60*1000, 0, 0);

  exit(0);
}

void
trivfs_modify_stat (struct stat *st)
{
  st->st_fstype = FSTYPE_MISC;
}

error_t
trivfs_goaway (int flags, mach_port_t realnode,
	       struct port_class *fsys_port_class,
	       struct port_class *file_port_class)
{
  error_t err;

  /* Stop all I/O.  */
  ports_inhibit_bucket_rpcs (pf_port_bucket);

  /* Now see if there are any old sockets lying around.  */
  err = sock_global_shutdown (flags);

  /* Exit if not, or if we must. */
  if (err == 0 || flags & FSYS_GOAWAY_FORCE)
    exit (0);

  /* We won't go away, so start things going again...  */
  ports_resume_bucket_rpcs (pf_port_bucket);

  return err;
}

void
thread_cancel (thread_t foo __attribute__ ((unused)))
{
}
