#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** Simulation time. */
extern long t;


/**
 * Opens a file for writing simulation output to.
 *
 * @param filename File to open.
 * @return Pointer to open FILE or NULL in case of error.
 */
FILE *init_out_file (const char *filename)
{
	if (filename == NULL || strlen(filename) < 1)
		return NULL;

	return fopen(filename, "w");
}


void write_int (FILE *out_file, int val)
{
	fprintf(out_file, "%ld\t%d\n", t, val);
}

void write_uint (FILE *out_file, unsigned int val)
{
	fprintf(out_file, "%ld\t%u\n", t, val);
}

void write_long (FILE *out_file, long val)
{
	fprintf(out_file, "%ld\t%ld\n", t, val);
}
