#####################################################
# Group: Davis Doan(cssc1090) & Jon Verne(cssc1145)
# CS 570: Operating Systems Summer 2017
# Assignment 3: Interrupt, Clock, & Timer
# Filename: makefile
#####################################################

EXE= aclock
FILE= main.c
CC = g++
flags =-pthread

make: 
	$(CC) $(flags) -o$(EXE) $(FILE)

clean:main.c
	rm aclock
