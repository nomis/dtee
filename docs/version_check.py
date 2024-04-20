import re
import sys

changelog = sys.argv[1]
sphinx_config = sys.argv[2]
build_version = sys.argv[3]
copyright_year = sys.argv[4][-4:]
copyright_config = re.compile(r"^copyright = u'[^ ']*(?P<year>....), .+$")
release_heading = re.compile(r"^(?P<version>[0-9]+\.[0-9]+\.[0-9]+)_ \|--\| (?P<date>[0-9]{4}-[0-9]{2}-[0-9]{2})$")

ok = False
for line in open(sphinx_config, "rt"):
	match = copyright_config.match(line)
	if match:
		match = match.groupdict()
		if match["year"] != copyright_year:
			print("Copyright year is " + copyright_year + " but docs config copyright year is " + match["year"], file=sys.stderr)
			sys.exit(1)
		else:
			ok = True
if not ok:
	print("Missing docs config copyright year", file=sys.stderr)
	sys.exit(1)

for line in open(changelog, "rt"):
	match = release_heading.match(line)
	if match:
		match = match.groupdict()
		if match["version"] != build_version:
			print("Latest release is " + match["version"] + " but build version is " + build_version, file=sys.stderr)
			sys.exit(1)
		elif not match["date"].startswith(copyright_year):
			print("Latest release is on " + match["date"] + " but copyright year is " + copyright_year, file=sys.stderr)
			sys.exit(1)
		else:
			sys.exit(0)

print("Unknown release")
sys.exit(1)
