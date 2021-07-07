import nums_to_bytes as ntb
import args
import re

labels = {}
to_rebuild = []
data_base = b''


def ariphmetics(data, l, k1, k2, f=False):
	try:
		return bytes([k1]) + args.rr(data)
	except Exception:
		return bytes([k2]) + args.rc(data, l, to_rebuild, f)


def add(data, l):
	return ariphmetics(data, l, 0, 1)


def fadd(data, l):
	return ariphmetics(data, l, 67, 68, True)


def sub(data, l):
	return ariphmetics(data, l, 2, 3)


def fsub(data, l):
	return ariphmetics(data, l, 69, 70, True)


def mul(data, l):
	return ariphmetics(data, l, 4, 5)


def fmul(data, l):
	return ariphmetics(data, l, 71, 72, True)


def div(data, l):
	return ariphmetics(data, l, 6, 7)


def fdiv(data, l):
	return ariphmetics(data, l, 73, 74, True)


def pow_(data, l):
	return ariphmetics(data, l, 48, 49)


def fpow_(data, l):
	return ariphmetics(data, l, 75, 76, True)


def and_(data, l):
	return ariphmetics(data, l, 14, 15)


def or_(data, l):
	return ariphmetics(data, l, 16, 17)


def xor(data, l):
	return ariphmetics(data, l, 18, 19)


def shr(data, l, k1=79, k2=80):
	try:
		return bytes([k1]) + args.rr(data)
	except Exception:
		return bytes([k2]) + args.rcb(data)


def shl(data, l):
	return shr(data, l, 81, 82)


def cmp_(data, l):
	return ariphmetics(data, l, 20, 21)


def fcmp_(data, l):
	return ariphmetics(data, l, 77, 78, True)


def mod(data, l):
	return ariphmetics(data, l, 27, 28)


def jmp(data, l):
	return args.jump(22, data, l, to_rebuild)


def loop(data, l):
	return args.jump(24, data, l, to_rebuild)


def call(data, l):
	return args.jump(40, data, l, to_rebuild)


def play(data, l):
	return args.jump(91, data, l, to_rebuild)


def nplay(data, l):
	return bytes([92])

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


def fpush(data, l):
	return push(data, l, 89)


def fpop(data, l):
	return push(data, l, 90)


def print_int(data, l):
	return print_(data, l, 43)


def scol(data, l):
	return bytes([66]) + args.c(data)


def point(data, l):
	return setc(data, l, (51, 50))


def mov(data, l, k1=8, k2=9, k3=10, k4=11, k5=12, k6=13):
	try:
		return ariphmetics(data, l, k3, k4, k1 != 8)
	except Exception:
		return movb(data, l, k1, k2, k5, k6)


def icvtf(data, l):
	return bytes([93]) + args.rr(data)


def fcvti(data, l):
	return bytes([94]) + args.rr(data)


def fmov(data, l):
	return mov(data, l, 83, 84, 85, 86, 87, 88)


def movw(data, l):
	return movb(data, l, 99, 100, 101, 102)


def movb(data, l, k1=44, k2=45, k3=46, k4=47):
	if args.is_reg(data[0]) and data[1] == ",":
		result = args.get_mor(data[2:], l, k1, k3, to_rebuild)
		result[1] += int(data[0][1:])
	elif args.is_reg(data[-1]) and data[-2] == ",":
		result = args.get_mor(data[:-2], l, k2, k4, to_rebuild)
		result[1] += int(data[-1][1:])
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


def inc_nl():
	args.nl += 1


def get_nl():
	return args.nl
