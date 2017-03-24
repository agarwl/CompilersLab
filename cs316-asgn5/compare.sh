#!/bin/bash

rm -rf MIPSCODE REFERCODE comparison.txt

MIPS='./MIPSCODE/'
REFER='./REFERCODE/'
# FLAGS='-icode -ast -symtab -d'
FLAGS='-d'
EXT='.dce'
mkdir MIPSCODE
mkdir REFERCODE
for f in ./testcases/*.c
do
	filename=$(echo $f | cut -d'/' -f 3)
	filename=$(echo $filename | cut -d'.' -f 1)
	./sclp $FLAGS $f >& $MIPS$filename.txt
	if [ -f $f$EXT ]; then
		# echo "FILE $f$EXT exists"
		file_exists=true
		# echo '$$$$$$$'
		mv $f$EXT $MIPS$filename$EXT
	fi
	./sclp_gullu $FLAGS $f >& $REFER$filename.txt
	if [ -f $f$EXT ]; then
		echo "FILE $f$EXT exists"
	# 	mv $f$EXT $REFER$filename$EXT
	fi
	echo "The Comparison for the file $f is:" >> comparison.txt
	# diff -w -i $MIPS$filename.txt $REFER$filename.txt >> comparison.txt
	# if [ file_exists ]; then
	# 	diff -w -i $MIPS$filename$EXT $REFER$filename$EXT >> comparison.txt
	# else
	# 	diff -w -i $MIPS$filename.txt $REFER$filename.txt >> comparison.txt
	# fi
	file_exists=false
	echo "-------------------------------------" >> comparison.txt
done