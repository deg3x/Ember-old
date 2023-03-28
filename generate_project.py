import subprocess
import sys

subprocess.Popen('ThirdParty\premake\premake5.exe --file=Scripts/premake.lua vs2022', shell=True).communicate()

sys.exit(0)