/**
 * Group: Davis Doan(cssc1090) & Jon Verne(cssc1145)
 * CS 570 Operating Systems Summer 2017
 * Assignment 1: Process Manager
 * Filename: header.h
 */

#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
using namespace std;

#define N 7

sem_t FLAG;

void write_pid();
void create_threads();
void create_semaphore();
void *worker_thread(void *arg);

#endif
