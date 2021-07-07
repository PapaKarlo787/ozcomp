import re
import nums_to_bytes as ntb
from args import float_re, int_re


def to_int(n):
	return int(n, 16 if "x" in n else 10)


def times(data, l, cmd):
	if re.match(int_re, data[-1]):
		return cmd[data[0]](data[1:-1], l) * to_int(data[-1])
	raise Exception


def _db(e):
	if re.match(int_re, e):
		return bytes([to_int(e)])
	if len(e) > 1 and e[0] == '"' and e[-1] == '"':
		return e[1:-1].encode()
	raise Exception


def _df(e):
	if re.match(float_re, e):
		return ntb.float_to_bytes(float(e))
	raise Exception


def _dw(e):
	if re.match(int_re, e):
		return ntb.int_to_bytes(to_int(e), 2)
	raise Exception


def _dd(e):
	if re.match(int_re, e):
		return ntb.int_to_bytes(to_int(e))
	raise Exception


def dd(data, l, f=_dd):
	res = b''
	if re.match(",*", "".join(data[1:-1:2])) and len(data) % 2:
		for x in map(f, data[::2]):
			res += x
		return res
	raise Exception


def db(data, l):
	return dd(data, l, _db)


def dw(data, l):
	return dd(data, l, _dw)


def df(data, l):
	return dd(data, l, _df)


def long_(data, l):
	if re.match(int_re, data[0]):
		return num_to_bytes(to_int(data[0]))
	raise Exception
