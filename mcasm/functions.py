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
	return rc(data, 0, 1)


def sub(data, l):
	return rc(data, 2, 3)

	
def mul(data, l):
	return rc(data, 4, 5)

	
def div(data, l):
	return rc(data, 6, 7)


def pow_(data, l):
	return rc(data, 48, 49)


def and_(data, l):
	return rc(data, 14, 15)


def or_(data, l):
	return rc(data, 16, 17)


def xor(data, l):
	return rc(data, 18, 19)


def cmp_(data, l):
	return rc(data, 20, 21)


def jmp(data, l):
	return bytes(jump(22, data, l))


def mod(data, l):
	return rc(data, 27, 28)


def loop(data, l):
	return bytes(jump(24, data, l))


def mov(data, l):
	try:
		return rc(data, 10, 11)
	except Exception:
		return movb(data, l, 12, 8, 13, 9)


def movb(data, l, k1=46, k2=44, k3=47, k4=45):
	if re.match(reg_re, data[0]) and data[1] == ",":
		r = int(data[0][1:])
		result = finish_mor(data, k1, k2, r, get_mor(data[2:], l))
	elif re.match(reg_re, data[-1]) and data[-2] == ",":
		r = int(data[-1][1:])
		result = finish_mor(data, k3, k4, r, get_mor(data[:-2], l))
	else:
		raise Exception
	return bytes(result)


def jc(data, cond, l):
	bit = 0 if 'n' in cond[0] else 1
	byte = 0
	for i in "gle":
		byte *= 2
		byte += bit if i in cond else (bit + 1) % 2
	result = jump(23, data, l + 1)
	result.insert(1, byte)
	return bytes(result)


def push(data, l, k=25):
	if not re.match(reg_re, data[0]):
		raise Exception
	return bytes([k, int(data[0][1:])])


def pop(data, l):
	return push(data, l, 26)


def print_int(data, l):
	return print_(data, l, 43)


def print_(data, l, n=29):
	if re.match(reg_re, data[0]):
		return bytes([n, int(data[0][1:])])
	return bytes(jump(30, data, l))


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


def point(data, l):
	return setc(data, l, (51, 50))


def circle(data, l):
	if len(data) == 5 and data[1] == data[3] and data[3] == ',':
		if re.match(float_re, data[0]) or re.match(int_re, data[0]):
			r = int(data[0], 16 if "x" in data[0] else 10)
			if not (re.match(int_re, data[2]) and re.match(int_re, data[4])):
				raise Exception
			result = list(map(int, data[2::2]))
			to_bytes(result, IEEE754(r), 0)
			print(result)
			return bytes([53] + result)
		if re.match(reg_re, data[0]) and re.match(reg_re, data[2]) and re.match(reg_re, data[4]):
			data = list(map(lambda x: int(x[1:]), data[::2]))
			return bytes([52, data[0], data[2]*16+data[1]])
	raise Exception


def line(data, l):
	res1 = setc(data[:3], l, (55, 54))
	res2 = setc(data[-3:], l, (55, 54))
	if len(data) == 7 and res1[0] == res2[0]:
		return res2+res1[1:]
	raise Exception


def int_(data, l):
	if re.match(reg_re, data[0]):
		return bytes([35, int(data[0][1:])])
	raise Exception


def gkey(data, l):
	return bytes([36])


def setc(data, l, k=(37, 38)):
	if data[1] == ",":
		if re.match(reg_re, data[0]) and re.match(reg_re, data[2]):
			return bytes([k[1], int(data[2][1:]) * 16 + int(data[0][1:])])
		c1 = int(data[0], 16 if "0x" == data[0][:2] else 10)
		c2 = int(data[2], 16 if "0x" == data[2][:2] else 10)
		if c1 > -1 and c2 > -1:
			return bytes([k[0], c2, c1])
	raise Exception


def draw(data, l):
	return bytes([39])


def call(data, l):
	return bytes(jump(40, data, l))


def ret(data, l):
	return bytes([41])


def rnd(data, l):
	return bytes([42])


def err_rc(data):
	if len(data) != 3 or not (re.match(reg_re, data[0]) and data[1] == "," and 
	re.match("-?({})|{}|{}|{}".format(int_re, float_re, reg_re, label_re), data[2])):
		raise Exception


def rc(data, f, s, l=0):
	err_rc(data)
	if data[2][0] == "r":
		result = [f]
		result.append(int(data[2][1:]) * 16 + int(data[0][1:]))
	elif re.match(label_re, data[2]):
		result = [s, 0, 0, 0, 0]
		to_rebuild.append((data[2], l + 1, nl))
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
		to_rebuild.append((data[0], l + 1, nl))
		result += [0] * 4
	else:
		raise Exception
	return result


def finish_mor(data, k1, k2, r, result):
	if len(data) == 7:
		result[0] += r
		result.insert(0, k1)
	else:
		result = [k2, r] + result
	return result


def get_mor(data, l=0):
	result = []
	if len(data) == 3 and data[0] == "[" and data[2] == "]":
		if re.match(reg_re, data[1]):
			result = [int(data[1][1:])*16]
		else:
			to_rebuild.append((data[1], l + 2, nl))
	elif (len(data) == 5 and data[0] == "[" and data[4] == "]"
		and data[2] == "+" and re.match(reg_re, data[3])):
		if re.match(reg_re, data[3]):
			result = [int(data[3][1:])*16]
		else:
			raise Exception("No such register")
		to_rebuild.append((data[1], l + 2, nl))
	else:
		raise Exception
	return result + [0] * 4
