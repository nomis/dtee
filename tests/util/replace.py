import sys

content = sys.stdin.read()

with open("strerror.txt", "r") as f:
	for from_str, to_str in zip(*[iter(f)]*2):
		content = content.replace(from_str.strip(), to_str.strip())

sys.stdout.write(content)
