Slotted Aloha Simulation
========================

This is a very simple C simulator of Slotted ALOHA protocol.


Running simulator
-----------------
Simply run:

	make

You can then start simulation:

	./aloha

If you want, you can specify a value for retransmission probability `p`:

	./aloha 0.01


Experimenting with many values for `p`
--------------------------------------
The `experiments` directory contains scripts for experimenting with
many different values for retransmission probability. You can simulate
using values defined in `p_vs_delays.sh`:

	bash experiments/p_vs_delays.sh

You can then plot obtained results using `gnuplot` (if installed):

	gnuplot experiments/plot_p_vs_delays.gnuplot -p

You can also run both commands at once using:

	make experiment
