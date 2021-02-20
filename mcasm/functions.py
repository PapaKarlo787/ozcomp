import nums_to_bytes as ntb
import args
import re

labels = {}
to_rebuild = []
data_base = b''


def ariphmetics(data, k1, k2):
	try:
		return bytes([k1]) + args.rr(data)
	except Exception:
		return bytes([k2]) + args.rc(data)


def add(data, l):
	return ariphmetics(data, 0, 1)


def sub(data, l):
	return ariphmetics(data, 2, 3)


def mul(data, l):
	return ariphmetics(data, 4, 5)


def div(data, l):
	return ariphmetics(data, 6, 7)


def pow_(data, l):
	return ariphmetics(data, 48, 49)


def and_(data, l):
	return ariphmetics(data, 14, 15)


def or_(data, l):
	return ariphmetics(data, 16, 17)


def xor(data, l):
	return ariphmetics(data, 18, 19)


def cmp_(data, l):
	return ariphmetics(data, 20, 21)


def mod(data, l):
	return ariphmetics(data, 27, 28)


def jmp(data, l):
	return args.jump(22, data, l, to_rebuild)


def loop(data, l):
	return args.jump(24, data, l, to_rebuild)


def call(data, l):
	return args.jump(40, data, l, to_rebuild)


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


def push(data, l, k=25):
	return bytes([k]) + args.r(data)


def pop(data, l):
	return push(data, l, 26)


def print_int(data, l):
	return print_(data, l, 43)


def scol(data, l):
	return bytes([66]) + args.c(data)


def point(data, l):
	return setc(data, l, (51, 50))


def mov(data, l):
	try:
		return ariphmetics(data, 10, 11)
	except Exception:
		return movb(data, l, 12, 8, 13, 9)


def movb(data, l, k1=46, k2=44, k3=47, k4=45):
	if args.is_reg(data[0]) and data[1] == ",":
		r = int(data[0][1:])
		result = finish_mor(data, k1, k2, r, get_mor(data[2:], l))
	elif args.is_reg(data[-1]) and data[-2] == ",":
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
	result = list(args.jump(23, data, l + 1, to_rebuild))
	result.insert(1, byte)
	return bytes(result)


def print_(data, l, n=29):
	return bytes([n]) + args.r(data)


def lprint(data, l):
	return args.jump(30, data, l, to_rebuild)


def delay(data, l, k1=31, k2=32):
	try:
		return bytes([k1]) + args.r(data)
	except Exception:
		if len(data) == 1 and int(data[0]) >= 0:
			return bytes([k2]) + ntb.int_to_bytes(int(data[0]))


def send(data, l):
	return delay(data, l, 33, 34)


def circle(data, l):
	if len(data) == 5 and data[1] == ',':
		if args.is_reg(data[0]):
			return args.r(data[:1]) + args.rr(data[2:])
		else:
			res = args.c(data[:1])
			try:
				return res + args.rr(data[2:])
			except Exception:
				return res + args.cc(data[2:])


def line(data, l, k=(55,56,57)):
	if len(data) == 7 and data[3] == ",":
		res1 = setc(data[:3], l, k[:2])
		res2 = setc(data[-3:], l, k[:2])
		if res1[0] != res2[0] and res1[0] < res2[0]:
			res2 = bytes([k[2]]) + res2[1:]
		return res2+res1[1:]
	raise Exception


def rect(data, l):
	return line(data, l, (59, 60, 61))


def bmp(data, l):
	for f in [(args.rcb, 62), (args.cbr, 63), (args.rr, 64), (args.cc, 65)]:
		try:
			res = bytes([f[1]]) + f[0](data[:3])
			break
		except Exception:
			pass
	else:
		raise Exception
	return res + args.jump(0, data[-1:], l+len(res)-1, to_rebuild)[1:]


def int_(data, l):
	return bytes([35]) + args.r(data)


def setc(data, l, k=(37, 38)):
	try:
		return bytes([k[0]]) + args.rr(data)
	except Exception:
		return bytes([k[1]]) + args.cc(data)


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
		if args.is_reg(data[1]):
			result = [int(data[1][1:])*16]
		else:
			to_rebuild.append((data[1], l + 2, nl))
	elif (len(data) == 5 and data[0] == "[" and data[4] == "]"
		and data[2] == "+" and args.is_reg(data[3])):
		result = [int(data[3][1:])*16]
		to_rebuild.append((data[1], l + 2, args.nl))
	else:
		raise Exception
	return result + [0] * 4

def inc_nl():
	args.nl += 1

def get_nl():
	return args.nl
