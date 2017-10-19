// functions that need to be used by multiple files
// for Assignment 2
// Coded by George Bai/Eu Shaun Lim

#ifndef FUNCTION_H
#define FUNCTION_H

#include "graph.h"

// get number of URLs
int getURLCount();

// get list of URLs
void getURLList(int url_count, char **list);

// normalise words by removing spaces and non-alphanumeric chars
void normalise(char word[]);

// strdup implementation
char *str_dup(char *old);

#endif
