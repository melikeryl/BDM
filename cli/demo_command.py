# -----------------------------------------------------------------------------
#
# Copyright (C) 2021 CERN & University of Surrey for the benefit of the
# BioDynaMo collaboration. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
#
# See the LICENSE file distributed with this work for details.
# See the NOTICE file distributed with this work for additional information
# regarding copyright ownership.
#
# -----------------------------------------------------------------------------

import os
import shutil
import sys

from print_command import Print
from git_utils import InitializeNewGitRepo
from common import CopySupportFiles

DEMO_DIR = os.path.join(os.environ["BDMSYS"], "demo")
STYLE_DIR = os.path.join(os.environ["BDMSYS"], "share", "util", "style_checks")
KNOWN_DEMOS = os.listdir(DEMO_DIR)


def DemoCommand(demo_name, destination=None):
    if not demo_name:
        print("Usage: biodynamo demo <demo name> [target directory]")
        print("Known demos:\n  {}".format("\n  ".join(KNOWN_DEMOS)))
        return
    if demo_name not in KNOWN_DEMOS:
        Print.error("Demo name \"{}\" is not known.".format(demo_name))
        print("Known demos:\n  {}".format("\n  ".join(KNOWN_DEMOS)))
        sys.exit(1)
    if destination is None:
        destination = "."
    if os.path.exists(destination):
        destination = os.path.join(destination, demo_name)
    if os.path.exists(destination):
        Print.error("Destination directory \"{}\" exists.".format(destination))
        Print.error(
            "Please remove it or create the demo in a different place.")
        Print.error("Abort \"biodynamo demo {}\".".format(demo_name))
        sys.exit(2)

    src_dir = os.path.join(DEMO_DIR, demo_name)
    print("Copying files from \"{}\" to \"{}\"...".format(src_dir, destination))
    shutil.copytree(src_dir, destination)

    CopySupportFiles(destination)

    InitializeNewGitRepo(destination)

    Print.success("The demo \"{}\" has been created in \"{}\".".format(
        demo_name, destination))
