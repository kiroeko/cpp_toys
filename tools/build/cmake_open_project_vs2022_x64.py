import os, re, shutil

currentpath = os.path.split(os.path.realpath(__file__))[0]
projectrootpath = re.sub(r"[\\/][^\\/]*[\\/][^\\/]*$", "", currentpath)

# Clear project
cmakefilespath = projectrootpath + "/temp/cmakefiles"
if os.path.exists(cmakefilespath) :
   shutil.rmtree(cmakefilespath)
else :
   os.makedirs(cmakefilespath)

# Call cmake
os.system(f"cd ../.. && cmake -S . -B \"{cmakefilespath}\" -G \"Visual Studio 17 2022\" -A x64 && cmake --open \"{cmakefilespath}\"")