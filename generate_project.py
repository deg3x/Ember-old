import subprocess
import sys
import platform

def main():
    
    premake_action = ''
    executable = ''

    if platform.system() == "Windows":
        premake_action = 'vs2022'
        executable = 'ThirdParty\premake\premake5.exe'
    elif platform.system() == "Darwin":
        premake_action = 'xcode4'
        executable = 'ThirdParty/premake/premake5'

    if len(sys.argv) >= 2 and sys.argv[1] == "clean":
        premake_action = 'clean'
        

    subprocess.Popen(executable + ' --file=Scripts/premake.lua ' + premake_action, shell=True).communicate()

    sys.exit(0)

if __name__ == "__main__":
    main()