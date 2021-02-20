import re
import nums_to_bytes as ntb


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
		return ntb.float_to_bytes(float(elem))
	if re.match(int_re, elem):
		return ntb.num_to_bytes(int(elem, 16 if "x" in elem else 10))
	if len(elem) > 1 and elem[0] == '"' and elem[-1] == '"':
		return list(elem[1:-1].encode())
	raise Exception
