set build_path=vs2015.win64
rmdir /s/q %build_path%
mkdir %build_path%
cd %build_path%
cmake .. -G"Visual Studio 15 Win64"

start ClassTool.sln
pause