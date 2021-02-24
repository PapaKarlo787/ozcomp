import re
import argparse
import sys
import os
import nums_to_bytes as ntb
from functions import *
from extensions import *


commands = {"add": add, "sub": sub, "mul": mul, "div": div, "mov": mov,
			"and": and_, "or": or_, "xor": xor, "cmp": cmp_, "int": int_,
			"jmp": jmp, "loop": loop, "push": push, "pop": pop,
			"mod": mod, "print": print_, "delay": delay, "send": send,
			"gkey": gkey, "scur": setc, "draw": draw, "data": data,
			"call": call, "ret": ret, "rnd": rnd, "iprint": print_int,
			"dd": dd, "movb": movb, "pow": pow_, "point": point,
			"circle": circle, "line": line, "rect": rect, "cls": cls,
			"bmp": bmp, "scol": scol, "lprint": lprint, "fmov": fmov,
			"fpush": fpush, "fpop": fpop, "shr": shr, "shl": shl,
			"fadd": fadd, "fsub": fsub, "fmul": fmul, "fdiv": fdiv,
			"fcmp": fcmp_, "fpow": fpow_, "play": play, "nplay": nplay}


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
	elif re.match("jn?[egli]$", data[0]):
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
				l = l.split("\"")
				for i in range(len(l)):
					if not i % 2:
						l[i] = l[i].lower()
				l = "\"".join(l)
				manage_line(pattern.findall(l))
				inc_nl()
			except Exception as e:
				msg = str(e) if str(e) else "Wrong line"
				raise Exception("{}: {} (line {})".format(fn, msg, get_nl()))


def add_labels(org):
	global nl, data_base
	for d in to_rebuild:
		if d[0] not in labels:
			raise Exception("No such label - {} (line {})".format(d[0], d[2]))
		x = list(data_base[d[1]:d[1]+4])
		for i in range(4):
			x[i] <<= i * 8
		result = ntb.int_to_bytes(labels[d[0]]+org+sum(x))
		data_base = data_base[:d[1]] + result + data_base[d[1]+4:]


def save(fn):
	with open(fn, 'wb') as f:
		f.write(data_base)
		f.flush()


if __name__ == "__main__":
#	try:
		parser = argparse.ArgumentParser(description='Process some integers.')
		parser.add_argument("inf", help="input file")
		parser.add_argument("outf", help="output file")
		parser.add_argument("org", type=int, default=0,
							help="open file", nargs='?')
		cmd = parser.parse_args()
		start(cmd.inf)
		add_labels(cmd.org)
		save(cmd.outf)
#	except Exception as e:
#		print(e)
#		sys.exit(1)
