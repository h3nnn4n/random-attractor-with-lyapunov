#!/bin/bash

for x in `ls -w 1 *.ppm`;
do
	j=`echo $x | tr -d .pm`
	convert $x w_$j.png
	echo $x to w_$j.png
done
