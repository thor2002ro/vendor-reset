/*
Vendor Reset - Vendor Specific Reset
Copyright (C) 2020 Geoffrey McRae <geoff@hostfission.com>
Copyright (C) 2020 Adam Madsen <adam@ajmadsen.com>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <linux/module.h>

#include "ioctl.h"
#include "ftrace.h"
#include "hooks.h"

#define vr_info(fmt, args...) pr_info("vendor-reset: " fmt, ##args)

static bool install_hook = true;

static int __init vendor_reset_init(void)
{
  int ret;

  ret = vendor_reset_ioctl_init();
  if (ret)
    return ret;

  if (install_hook)
  {
    ret = fh_install_hooks(fh_hooks);
    if (ret)
      goto err;

    vr_info("hooks installed\n");
  }

  return 0;

err:
  vendor_reset_ioctl_exit();
  return ret;
}

static void __exit vendor_reset_exit(void)
{
  if (install_hook)
    fh_remove_hooks(fh_hooks);

  vendor_reset_ioctl_exit();
}

module_init(vendor_reset_init);
module_exit(vendor_reset_exit);
module_param(install_hook, bool, 0);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Geoffrey McRae <geoff@hostfission.com>");
MODULE_AUTHOR("Adam Madsen <adam@ajmadsen.com>");
