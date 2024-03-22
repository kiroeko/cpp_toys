$currentpath = $PSScriptRoot
$projectrootpath = (get-item $currentpath).parent.parent.FullName

# Clear project
$cmakefilespath = $projectrootpath + "/temp/cmakefiles"
if (Test-Path -Path $cmakefilespath)
{
    Remove-Item -Recurse -Force -Path $cmakefilespath
} 
else 
{
    $null = New-Item -ItemType Directory -Path $cmakefilespath
}

# Call cmake
(cd ../..)
(cmake -S . -B "$cmakefilespath" -G "Visual Studio 17 2022" -A x64)
(cmake --build "$cmakefilespath" --config Release)

cd $currentpath