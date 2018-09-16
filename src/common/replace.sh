#!/bin/bash


grep_results=`grep -rl $1 ./`
echo $grep_results

echo $1
echo $2
echo $3

replace="sed -i s/$1/$2/g $grep_results"
echo $replace

$replace
