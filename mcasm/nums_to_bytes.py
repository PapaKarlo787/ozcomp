import struct


def float_to_bytes(x):
    return int_to_bytes(struct.unpack('I', struct.pack('f', x))[0])


def num_to_bytes(n):
	res = []
	while True:
		res.append(n % 256)
		n //= 256
		if not n:
			break
	return bytes(res)


def int_to_bytes(n, k=4):
	res = []
	if n < 0:
		n += 256**4
	for i in range(k):
		res.append(n % 256)
		n //= 256
	if n:
		raise Exception
	return bytes(res)
