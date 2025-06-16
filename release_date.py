"""Get SOURCE_DATE_EPOCH from the CHANGELOG for a release version"""
from datetime import datetime, timezone
import argparse
import os
import re
import sys

CHANGELOG_FILENAME = os.path.join(os.path.dirname(__file__), "CHANGELOG.rst")


def for_version(changelog, build_version):
	release_heading = re.compile(r"^(?P<version>[0-9]+\.[0-9]+\.[0-9]+)_ \|--\| (?P<date>[0-9]{4}-[0-9]{2}-[0-9]{2})$")

	for line in open(changelog, "rt"):
		match = release_heading.match(line)
		if match:
			match = match.groupdict()
			if match["version"] == build_version:
				return match["date"]


def unix_time(release_date):
	return int(
		datetime.strptime(release_date, "%Y-%m-%d")
		.replace(tzinfo=timezone.utc)
		.timestamp()
	)


if __name__ == "__main__":
	parser = argparse.ArgumentParser(description="Find date for a release version")
	parser.add_argument("version", metavar="VERSION", help="Release version")
	parser.add_argument(
		"-e",
		"--epoch",
		action="store_true",
		help="Output a Unix time value",
	)
	args = parser.parse_args()

	release_date = for_version(CHANGELOG_FILENAME, args.version)
	if release_date is None:
		print("Unknown release", file=sys.stderr)
		sys.exit(1)

	if args.epoch:
		print(unix_time(release_date))
	else:
		print(release_date)

	sys.exit(0)
