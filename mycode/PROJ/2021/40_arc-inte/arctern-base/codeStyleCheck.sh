#!/bin/bash

for i in  $(find ./UnitTest -name *.h -o -name *.cpp); do
	python2 ./Tools/code-style/cpplint.py --linelength=120 $i
done


#for i in  $(find ./src -name *.h -o -name *.cpp); do
#	python2 ./Tools/code-style/cpplint.py --linelength=120 $i
#done

#for i in  $(find ./Example -name *.h -o -name *.cpp); do
#	python2 ./Tools/code-style/cpplint.py --linelength=120  $i
#done

