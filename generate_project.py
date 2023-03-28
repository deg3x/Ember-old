import subprocess
import sys
import platform

if platform.system() == "Windows":
    subprocess.Popen('ThirdParty\premake\premake5.exe --file=Scripts/premake.lua vs2022', shell=True).communicate()
elif platform.system() == "Darwin":
    subprocess.Popen('./ThirdParty/premake/premake5 --file=Scripts/premake.lua xcode4', shell=True).communicate()


sys.exit(0)