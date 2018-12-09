from __future__ import print_function

import re
import sys

changelog = sys.argv[1]
build_version = sys.argv[2]
release_heading = re.compile(r"^(?P<version>[0-9]+\.[0-9]+\.[0-9]+)_ \|--\| (?P<date>[0-9]{4}-[0-9]{2}-[0-9]{2})$")

for line in open(changelog, "rt"):
	match = release_heading.match(line)
	if match:
		match = match.groupdict()
		if match["version"] != build_version:
			print("Latest release is " + match["version"] + " but build version is " + build_version, file=sys.stderr)
			sys.exit(1)
		else:
			sys.exit(0)

print("Unknown release")
sys.exit(1)
