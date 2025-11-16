#!/bin/sh
BINARY_HOME=./bin
INPUT_HOME=./input
INPUT=${INPUT_HOME}/AB_NYC_2019.csv

cat ${INPUT} | ${BINARY_HOME}/mapper_ave | sort -k1 | ${BINARY_HOME}/reducer_ave > output_ave
cat output_ave

cat ${INPUT} | ${BINARY_HOME}/mapper_dis | sort -k1 | ${BINARY_HOME}/reducer_dis > output_dis
cat output_dis
