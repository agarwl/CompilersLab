#!/bin/bash

rm -rf MIPSCODE REFERCODE comparison.txt

MIPS='./MIPSCODE/'
REFER='./REFERCODE/'
FLAGS='-d'
mkdir MIPSCODE
mkdir REFERCODE
for f in ./test_files/*.c
do
	filename=$(echo $f | cut -d'/' -f 3)
	filename=$(echo $filename | cut -d'.' -f 1)
	./sclp $FLAGS  $f > $MIPS$filename.s
	./sclp16 $FLAGS  $f > $REFER$filename.s
	echo "The Comparison for the file $f is:" >> comparison.txt
	(spim -f $MIPS$filename.s | tail -n +6 ) > $MIPS$filename.print
	(spim -f $REFER$filename.s | tail -n +6 ) > $REFER$filename.print
	# diff -w -i $MIPS$filename.s $REFER$filename.s >> comparison.txt
	diff -w -i $MIPS$filename.print $REFER$filename.print >> comparison.txt
	echo "-------------------------------------" >> comparison.txt
done