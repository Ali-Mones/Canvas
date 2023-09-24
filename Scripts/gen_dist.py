import os
import shutil

include_files = [
    "Application.h",
    "Canvas.h",
    "CanvasCamera.h",
    "Core.h",
    "EntryPoint.h",
    "Input.h"
]

lib = "Canvas.lib"
dll = "Canvas.dll"

main_file_content = \
"""#include "include/EntryPoint.h"
#include "include/Canvas.h"

void Setup()
{
}

void Draw()
{
}
"""

src_path = "../Canvas/src/"
bin_path = "../bin/Release-windows-x86_64/Canvas/"

if "Canvas" not in os.listdir("."):
    os.mkdir("Canvas")
if "include" not in os.listdir("./Canvas"):
    os.mkdir("./Canvas/include")
if "lib" not in os.listdir("./Canvas"):
    os.mkdir("./Canvas/lib")

for include in include_files:
    shutil.copy(f"{src_path + include}", "./Canvas/include/")

shutil.copy(f"{bin_path + lib}", "./Canvas/lib")
shutil.copy(f"{bin_path + dll}", "./Canvas/")

with open("./Canvas/main.cpp", "x") as file:
    file.write(main_file_content)

shutil.make_archive("Canvas-dist", "zip", "Canvas")
shutil.rmtree("Canvas")
