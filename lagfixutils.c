/* Copyright (C) 2010 Zsolt Sz Sztupák
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#include "bootloader.h"
#include "common.h"
#include "cutils/properties.h"
#include "install.h"
#include "minui/minui.h"
#include "minzip/DirUtil.h"
#include "roots.h"
#include "recovery_ui.h"

#include "extendedcommands.h"
#include "commands.h"
#include "lagfixutils.h"
#include "nandroid.h"

extern char **environ;

void apply_root_to_device() {
  ensure_root_path_mounted("SYSTEM:");
  ui_print("Copying Superuser and Busybox files to /system/xbin\n");

  ui_print("Copying su binary\n");
  __system("rm /system/bin/su");
  __system("rm /system/xbin/su");
  __system("cp /res/misc/su /system/xbin/su");
  __system("chown 0.0 /system/xbin/su");
  __system("chmod 4755 /system/xbin/su");

  ui_print("Copying Superuser.apk\n");
  __system("rm /system/app/Superuser.apk");
  __system("rm /data/app/Superuser.apk");
  __system("cp /res/misc/Superuser.apk /system/app/Superuser.apk");
  __system("chown 0.0 /system/app/Superuser.apk");
  __system("chmod 644 /system/app/Superuser.apk");

  ui_print("Copying Busybox executable\n");
  __system("rm /system/xbin/busybox");
  __system("rm /system/bin/busybox");
  __system("cp /sbin/recovery /system/xbin/busybox");
  __system("chmod 755 /system/xbin/busybox");

  ui_print("Creating symlinks to commands\n");
  __system("ln -s /system/xbin/busybox /system/xbin/[");
  __system("ln -s /system/xbin/busybox /system/xbin/[[");
  __system("ln -s /system/xbin/busybox /system/xbin/ash");
  __system("ln -s /system/xbin/busybox /system/xbin/awk");
  __system("ln -s /system/xbin/busybox /system/xbin/basename");
  __system("ln -s /system/xbin/busybox /system/xbin/bbconfig");
  __system("ln -s /system/xbin/busybox /system/xbin/bunzip2");
  __system("ln -s /system/xbin/busybox /system/xbin/bzcat");
  __system("ln -s /system/xbin/busybox /system/xbin/bzip2");
  __system("ln -s /system/xbin/busybox /system/xbin/cal");
  __system("ln -s /system/xbin/busybox /system/xbin/cat");
  __system("ln -s /system/xbin/busybox /system/xbin/catv");
  __system("ln -s /system/xbin/busybox /system/xbin/chgrp");
  __system("ln -s /system/xbin/busybox /system/xbin/chmod");
  __system("ln -s /system/xbin/busybox /system/xbin/chown");
  __system("ln -s /system/xbin/busybox /system/xbin/chroot");
  __system("ln -s /system/xbin/busybox /system/xbin/cksum");
  __system("ln -s /system/xbin/busybox /system/xbin/clear");
  __system("ln -s /system/xbin/busybox /system/xbin/cmp");
  __system("ln -s /system/xbin/busybox /system/xbin/cp");
  __system("ln -s /system/xbin/busybox /system/xbin/cpio");
  __system("ln -s /system/xbin/busybox /system/xbin/cut");
  __system("ln -s /system/xbin/busybox /system/xbin/date");
  __system("ln -s /system/xbin/busybox /system/xbin/dc");
  __system("ln -s /system/xbin/busybox /system/xbin/dd");
  __system("ln -s /system/xbin/busybox /system/xbin/depmod");
  __system("ln -s /system/xbin/busybox /system/xbin/devmem");
  __system("ln -s /system/xbin/busybox /system/xbin/df");
  __system("ln -s /system/xbin/busybox /system/xbin/diff");
  __system("ln -s /system/xbin/busybox /system/xbin/dirname");
  __system("ln -s /system/xbin/busybox /system/xbin/dmesg");
  __system("ln -s /system/xbin/busybox /system/xbin/dos2unix");
  __system("ln -s /system/xbin/busybox /system/xbin/du");
  __system("ln -s /system/xbin/busybox /system/xbin/echo");
  __system("ln -s /system/xbin/busybox /system/xbin/egrep");
  __system("ln -s /system/xbin/busybox /system/xbin/env");
  __system("ln -s /system/xbin/busybox /system/xbin/expr");
  __system("ln -s /system/xbin/busybox /system/xbin/false");
  __system("ln -s /system/xbin/busybox /system/xbin/fdisk");
  __system("ln -s /system/xbin/busybox /system/xbin/fgrep");
  __system("ln -s /system/xbin/busybox /system/xbin/find");
  __system("ln -s /system/xbin/busybox /system/xbin/fold");
  __system("ln -s /system/xbin/busybox /system/xbin/free");
  __system("ln -s /system/xbin/busybox /system/xbin/freeramdisk");
  __system("ln -s /system/xbin/busybox /system/xbin/fuser");
  __system("ln -s /system/xbin/busybox /system/xbin/getopt");
  __system("ln -s /system/xbin/busybox /system/xbin/grep");
  __system("ln -s /system/xbin/busybox /system/xbin/gunzip");
  __system("ln -s /system/xbin/busybox /system/xbin/gzip");
  __system("ln -s /system/xbin/busybox /system/xbin/head");
  __system("ln -s /system/xbin/busybox /system/xbin/hexdump");
  __system("ln -s /system/xbin/busybox /system/xbin/id");
  __system("ln -s /system/xbin/busybox /system/xbin/insmod");
  __system("ln -s /system/xbin/busybox /system/xbin/install");
  __system("ln -s /system/xbin/busybox /system/xbin/kill");
  __system("ln -s /system/xbin/busybox /system/xbin/killall");
  __system("ln -s /system/xbin/busybox /system/xbin/killall5");
  __system("ln -s /system/xbin/busybox /system/xbin/length");
  __system("ln -s /system/xbin/busybox /system/xbin/less");
  __system("ln -s /system/xbin/busybox /system/xbin/ln");
  __system("ln -s /system/xbin/busybox /system/xbin/losetup");
  __system("ln -s /system/xbin/busybox /system/xbin/ls");
  __system("ln -s /system/xbin/busybox /system/xbin/lsmod");
  __system("ln -s /system/xbin/busybox /system/xbin/lspci");
  __system("ln -s /system/xbin/busybox /system/xbin/lsusb");
  __system("ln -s /system/xbin/busybox /system/xbin/lzop");
  __system("ln -s /system/xbin/busybox /system/xbin/lzopcat");
  __system("ln -s /system/xbin/busybox /system/xbin/md5sum");
  __system("ln -s /system/xbin/busybox /system/xbin/mkdir");
  __system("ln -s /system/xbin/busybox /system/xbin/mke2fs");
  __system("ln -s /system/xbin/busybox /system/xbin/mkfifo");
  __system("ln -s /system/xbin/busybox /system/xbin/mkfs.ext2");
  __system("ln -s /system/xbin/busybox /system/xbin/mknod");
  __system("ln -s /system/xbin/busybox /system/xbin/mkswap");
  __system("ln -s /system/xbin/busybox /system/xbin/mktemp");
  __system("ln -s /system/xbin/busybox /system/xbin/modprobe");
  __system("ln -s /system/xbin/busybox /system/xbin/more");
  __system("ln -s /system/xbin/busybox /system/xbin/mount");
  __system("ln -s /system/xbin/busybox /system/xbin/mountpoint");
  __system("ln -s /system/xbin/busybox /system/xbin/mv");
  __system("ln -s /system/xbin/busybox /system/xbin/nice");
  __system("ln -s /system/xbin/busybox /system/xbin/nohup");
  __system("ln -s /system/xbin/busybox /system/xbin/od");
  __system("ln -s /system/xbin/busybox /system/xbin/patch");
  __system("ln -s /system/xbin/busybox /system/xbin/pgrep");
  __system("ln -s /system/xbin/busybox /system/xbin/pidof");
  __system("ln -s /system/xbin/busybox /system/xbin/pkill");
  __system("ln -s /system/xbin/busybox /system/xbin/printenv");
  __system("ln -s /system/xbin/busybox /system/xbin/printf");
  __system("ln -s /system/xbin/busybox /system/xbin/ps");
  __system("ln -s /system/xbin/busybox /system/xbin/pwd");
  __system("ln -s /system/xbin/busybox /system/xbin/rdev");
  __system("ln -s /system/xbin/busybox /system/xbin/readlink");
  __system("ln -s /system/xbin/busybox /system/xbin/realpath");
  __system("ln -s /system/xbin/busybox /system/xbin/renice");
  __system("ln -s /system/xbin/busybox /system/xbin/reset");
  __system("ln -s /system/xbin/busybox /system/xbin/rm");
  __system("ln -s /system/xbin/busybox /system/xbin/rmdir");
  __system("ln -s /system/xbin/busybox /system/xbin/rmmod");
  __system("ln -s /system/xbin/busybox /system/xbin/run-parts");
  __system("ln -s /system/xbin/busybox /system/xbin/sed");
  __system("ln -s /system/xbin/busybox /system/xbin/seq");
  __system("ln -s /system/xbin/busybox /system/xbin/setsid");
  __system("ln -s /system/xbin/busybox /system/xbin/sh");
  __system("ln -s /system/xbin/busybox /system/xbin/sha1sum");
  __system("ln -s /system/xbin/busybox /system/xbin/sha256sum");
  __system("ln -s /system/xbin/busybox /system/xbin/sha512sum");
  __system("ln -s /system/xbin/busybox /system/xbin/sleep");
  __system("ln -s /system/xbin/busybox /system/xbin/sort");
  __system("ln -s /system/xbin/busybox /system/xbin/split");
  __system("ln -s /system/xbin/busybox /system/xbin/stat");
  __system("ln -s /system/xbin/busybox /system/xbin/strings");
  __system("ln -s /system/xbin/busybox /system/xbin/stty");
  __system("ln -s /system/xbin/busybox /system/xbin/swapoff");
  __system("ln -s /system/xbin/busybox /system/xbin/swapon");
  __system("ln -s /system/xbin/busybox /system/xbin/sync");
  __system("ln -s /system/xbin/busybox /system/xbin/sysctl");
  __system("ln -s /system/xbin/busybox /system/xbin/tac");
  __system("ln -s /system/xbin/busybox /system/xbin/tail");
  __system("ln -s /system/xbin/busybox /system/xbin/tar");
  __system("ln -s /system/xbin/busybox /system/xbin/tee");
  __system("ln -s /system/xbin/busybox /system/xbin/test");
  __system("ln -s /system/xbin/busybox /system/xbin/time");
  __system("ln -s /system/xbin/busybox /system/xbin/top");
  __system("ln -s /system/xbin/busybox /system/xbin/touch");
  __system("ln -s /system/xbin/busybox /system/xbin/tr");
  __system("ln -s /system/xbin/busybox /system/xbin/true");
  __system("ln -s /system/xbin/busybox /system/xbin/tty");
  __system("ln -s /system/xbin/busybox /system/xbin/umount");
  __system("ln -s /system/xbin/busybox /system/xbin/uname");
  __system("ln -s /system/xbin/busybox /system/xbin/uniq");
  __system("ln -s /system/xbin/busybox /system/xbin/unix2dos");
  __system("ln -s /system/xbin/busybox /system/xbin/unlzop");
  __system("ln -s /system/xbin/busybox /system/xbin/unzip");
  __system("ln -s /system/xbin/busybox /system/xbin/uptime");
  __system("ln -s /system/xbin/busybox /system/xbin/usleep");
  __system("ln -s /system/xbin/busybox /system/xbin/uudecode");
  __system("ln -s /system/xbin/busybox /system/xbin/uuencode");
  __system("ln -s /system/xbin/busybox /system/xbin/watch");
  __system("ln -s /system/xbin/busybox /system/xbin/wc");
  __system("ln -s /system/xbin/busybox /system/xbin/which");
  __system("ln -s /system/xbin/busybox /system/xbin/whoami");
  __system("ln -s /system/xbin/busybox /system/xbin/xargs");
  __system("ln -s /system/xbin/busybox /system/xbin/yes");
  __system("ln -s /system/xbin/busybox /system/xbin/zcat");
  ui_print("Done!\n");
}

static char* startval[] = {"DATA_FS=","CACHE_FS=","DBDATA_FS=","DATA_LOOP=","CACHE_LOOP=","DBDATA_LOOP=","BIND_DATA_TO_DBDATA=", NULL};
static int catvals[] = { 3,6,7,999 };
static char* categories[][4] = {
    {"rfs","ext2","ext4",NULL},
    {"false","ext2",NULL,NULL},
    {"false","true",NULL,NULL},
    {NULL,NULL,NULL,NULL}
  };



void advanced_lagfix_menu() {
  static char* headers[] = { "Advanced Lagfix Menu",
                             "", NULL };

  int chosen_item = 0;
  for (;;)
    {
        char* list[8];
        FILE* f = fopen("/system/etc/lagfix.conf","r");
        int i=0;
        for (i=0;i<7;i++) {
          list[i] = malloc(64);
        }
        list[7] = NULL;
        if (f==NULL) {
          LOGE("Could not open lagfix.conf!\n");
        } else {
          ui_print("Current configuration is:\n");
          i=0;
          while ((i<7) && (fgets(list[i],63,f))) {
            ui_print(list[i]);
            list[i][strlen(list[i])-1]='\0'; // remove trailing newline
            i++;
          }
          fclose(f);
        }

        chosen_item = get_menu_selection(headers, list, 0);
        if (chosen_item == GO_BACK)
            break;

        int category=0;
        while (chosen_item>=catvals[category]) {
          category++;
        }

        ui_print("%d\n",category);

        int curval=0;
        while ((categories[category][curval]) && (memcmp(list[chosen_item]+strlen(startval[chosen_item]),categories[category][curval],strlen(categories[category][curval]))!=0))
          curval++;

        ui_print("%d\n",curval);

        if (categories[category][curval]) {
          curval++;
          if (!categories[category][curval]) {
            curval = 0;
          }
        } else {
          curval=0;
        }

        ui_print("%d\n",curval);

        sprintf(list[chosen_item],"%s%s",startval[chosen_item],categories[category][curval]);

        f = fopen("/system/etc/lagfix.conf","w+");
        for (i=0; i<7; i++) {
          fprintf(f,"%s\n",list[i]);
        }
        fclose(f);
    }
}

void lagfix_menu() {
  static char* headers[] = {  "Lagfix Menu",
                              "d: /data; o: /cache+/dbdata; a: d+o",
                              "binds: /data/data and /data/dalvik-cache moved",
                              NULL
  };

  static char* list[] = { "Disable lagfix",
                          "Use OCLF (d=rfs+e2;o=rfs)",
                          "Use Voodoo (d=e4;o=rfs)",
                          "Use NO-RFS standard (d=rfs+e2;o=e4)",
                          "Use NO-RFS advanced (a=e4;binds)",
                          "Use NO-RFS extended (a=e4+e2;binds)",
                          "Advanced options",
                          NULL
    };

    for (;;)
    {
        FILE* f = fopen("/system/etc/lagfix.conf","r");
        if (f==NULL) {
          LOGE("Could not open lagfix.conf!");
        } else {
          char buf[64];
          ui_print("Current configuration is:\n");
          while (fgets(buf,63,f)) {
            ui_print(buf);
          }
          fclose(f);
        }

        int chosen_item = get_menu_selection(headers, list, 0);
        if (chosen_item == GO_BACK)
            break;
        switch (chosen_item)
        {
          case 0:f = fopen("/system/etc/lagfix.conf","w+");fprintf(f,"DATA_FS=rfs\nCACHE_FS=rfs\nDBDATA_FS=rfs\nDATA_LOOP=false\nCACHE_LOOP=false\nDBDATA_LOOP=false\nBIND_DATA_TO_DBDATA=false\n");fclose(f);break;
          case 1:f = fopen("/system/etc/lagfix.conf","w+");fprintf(f,"DATA_FS=rfs\nCACHE_FS=rfs\nDBDATA_FS=rfs\nDATA_LOOP=ext2\nCACHE_LOOP=false\nDBDATA_LOOP=false\nBIND_DATA_TO_DBDATA=false\n");fclose(f);break;
          case 2:f = fopen("/system/etc/lagfix.conf","w+");fprintf(f,"DATA_FS=ext4\nCACHE_FS=rfs\nDBDATA_FS=rfs\nDATA_LOOP=false\nCACHE_LOOP=false\nDBDATA_LOOP=false\nBIND_DATA_TO_DBDATA=false\n");fclose(f);break;
          case 3:f = fopen("/system/etc/lagfix.conf","w+");fprintf(f,"DATA_FS=rfs\nCACHE_FS=ext4\nDBDATA_FS=ext4\nDATA_LOOP=ext2\nCACHE_LOOP=false\nDBDATA_LOOP=false\nBIND_DATA_TO_DBDATA=false\n");fclose(f);break;
          case 4:f = fopen("/system/etc/lagfix.conf","w+");fprintf(f,"DATA_FS=ext4\nCACHE_FS=ext4\nDBDATA_FS=ext4\nDATA_LOOP=false\nCACHE_LOOP=false\nDBDATA_LOOP=false\nBIND_DATA_TO_DBDATA=true\n");fclose(f);break;
          case 5:f = fopen("/system/etc/lagfix.conf","w+");fprintf(f,"DATA_FS=ext4\nCACHE_FS=ext4\nDBDATA_FS=ext4\nDATA_LOOP=ext2\nCACHE_LOOP=ext2\nDBDATA_LOOP=ext2\nBIND_DATA_TO_DBDATA=true\n");fclose(f);break;
          case 6:advanced_lagfix_menu(); break;
        }
    }
}

int searchfor_in_config_file(const char* searchfor, int category) {
  int res = -1;
  char buf[64]; 
  FILE *f = fopen("/system/etc/lagfix.conf.old","r");
  if (!f) return -1;
  while (fgets(buf,63,f)) {
    if (memcmp(searchfor,buf,strlen(searchfor))==0) {
      res++;
      while (categories[category][res] && (memcmp(buf+strlen(searchfor),categories[category][res],strlen(categories[category][res]))!=0)) {
        res++;
      }
      if (!categories[category][res]) res=-1;
    }
  }
  fclose(f);
  return res;

}

int get_loop_options(const char * name) {
  char searchfor[64];
  sprintf(searchfor,"%s_LOOP=",name);
  return searchfor_in_config_file(searchfor,1);
}

int get_fs_options(const char * name) {
  char searchfor[64];
  sprintf(searchfor,"%s_FS=",name);
  return searchfor_in_config_file(searchfor,0);
}

int dont_use_bind_options = 0;
int get_bind_options() {
  if (dont_use_bind_options) return 0;
  return searchfor_in_config_file("BIND_DATA_TO_DBDATA=",2);
}

// there should be some checks whether the action actually succeded
int ensure_lagfix_mount_points(const RootInfo *info) {
  int getfsopts,getloopopts,bindopts;
  if (strcmp(info->name,"DATA:")==0) {
    getfsopts = get_fs_options("DATA");
    getloopopts = get_loop_options("DATA");
    bindopts = get_bind_options();
    if (getloopopts) {
      __system("mkdir /res/odata");
      __system("chmod 700 /res/odata");
      if (getfsopts==0) {
        __system("mount -t rfs /dev/block/mmcblk0p2 /res/odata");
      } else if (getfsopts==1) {
        __system("mount -t ext2 /dev/block/mmcblk0p2 /res/odata");
      } else if (getfsopts==2) {
        __system("mount -t ext4 /dev/block/mmcblk0p2 /res/odata");
      }
      __system("losetup /dev/block/loop1 /res/odata/.extfs");
      __system("mount -t ext2 /dev/block/loop1 /data");
    } else {
      if (getfsopts==0) {
        __system("mount -t rfs /dev/block/mmcblk0p2 /data");
      } else if (getfsopts==1) {
        __system("mount -t ext2 /dev/block/mmcblk0p2 /data");
      } else if (getfsopts==2) {
        __system("mount -t ext4 /dev/block/mmcblk0p2 /data");
      }
    }
    if (bindopts) {
      ensure_root_path_mounted("DATADATA:");
      __system("mkdir -p /dbdata/.data/data");
      __system("mkdir -p /dbdata/.data/dalvik-cache");
      __system("mkdir -p /data/data");
      __system("mkdir -p /data/dalvik-cache");
      __system("mount -o bind /dbdata/.data/data /data/data");
      __system("mount -o bind /dbdata/.data/dalvik-cache /data/dalvik-cache");
    }
  } else if (strcmp(info->name,"DATADATA:")==0) {
    getfsopts = get_fs_options("DBDATA");
    getloopopts = get_loop_options("DBDATA");
    if (getloopopts) {
      __system("mkdir /res/odbdata");
      __system("chmod 700 /res/odbdata");
      if (getfsopts==0) {
        __system("mount -t rfs /dev/block/stl10 /res/odbdata");
      } else if (getfsopts==1) {
        __system("mount -t ext2 /dev/block/stl10 /res/odbdata");
      } else if (getfsopts==2) {
        __system("mount -t ext4 /dev/block/stl10 /res/odbdata");
      }
      __system("losetup /dev/block/loop2 /res/odbdata/.extfs");
      __system("mount -t ext2 /dev/block/loop2 /dbdata");
    } else {
      if (getfsopts==0) {
        __system("mount -t rfs /dev/block/stl10 /dbdata");
      } else if (getfsopts==1) {
        __system("mount -t ext2 /dev/block/stl10 /dbdata");
      } else if (getfsopts==2) {
        __system("mount -t ext4 /dev/block/stl10 /dbdata");
      }
    }
  } else if (strcmp(info->name,"CACHE:")==0) {
    getfsopts = get_fs_options("CACHE");
    getloopopts = get_loop_options("CACHE");
    bindopts = get_bind_options();
    if (getloopopts) {
      __system("mkdir /res/ocache");
      __system("chmod 700 /res/ocache");
      if (getfsopts==0) {
        __system("mount -t rfs /dev/block/stl11 /res/ocache");
      } else if (getfsopts==1) {
        __system("mount -t ext2 /dev/block/stl11 /res/ocache");
      } else if (getfsopts==2) {
        __system("mount -t ext4 /dev/block/stl11 /res/ocache");
      }
      __system("losetup /dev/block/loop3 /res/ocache/.extfs");
      __system("mount -t ext2 /dev/block/loop3 /cache");
    } else {
      if (getfsopts==0) {
        __system("mount -t rfs /dev/block/stl11 /cache");
      } else if (getfsopts==1) {
        __system("mount -t ext2 /dev/block/stl11 /cache");
      } else if (getfsopts==2) {
        __system("mount -t ext4 /dev/block/stl11 /cache");
      }
    }
  } else {
    return 1;
  }
  return 0;
}

// not implemented
int ensure_lagfix_unmount_points(const RootInfo *info) {
  if (strcmp(info->name,"DATA:")==0) {
    return -1;
  } else if (strcmp(info->name,"DATADATA:")==0) {
    return -1;
  } else if (strcmp(info->name,"CACHE:")==0) {
    return -1;
  } else return 1;
}

//not implemented
int ensure_lagfix_formatted(const RootInfo *info) {
// we won't remove hidden files yet
  if (strcmp(info->name,"DATA:")==0) {
    __system("rm -rf /data/*");
    return 0;
  } else if (strcmp(info->name,"DATADATA:")==0) {
    __system("rm -rf /dbdata/*");
    return 0;
  } else if (strcmp(info->name,"CACHE:")==0) {
    __system("rm -rf /cache/*");
    return 0;
  } else return 1;
}

int unmount_all_lagfixed() {
  sync();
  __system("umount -f /data/data");
  __system("umount -f /data/dalvik-cache");
  __system("umount -f -d /dev/block/loop3");
  __system("umount -f -d /dev/block/loop2");
  __system("umount -f -d /dev/block/loop1");
  __system("umount -f /res/ocache");
  __system("umount -f /res/odata");
  __system("umount -f /res/odbdata");
  __system("umount -f /cache");
  __system("umount -f /data");
  __system("umount -f /dbdata");
  return 0;
}

int create_lagfix_partition(int id) {
  char loopname[64],blockname[64];
  char looppos[64],blockpos[64];
  char name[64];
  int loopsize;
  if (id==0) {
    strcpy(loopname,"/dev/block/loop1");
    strcpy(blockname,"/dev/block/mmcblk0p2");
    strcpy(looppos,"/res/odata");
    strcpy(blockpos,"/data");
    strcpy(name,"DATA");
    loopsize = 1831634944;
  } else if (id==1) {
    strcpy(loopname,"/dev/block/loop2");
    strcpy(blockname,"/dev/block/stl10");
    strcpy(looppos,"/res/odbdata");
    strcpy(blockpos,"/dbdata");
    strcpy(name,"DBDATA");
    loopsize = 128382976;
  } else {
    strcpy(loopname,"/dev/block/loop3");
    strcpy(blockname,"/dev/block/stl11");
    strcpy(looppos,"/res/ocache");
    strcpy(blockpos,"/cache");
    strcpy(name,"CACHE");
    loopsize = 29726720;
  }
  int ft = get_fs_options(name);
  int loop = get_loop_options(name);
  char tmp[256];
  if (ft==0) {
    sprintf(tmp,"/sbin/fat.format %s",blockname);
    __system(tmp);
  } else if (ft==1) {
    sprintf(tmp,"/sbin/mkfs.ext2 %s",blockname);
    __system(tmp);
  } else if (ft==2) {
    sprintf(tmp,"/sbin/mkfs.ext4 %s",blockname);
    __system(tmp);
  }

  if (loop) {
    sprintf(tmp,"mount %s %s",blockname,looppos);
    __system(tmp);
    sprintf(tmp,"%s/.extfs",looppos);
    FILE*f = fopen(tmp,"w+");fclose(f);
    truncate(tmp,loopsize);
    sprintf(tmp,"losetup /dev/block/loop0 %s/.extfs",looppos);
    __system(tmp);
    __system("mkfs.ext2 /dev/block/loop0");
    __system("losetup -d /dev/block/loop0");
    sprintf(tmp,"umount %s",blockname);
    __system(tmp);
  }
  return 0;
}

int do_lagfix() {
  ui_print("checking mounts avialable\n");
  if (ensure_root_path_mounted("DATA:")!=0) return -1;
  if (ensure_root_path_mounted("DATADATA:")!=0) return -1;
  if (ensure_root_path_mounted("CACHE:")!=0) return -1;
  if (ensure_root_path_mounted("SDCARD:")!=0) return -1;

  char tmp[PATH_MAX];
  nandroid_generate_timestamp_path(tmp);
  ui_print("Creating a nandroid backup at %s\n",tmp);
  if (nandroid_backup_flags(tmp,DONT_BACKUP_SYSTEM)!=0) return -1;

  ui_print("Backup completed, recreating file systems\n");

  ui_print("Unmounting\n");
  unmount_all_lagfixed();

  ui_print("Switching to new config\n");
  __system("cp /system/etc/lagfix.conf /system/etc/lagfix.conf.old");

  ui_print("Creating /data\n");
  create_lagfix_partition(0);
  ui_print("Creating /dbdata\n");
  create_lagfix_partition(1);
  ui_print("Creating /cache\n");
  create_lagfix_partition(2);

  ui_print("Mounting to test\n");
  dont_use_bind_options = 1;
  if (ensure_root_path_mounted("DATA:")!=0) return -1;
  if (ensure_root_path_mounted("DATADATA:")!=0) return -1;
  if (ensure_root_path_mounted("CACHE:")!=0) return -1;
  dont_use_bind_options = 0;
  __system("mount"); // for debug purposes
  if (get_bind_options()) {
    ui_print("Creating bind directories\n");
    __system("mkdir -p /dbdata/.data/data");
    __system("mkdir -p /dbdata/.data/dalvik-cache");
    __system("mkdir -p /data/data");
    __system("mkdir -p /data/dalvik-cache");
  }

  ui_print("Unmounting again\n");
  unmount_all_lagfixed();

  ui_print("Restoring data\n");
  nandroid_restore(tmp,0,0,1,1,0);

  // restore might have brought some .data into dbdata, clear them
  if (!get_bind_options()) {
    if (ensure_root_path_mounted("DATADATA:")!=0) return -1;
    __system("rm -rf /dbdata/.data");
  }

  __system("mount");
  ui_print("Unmounting again to be sure\n");
  sync();
  sleep(5);
  unmount_all_lagfixed();
  sync();
  return 0;
}

int lagfixer_main(int argc, char** argv) {
  ui_init();
  ui_print(EXPAND(RECOVERY_VERSION)" - lagfixer\n");
  create_fstab();
  ui_set_show_text(1);

  int res = do_lagfix();
  if (res) {
    ui_print("Something went wrong while doing the lagfix, sorry.\n");
  } else {
    ui_print("Done. Your device will reboot soon.\n");
  }
  sleep(5);

  gr_exit();
  ev_exit();
  return 0;
}
