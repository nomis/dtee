# Debian 13
FROM debian:trixie
ENV DEBIAN_FRONTEND=noninteractive

RUN rm /etc/apt/apt.conf.d/docker-clean
RUN echo 'APT::Keep-Downloaded-Packages "true";' >/etc/apt/apt.conf.d/keep-downloaded-packages

RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=debian-trixie-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=debian-trixie-var-lib-apt \
	apt-get update
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=debian-trixie-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=debian-trixie-var-lib-apt \
	apt-get install -y build-essential git procps python3-pip
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=debian-trixie-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=debian-trixie-var-lib-apt \
	apt-get install -y libboost-all-dev gettext lcov
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=debian-trixie-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=debian-trixie-var-lib-apt \
	apt-get install -y \
		gcc-13 gcc-14 \
		g++-13 g++-14 \
		clang-17 libc++abi-dev

# A non-C locale is required for testing gettext()
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=debian-trixie-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=debian-trixie-var-lib-apt \
	apt-get install -y locales

RUN sed -E 's/^# (en_AU.+ UTF-8)$/\1/' -i /etc/locale.gen
RUN locale-gen

# Workaround for https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=1088217
RUN \
	sed -i /var/lib/dpkg/status \
		-e 's/Depends: libpmix2t64 (= 5.0.4-1), libevent-dev, libhwloc-dev, zlib1g-dev, python3-sphinx, libjs-jquery/Depends: libpmix2t64 (= 5.0.4-1), libevent-dev, libhwloc-dev, zlib1g-dev, libjs-jquery/'
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=debian-trixie-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=debian-trixie-var-lib-apt \
	apt-get autoremove -y

ADD https://pypi.org/pypi/gcovr/json /dev/shm/gcovr.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=debian-trixie-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install --break-system-packages -U gcovr

ADD https://pypi.org/pypi/Sphinx/json /dev/shm/Sphinx.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=debian-trixie-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install --break-system-packages -U Sphinx

ADD https://pypi.org/pypi/ninja/json /dev/shm/ninja.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=debian-trixie-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install --break-system-packages -U ninja

ADD https://pypi.org/pypi/meson/json /dev/shm/meson.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=debian-trixie-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install --break-system-packages -U meson
