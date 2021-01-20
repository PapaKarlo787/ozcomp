import re
import argparse
import sys
import os
from functions import *
from extensions import *


commands = {"add": add, "sub": sub, "mul": mul, "div": div, "mov": mov,
			"and": and_, "or": or_, "xor": xor, "cmp": cmp_, "int": int_,
			"jmp": jmp, "loop": loop, "push": push, "pop": pop,
			"mod": mod, "print": print_, "delay": delay, "send": send,
			"gkey": gkey, "setc": setc, "draw": draw, "data": data,
			"call": call, "ret": ret, "rnd": rnd, "iprint": print_int,
			"dd": dd, "movb": movb, "pow": pow_, "point": point,
			"circle": circle, "line": line}


pattern = re.compile(r"\".*\"|\[|\]|\+|-?[\w\.]+|,|:|;.*|-")


def manage_line(data):
	global data_base, addr, nl
	if data and data[-1][0] == ";":
		data.pop()
	if not data:
		return
	if data[0] in commands:
		data_base += commands[data[0]](data[1:], len(data_base))
	elif len(data) == 2 and data[1] == ":":
		if data[0] in labels:
			raise Exception("Label '{}' already presents".format(data[0]))
		if not re.match(label_re, data[0]):
			raise Exception("Wrong label name")
		labels[data[0]] = len(data_base)
	elif re.match("jn?[egl]$", data[0]):
		data_base += jc(data[1:], data[0][1:], len(data_base))
	elif data[0] == "include" and len(data) == 2 and "\"\"" == data[1][0]+data[1][-1]:
		nl, _ = (nl, start(data[1][1:-1]))
	else:
		raise Exception


def start(fn, visited=[]):
	global nl
	if os.path.abspath(fn) in visited:
		raise Exception("file {} already included".format(fn))
	visited.append(os.path.abspath(fn))
	with open(fn) as f:
		for l in f:
			try:
				manage_line(pattern.findall(l.lower()))
				nl += 1
			except Exception as e:
				msg = str(e) if str(e) else "Wrong line"
				raise Exception("{}: {} (line {})".format(fn, msg, nl))


def add_labels():
	global nl, data_base
	for d in to_rebuild:
		result = []
		if d[0] not in labels:
			nl = d[2]
			raise Exception("No such label")
		to_bytes(result, labels[d[0]], 0)
		data_base = data_base[:d[1]] + bytes(result) + data_base[d[1]+4:]


def save(fn):
	with open(fn, 'wb') as f:
		f.write(data_base)
		f.flush()


if __name__ == "__main__":
	try:
		parser = argparse.ArgumentParser(description='Process some integers.')
		parser.add_argument("inf", help="input file")
		parser.add_argument("outf", help="output file")
		args = parser.parse_args()
		start(args.inf)
		add_labels()
		save(args.outf)
	except Exception as e:
		print(e)
		sys.exit(1)
