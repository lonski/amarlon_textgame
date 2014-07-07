#!/bin/bash
err_file=cppcheck_err.txt

cppcheck --enable=style,performance,portability --std=c++11 --suppressions-list=cppcheck_suppressions.txt $1 .  2> $err_file

err_size=$(wc -c "$err_file" | cut -f 1 -d ' ')

if [ $err_size -eq  0 ]
then
	rm $err_file
fi

if [ $err_size -gt 0 ]
then
	nano cppcheck_err.txt
else
	echo ""
	echo -e "\e[32m-=NO WARRINGS! YAY!=-\e[0m"
	echo ""
fi
