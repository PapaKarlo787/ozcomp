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


def draw(data, l):
	return bytes([39])


def ret(data, l):
	return bytes([41])


def rnd(data, l):
	return bytes([42])


def gkey(data, l):
	return bytes([36])


def cls(data, l):
	return bytes([58])


def pop(data, l):
	return push(data, l, 26)


def print_int(data, l):
	return print_(data, l, 43)


def call(data, l):
	return bytes(jump(40, data, l))


def scol(data, l):
	if len(data) == 1:
		return bytes([66, int(data[0])])
	raise Exception


def point(data, l):
	return setc(data, l, (51, 50))


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
	for i in "igle":
		byte *= 2
		byte += bit if i in cond else (bit + 1) % 2
	result = jump(23, data, l + 1)
	result.insert(1, byte)
	return bytes(result)


def push(data, l, k=25):
	if not re.match(reg_re, data[0]):
		raise Exception
	return bytes([k, int(data[0][1:])])


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
	result = [32]
	result += to_bytes(IEEE754(x))
	return bytes(result)


def send(data, l):
	if re.match(reg_re, data[0]):
		return bytes([33, int(data[0][1:])])
	x = int(data[0], 16 if "x" in data[0] else 10)
	return bytes([34, x])


def circle(data, l):
	if len(data) == 5 and data[1] == data[3] and data[3] == ',':
		result = list(setc(data[2:], l, (52, 53)))
		if re.match(float_re, data[0]) or re.match(int_re, data[0]):
			r = int(data[0], 16) if "x" in data[0] else float(data[0])
			result += to_bytes(IEEE754(r))
			if result[0] == 52:
				result[0] = 54
		elif re.match(reg_re, data[0]):
			result.insert(1, int(data[0][1:]))
			if result[0] != 52:
				raise Exception
		else:
			raise Exception
		return bytes(result)


def line(data, l, k=(55,56,57)):
	res1 = setc(data[:3], l, k[:2])
	res2 = setc(data[-3:], l, k[:2])
	if res1[0] != res2[0]:
		res2 = bytes([k[2]]) + res2[1:]
	if len(data) == 7:
		return res2+res1[1:]
	raise Exception


def rect(data, l):
	return line(data, l, (59, 60, 61))


def bmp(data, l):
	if  len(data) < 5 or data[3] != ',' or data[1] != ',':
		raise Exception
	try:
		result = setc(data[:-2], l, (64, 65))
	except Exception:
		if re.match(reg_re, data[2]) and int(data[0]) in range(84):
			result = bytes([63, int(data[0]), int(data[2][1:])])
		elif re.match(reg_re, data[0]) and int(data[2]) in range(48):
			result = bytes([62, int(data[0][1:]), int(data[2])])
		else:
			raise Exception
	return result + bytes(jump(0, data[-1:], l + (1 if result[0] == 64 else 2))[1:])


def int_(data, l):
	if re.match(reg_re, data[0]):
		return bytes([35, int(data[0][1:])])
	raise Exception


def setc(data, l, k=(37, 38)):
	if data[1] == ",":
		if re.match(reg_re, data[0]) and re.match(reg_re, data[2]):
			return bytes([k[0], int(data[2][1:]) * 16 + int(data[0][1:])])
		c1 = int(data[0], 16 if "0x" == data[0][:2] else 10)
		c2 = int(data[2], 16 if "0x" == data[2][:2] else 10)
		if c1 > -1 and c2 > -1:
			return bytes([k[1], c2, c1])
	raise Exception


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
		result += to_bytes(x)
	return bytes(result)


def to_bytes(x):
	result = []
	for i in range(4):
		result.append(x % 256)
		x //= 256
	if x > 0:
		raise Exception("Too big number")
	return result


def jump(n, data, l=0):
	result = [n]
	if re.match(int_re, data[0]):
		result += to_bytes(int(data[0], 16 if "x" in data[0] else 10))
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
		result = [int(data[3][1:])*16]
		to_rebuild.append((data[1], l + 2, nl))
	else:
		raise Exception
	return result + [0] * 4
