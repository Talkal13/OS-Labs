#!/bin/bash

MPOINT="./mount-point"

rm -R -f temp
mkdir temp

#1
#Linux filesystem
cp ./src/fuseLib.c ./temp/filea.txt

# FUSE filesystem
cp ./temp/filea.txt $MPOINT/

#echo ./src/fuseLib.c > ./temp/filea.txt
cp ./src/myFS.h ./temp/fileb.txt
cp ./temp/fileb.txt $MPOINT/
#echo ./src/myFS.h > ./temp/fileb.txt

#2

./my-fsck virtual-disk
if diff ./temp/filea.txt $MPOINT/filea.txt ; then
	echo "Success comparing file filea.txt :)";
else 
	echo "Failure comparing file filea.txt :(";
fi
if diff ./temp/fileb.txt $MPOINT/fileb.txt ; then
	echo "Success comparing file fileb.txt :)"
else 
	echo "Failure comparing file fileb.txt :("
fi

truncate ./temp/filea.txt -o -s -1
truncate ./$MPOINT/filea.txt -o -s -1

./my-fsck virtual-disk
if diff ./temp/filea.txt $MPOINT/filea.txt ; then
	echo "Success comparing file filea.txt :)"
else 
	echo "Failure comparing file filea.txt :("
fi

#3

cp ./src/fuseLib.h $MPOINT/filec.txt
./my-fsck virtual-disk
if diff ./src/fuseLib.h $MPOINT/filec.txt ; then
	echo "Success comparing file filec.txt :)" 
else 
	echo "Failure comparing file filec.txt :("
fi

truncate ./temp/fileb.txt -o -s +1
truncate ./$MPOINT/fileb.txt -o -s +1

./my-fsck virtual-disk
if diff ./temp/fileb.txt $MPOINT/fileb.txt ; then
	echo "Success comparing file fileb.txt :)"
else 
	echo "Failure comparing file fileb.txt :("
fi
 
