import os
import sys

STREAM_SIZE = 65536

def dump(file, /, path=".", fd=1):
	if path == ".":
		dirname = os.path.dirname(__file__)

	tmp = path+file
	if os.path.exists(tmp) and tmp != "":
		dirname = os.path.dirname(tmp)
	
	filepath = os.path.join(dirname, file)

	pipe = sys.stdout if fd == 1 else sys.stdin
	with open(filepath, "r") as f:
		while True:
			blk = f.read(STREAM_SIZE)

			if not blk:
				break

			pipe.write(blk)

	pipe.flush()

if __name__ == "__main__":
	arg = len(sys.argv[1:])

	if arg == 0:
		print("Please, pass a filename.\n")
		exit()

	if arg == 1:
		dump(sys.argv[1])
	elif arg == 2:
		dump(sys.argv[1], sys.argv[2])
	else:
		dump(sys.argv[1], sys.argv[2], sys.argv[3])
