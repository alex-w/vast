#!/usr/bin/env bash

echo -n "Looking for PSFEx...  "

command -v psfex &>psfex_path.tmp && cat psfex_path.tmp && mv psfex_path.tmp psfex.found && exit

 echo "ERROR!"
 echo " "
 echo "Can't find a PSFEx executable in \$PATH"
 echo " "
cat psfex_path.tmp
 echo " "
 echo "Please install PSFEx..."
 echo " "

rm -f psfex_path.tmp
