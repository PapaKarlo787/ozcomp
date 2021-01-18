import re
import struct


reg_re = "r(\d|1[0-5])$"
float_re = "-?(0\.\d+|[1-9][0-9]*\.\d+)$"
int_re = "(0|[1-9][0-9]*|0x[0-9a-f]+)$"
label_re = "([a-z]\w*)$"


def data(data, l):
	if data[1] != "," or not re.match(int_re, data[2]):
		raise Exception
	return bytes(get_data_element(data[0]) * int(data[2]))


def dd(data, l):
	result = b''
	for e in map(get_data_element, data[::2]):
		result += bytes(e)
	return result


def get_data_element(elem):
	result = []
	if re.match(float_re, elem):
		x = IEEE754(float(elem))
	elif re.match(int_re, elem):
		x = int(elem, 16 if "x" in elem else 10)
	elif len(elem) > 1 and elem[0] == '\"' and elem[-1] == '\"':
		return list(elem[1:-1].encode())
	else:
		raise Exception
	num_to_bytes(result, x, 0)
	return result


def IEEE754(n) :
    return struct.unpack('I', struct.pack('f', n))[0]


def num_to_bytes(result, x, n = 1):
	while True:
		result.insert(n, x % 256)
		x //= 256
		if not x:
			break
