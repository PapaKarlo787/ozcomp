import re
import struct

def IEEE754(n) :
    return struct.unpack('I', struct.pack('f', n))[0]

labels = {}
to_rebuild = []
data_base = b''
reg_re = "r(\d|1[0-5])$"
float_re = "-?(0\.\d+|[1-9][0-9]*\.\d+)$"
int_re = "(0|[1-9][0-9]*|0x[0-9a-f]+)$"
label_re = "([a-z]\w*)$"
nl = 1

def add(data, l):
	err_rc(data)
	return rc(data, 0, 1)
	

def sub(data, l):
	err_rc(data)
	return rc(data, 2, 3)

	
def mul(data, l):
	err_rc(data)
	return rc(data, 4, 5)

	
def div(data, l):
	err_rc(data)
	return rc(data, 6, 7)


def mov(data, l):
	try:
		err_rc(data)
		return rc(data, 10, 11)
	except Exception:
		if re.match(reg_re, data[0]) and data[1] == ",":
			r = int(data[0][1:])
			result = get_mor(data[2:], l)
			if len(data) == 7:
				result[0] = result[0] + r
				result.insert(0, 12)
			else:
				result = [8, r] + result
		elif re.match(reg_re, data[-1]) and data[-2] == ",":
			r = int(data[-1][1:])
			result = get_mor(data[:-2], l)
			if len(data) == 7:
				result[0] = result[0] + r
				result.insert(0, 13)
			else:
				result = [9, r] + result
		else:
			raise Exception
		return bytes(result)


def and_(data, l):
	err_rc(data)
	return rc(data, 14, 15)


def or_(data, l):
	err_rc(data)
	return rc(data, 16, 17)


def xor(data, l):
	err_rc(data)
	return rc(data, 18, 19)


def cmp_(data, l):
	err_rc(data)
	return rc(data, 20, 21)


def jmp(data, l):
	return bytes(jump(22, data, l))


def jc(data, cond, l):
	bit = 0 if cond[0] == 'n' else 1
	byte = 0
	for i in "gle":
		byte *= 2
		byte += bit if i in cond else (bit + 1) % 2
	result = jump(23, data, l + 1)
	result.insert(1, byte)
	return bytes(result)


def loop(data, l):
	return bytes(jump(24, data))


def push(data, l):
	if not re.match(reg_re, data[0]):
		raise Exception
	return bytes([25, int(data[0][1:])])


def pop(data, l):
	if not re.match(reg_re, data[0]):
		raise Exception
	return bytes([26, int(data[0][1:])])


def mod(data, l):
	err_rc(data)
	return rc(data, 27, 28)


def print_(data, l):
	if re.match(reg_re, data[0]):
		return bytes([29, int(data[0][1:])])
	return bytes(jump(30, data))


def delay(data, l):
	if re.match(reg_re, data[0]):
		return bytes([31, int(data[0][1:])])
	x = float(int(data[0], 16) if "x" in data[0] else data[0])
	if x < 0:
		raise Exception
	x = IEEE754(x)
	result = [32]
	to_bytes(result, x)
	return bytes(result)


def send(data, l):
	if re.match(reg_re, data[0]):
		return bytes([33, int(data[0][1:])])
	x = int(data[0], 16 if "x" in data[0] else 10)
	return bytes([34, x])


def int_(data, l):
	if re.match(reg_re, data[0]):
		return bytes([35, int(data[0][1:])])
	raise Exception


def gkey(data, l):
	return bytes([36])


def setc(data, l):
	if data[1] == ",":
		if re.match(reg_re, data[0]) and re.match(reg_re, data[2]):
			return bytes([38, int(data[2][1:]) * 16 + int(data[0][1:])])
		c1 = int(data[0], 16 if "x" in data[0] else 10)
		c2 = int(data[2], 16 if "x" in data[2] else 10)
		if c1 > -1 and c2 > -1:
			return bytes([37, c1, c2])
	raise Exception


def draw(data, l):
	return bytes(jump(39, data, l))


def call(data, l):
	return bytes(jump(40, data, l))

def ret(data, l):
	return bytes([41])


def rnd(data, l):
	return bytes([42])


def data(data, l):
	if data[1] != "," or not re.match(int_re, data[2]):
		raise Exception
	n = int(data[2])
	result = []
	if re.match(float_re, data[0]):
		x = IEEE754(float(data[0]))
		to_bytes(result, x, 0)
		return bytes(result * n)
	elif re.match(int_re, data[0]):
		x = int(data[0], 16 if "x" in data[0] else 10)
	else:
		raise Exception
	while True:
		result.insert(0, x % 256)
		x //= 256
		if x == 0:
			break
	return bytes(result * n)


def err_rc(data):
	if len(data) != 3 or not (re.match(reg_re, data[0]) and data[1] == "," and 
	re.match("-?({})|{}|{}|{}".format(int_re, float_re, reg_re, label_re), data[2])):
		raise Exception


def rc(data, f, s, l=0):
	if data[2][0] == "r":
		result = [f]
		result.append(int(data[2][1:]) * 16 + int(data[0][1:]))
	elif re.match(label_re, data[2]):
		result = [s]
		if data[2] in labels:
			to_bytes(result, labels[data[2]])
		else:
			to_rebuild.append((data[2], l + 1, nl))
			result += [0] * 4
	else:
		result = [s, int(data[0][1:])]
		x = IEEE754(float(int(data[2], 16) if "x" in data[2] else data[2]))
		to_bytes(result, x, 2)
	return bytes(result)


def to_bytes(result, x, n = 1):
	for i in range(4):
		result.insert(n, x % 256)
		x //= 256
	if x > 0:
		raise Exception("Too big number")


def jump(n, data, l=0):
	result = [n]
	if re.match(int_re, data[0]):
		to_bytes(result, int(data[0], 16 if "x" in data[0] else 10))
	elif re.match(label_re, data[0]):
		if data[0] in labels:
			to_bytes(result, labels[data[0]])
		else:
			to_rebuild.append((data[0], l + 1, nl))
			result += [0] * 4
	else:
		raise Exception
	return result


def get_mor(data, l=0):
	result = []
	if len(data) == 3 and data[0] == "[" and data[2] == "]":
		if data[1] in labels:
			to_bytes(result, labels[data[1]], 0)
		elif re.match(reg_re, data[1]):
			result = [int(data[1][1:])*16, 0, 0, 0, 0]
		else:
			to_rebuild.append((data[1], l + 2, nl))
			result = [0] * 4
	elif (len(data) == 5 and data[0] == "[" and data[4] == "]"
		and data[2] == "+" and re.match(reg_re, data[3])):
		if re.match(reg_re, data[3]):
			result = [int(data[3][1:])*16]
		else:
			raise Exception("No such register")
		if data[1] in labels:
			to_bytes(result, labels[data[1]])
		else:
			to_rebuild.append((data[1], l + 2, nl))
			result += [0] * 4
	else:
		raise Exception
	return result
