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

extern char **environ;
// runs the program and redirects it's output to the screen using ui_print
int graphsh_main(int argc, char** argv) {
  if (argc>=2) {
    ui_init();
    ui_print(EXPAND(RECOVERY_VERSION)" - script runner\n");

    ui_set_show_text(1);

    FILE* f = popen(argv[1],"r");
    char buff[255];
    int l;
    if (!f) {
      ui_print("Could not open process\n");
      sleep(1);
      gr_exit();
      ev_exit();
      return -1;
    }
    while(fgets(buff,250,f))
    {
      ui_print("%s",buff);
    }
    pclose(f);
    gr_exit();
    ev_exit();
    return 0;
  } else {
    printf("Usage: graphsh command\n  command with it's optional parameter(s) must be in quotes.\n");
    return -1;
  }
}

