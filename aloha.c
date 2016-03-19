#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file_output.h"


/* Possible states */
#define STATE_SILENT 0
#define STATE_RETX 1
#define STATE_TX 2

/* Simulation static parameters */
#define ALPHA 0.004
#define N 100
#define T 100000

/* Probability of retransmission. */
static double p = 0.04;

/* Simulation time. */
long t = 0;


/*
 * Incrementally computes mean delay using Welford's algorithm.
 */
double update_mean_delay (double sample)
{
        static int i = 1;
	static double sample_mean = 0.0;

        sample_mean = sample_mean + (1.0/(double)i)*(sample - sample_mean);

        ++i;
        return sample_mean;
}

/*
 * Parses a probability value given a string.
 */
double parse_probability (char *string)
{
	if (string == NULL || strlen(string) < 1)
		return -1.0;
	
	char *endptr;
	double d = strtod(string, &endptr);

	if (*endptr != '\0') {
		return -1.0;
	}

	return d;
}

/*
 * Parses CLI arguments and validates input.
 * In case of errors, execution fails.
 */
void parse_args (int argc, char **argv)
{
	if (argc == 2) {
		p = parse_probability(argv[1]);
		if (p < 0.0 || p > 1.0) {
			fprintf(stderr, "Invalid parameter for probability!\n");
			exit(EXIT_FAILURE);
		}
	} else if (argc > 2) {
		fprintf(stderr, "Usage: %s [<retransmission_probability>]\n", *argv);
	}
}

int main (int argc, char **argv)
{
	int state[N];
	long tx_time[N];
	FILE *delays_file = init_out_file("delays.dat");
	FILE *queue_file = init_out_file("queue.dat");
	int i;
	unsigned long transmitted_packets = 0;
	double mean_delay = 0.0;

	/* Initializes state for each user. */
	for (i=0; i<N; ++i) {
		state[i] = STATE_SILENT;
	}

	/* Parses argument (if available) */
	parse_args(argc, argv);


	for (t = 0; t < T; ++t) {
		for (i=0; i<N; ++i) {
			if (state[i] == STATE_SILENT) {
				if ((double)rand()/(double)RAND_MAX <= ALPHA) {
					/* i transmits ... */
					tx_time[i] = t;
					state[i] = STATE_TX;
				}
			} else {
				if ((double)rand()/(double)RAND_MAX <= p) {
					/* i retransmits ... */
					state[i] = STATE_TX;
				}
			}
		}

		/* Verifies collisions/transmission */
		unsigned int tx_counter = 0;
		unsigned int retx_counter = 0;
		int last_tx_user = -1;
		for (i=0; i<N; ++i) {
			if (state[i] == STATE_TX) {
				++tx_counter;
				++retx_counter;
				last_tx_user = i;
			} else if (state[i] == STATE_RETX) {
				++retx_counter;
			}
		}

		if (tx_counter == 1) {
			state[last_tx_user] = STATE_SILENT;
			transmitted_packets++;
			long delay = t - tx_time[last_tx_user];
			write_long(delays_file, delay);
			mean_delay = update_mean_delay(delay);
		} else if (tx_counter > 1) {
			/* Collision */
			for (i=0; i<N; ++i) {
				if (state[i] == STATE_TX) {
					state[i] = STATE_RETX;
				}
			}
		}

		write_uint(queue_file, retx_counter);
	}


	printf("p = %.3f\n", p);
	printf("Transmitted packets: %lu\n", transmitted_packets);
	printf("Mean delay: %f\n", mean_delay);


	fclose(delays_file);
	fclose(queue_file);

	return EXIT_SUCCESS;
}

