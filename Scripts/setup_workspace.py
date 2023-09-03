import os

workspace_name = input("Workspace Name: ")
project_name = input("Project Name: ")

os.rename("Sandbox", project_name)

premake_file = open("premake5.lua", "r+")

s = premake_file.read()

s = s.replace("OpenGL", workspace_name)
s = s.replace("Sandbox", project_name)

premake_file.seek(0)
premake_file.truncate(0)
premake_file.write(s)

premake_file.close()
