import re

reg_re = "r(\d|1[0-5])$"
float_re = "-?(0\.\d+|[1-9][0-9]*\.\d+)$"
int_re = "(0|[1-9][0-9]*|0x[0-9a-f]+)$"
label_re = "([a-z]\w*)$"

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

def dd(data, l):
	result = []
	while data:
		if data[1] != ',':
			raise Exception
		if re.match(int_re, data[0]):
			data[0] = int(data[0])
		elif re.match(float_re, data[0]):
			data[0] = IEEE754(float(data[0]))
		else:
			raise Exception		
		loc_res = []
		while data[0]:
			loc_res.insert(0, data[0] % 256)
			data[0] //= 256
		result += loc_res
		data = data[2:]
	return bytes(result)
