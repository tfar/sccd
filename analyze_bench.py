#!/usr/bin/env python

import sys  # input ouput
import re	# regular expressions
from tabulate import tabulate # table rendering on terminal
import numpy

warmup_items = 2

bench_cycles = {}

p = re.compile(ur'bench\(([^\s]+)\): ([^\s]+) cycles')

for line in sys.stdin:
	line_match = re.match(p, line)
	if line_match:
		name, cycles = line_match.groups()
		if not name in bench_cycles:
			bench_cycles[name] = []
		bench_cycles[name].append(int(cycles))

result_table = []

number_of_runs = None

for benchmark, v in bench_cycles.iteritems():
	if not number_of_runs:
		number_of_runs = len(v)
	else:
		assert(len(v) == number_of_runs)

	npvalues = numpy.array(v[warmup_items:])
	result_table.append(
		[benchmark,
		numpy.amin(npvalues),
		numpy.average(npvalues),
		numpy.amax(npvalues),
		numpy.std(npvalues),
		numpy.std(npvalues) * 100/numpy.average(npvalues)])

result_table.sort(key=lambda x: x[0])

print "\nNumber of runs: %d (first %d ignored)\n" % (number_of_runs, warmup_items)
print tabulate(result_table, headers = ["Benchmark", "Minimum", "Average", "Maximum", "SD", "RSD (%)"], floatfmt=".3f")
