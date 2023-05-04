import subprocess
import sys
import platform

def main():
    
    premake_action = ''

    if len(sys.argv) >= 2 and sys.argv[1] == "clean":
        premake_action = 'clean'
    else:
        if platform.system() == "Windows":
            premake_action = 'vs2022'
        elif platform.system() == "Darwin":
            premake_action = 'xcode4'

    subprocess.Popen('ThirdParty\premake\premake5.exe --file=Scripts/premake.lua ' + premake_action, shell=True).communicate()

    sys.exit(0)

if __name__ == "__main__":
    main()