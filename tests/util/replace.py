import sys

content = sys.stdin.read()

for replace_file in ["strerror.txt", "strsignal.txt"]:
	with open(replace_file, "r") as f:
		for from_str, to_str in zip(*[iter(f)]*2):
			content = content.replace(from_str.strip(), to_str.strip())

sys.stdout.write(content)
