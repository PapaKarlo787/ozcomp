import re
import nums_to_bytes as ntb


reg_re = "r(\d|1[0-5])$"
float_re = "-?(0\.\d+|[1-9][0-9]*\.\d+)$"
int_re = "(0|-?[1-9][0-9]*|0x[0-9a-f]+)$"
label_re = "([a-z]\w*)$"
nl = 1


def is_three(data):
	return len(data) == 3 and data[1] == ","


def is_reg(d):
	return re.match(reg_re, d)


def to_int(n):
	return int(n, 16 if "x" in n else 10)


def rc(data):
	if is_three(data) and re.match(reg_re, data[0]):
		r = bytes([int(data[0][1:])])
		if re.match(int_re, data[2]):
			return r + ntb.int_to_bytes(int(data[2]))
		if re.match(float_re, data[2]):
			return r + ntb.float_to_bytes(float(data[2]))
	raise Exception


def rcb(data):
	if is_three(data):
		return r(data[:1])+c(data[2:])
	raise Exception


def cbr(data):
	return rcb(data[::-1])[::-1]


def rr(data):
	if is_three(data) and re.match(reg_re, data[0]) and re.match(reg_re, data[2]):
		return bytes([int(data[2][1:]) * 16 + int(data[0][1:])])
	raise Exception


def cc(data):
	if is_three(data):
		x = c(data[:1]) + c(data[2:])
		if len(x) == 2:
			return x
	raise Exception


def r(data):
	if len(data) == 1 and re.match(reg_re, data[0]):
		return bytes([int(data[0][1:])])
	raise Exception


def c(data):
	if len(data) == 1 and re.match(int_re, data[0]):
		return bytes([to_int(data[0])])
	raise Exception


def jump(c, data, l, to_rebuild):
	result = bytes([c])
	if re.match(int_re, data[0]):
		result += ntb.int_to_bytes(to_int(data[0]))
	elif re.match(label_re, data[0]):
		to_rebuild.append((data[0], l + 1, nl))
		result += bytes([0] * 4)
	else:
		raise Exception
	return result
