/*****************************************************************************
 *  Copyright (C) 2007 Lawrence Livermore National Security, LLC.
 *  This module was written by Jim Garlick <garlick@llnl.gov>
 *  UCRL-CODE-232438 All Rights Reserved.
 *
 *  This file is part of the Lustre Monitoring Tool.
 *  For details, see http://github.com/chaos/lmt.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the license, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY OF MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the terms and conditions of the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA or see
 *  http://www.gnu.org/licenses.
 *****************************************************************************/

#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <errno.h>
#include <sys/utsname.h>
#include <inttypes.h>
#include <math.h>

#include "list.h"
#include "hash.h"
#include "error.h"

#include "proc.h"
#include "stat.h"
#include "meminfo.h"
#include "lustre.h"

#include "lmt.h"
#include "router.h"
#include "util.h"
#include "lmtconf.h"

typedef struct {
    uint64_t    usage[2];
    uint64_t    total[2];
    int         valid;      /* number of valid samples [0,1,2] */
} usage_t;

static int
_get_mem_usage (pctx_t ctx, double *fp)
{
    uint64_t kfree, ktot;

    if (proc_meminfo (ctx, &ktot, &kfree) < 0) {
        if (lmt_conf_get_proto_debug ())
            err ("error reading memory usage from proc");
        return -1;
    }
    *fp = ((double)(ktot - kfree) / (double)(ktot)) * 100.0;
    return 0;
}

int
lmt_router_string_v1 (pctx_t ctx, char *s, int len)
{
    static uint64_t cpuusage = 0, cputot = 0;
    int retval = -1;
    struct utsname uts;
    double mempct, cpupct;
    uint64_t newbytes;
    int n, ena;

    if (proc_lustre_lnet_routing_enabled (ctx, &ena) < 0)
        goto done;
    if (!ena) {
        errno = 0;
        goto done;
    }
    if (uname (&uts) < 0) {
        err ("uname");
        goto done;
    }
    if (proc_stat2 (ctx, &cpuusage, &cputot, &cpupct) < 0) {
        if (lmt_conf_get_proto_debug ())
            err ("error reading cpu usage from proc");
        goto done;
    }
    if (_get_mem_usage (ctx, &mempct) < 0) {
        goto done;
    }
    if (proc_lustre_lnet_newbytes (ctx, &newbytes) < 0) {
        if (lmt_conf_get_proto_debug ())
            err ("error reading lustre lnet newbytes from proc");
        goto done;
    }
    /* N.B. Use 1.0 not 1 for version for backwards compat - issue 34 */
    n = snprintf (s, len, "1.0;%s;%f;%f;%"PRIu64,
                  uts.nodename, cpupct, mempct, newbytes);
    if (n >= len) {
        if (lmt_conf_get_proto_debug ())
            msg ("string overflow");
        goto done;
    }
    retval = 0;
done:
    return retval;
}

int
lmt_router_decode_v1 (const char *s, char **namep, float *pct_cpup,
                      float *pct_memp, uint64_t *bytesp)
{
    int retval = -1;
    char *name = xmalloc (strlen (s) + 1);
    float pct_mem, pct_cpu;
    uint64_t bytes;

    if (sscanf (s, "%*f;%[^;];%f;%f;%"PRIu64,
                name, &pct_cpu, &pct_mem, &bytes) != 4) {
        if (lmt_conf_get_proto_debug ())
            msg ("lmt_router_v1: parse error");
        goto done;
    }
    *namep = name;
    *bytesp = bytes;
    *pct_cpup = pct_cpu;
    *pct_memp = pct_mem;
    retval = 0;
done:
    if (retval < 0)
        free (name);
    return retval;
}

/*
 * vi:tabstop=4 shiftwidth=4 expandtab
 */
