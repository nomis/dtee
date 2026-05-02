# Ubuntu 26.04
FROM ubuntu:resolute
ENV DEBIAN_FRONTEND=noninteractive

RUN rm /etc/apt/apt.conf.d/docker-clean
RUN echo 'APT::Keep-Downloaded-Packages "true";' >/etc/apt/apt.conf.d/keep-downloaded-packages

RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=ubuntu-resolute-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=ubuntu-resolute-var-lib-apt \
	apt-get update && apt-get install -y build-essential git procps python3-pip
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=ubuntu-resolute-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=ubuntu-resolute-var-lib-apt \
	apt-get update && apt-get install -y libboost-all-dev gettext lcov
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=ubuntu-resolute-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=ubuntu-resolute-var-lib-apt \
	apt-get update && apt-get install -y \
		gcc-15 gcc-16 \
		g++-15 g++-16 \
		clang-20 clang-21 clang-22 libc++abi-dev

# A non-C locale is required for testing gettext()
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=ubuntu-resolute-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=ubuntu-resolute-var-lib-apt \
	apt-get update && apt-get install -y locales

RUN sed -E 's/^# (en_AU.+ UTF-8)$/\1/' -i /etc/locale.gen
RUN locale-gen

ADD https://pypi.org/pypi/gcovr/json /dev/shm/gcovr.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=ubuntu-resolute-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install --break-system-packages -U gcovr

ADD https://pypi.org/pypi/Sphinx/json /dev/shm/Sphinx.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=ubuntu-resolute-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install --break-system-packages -U Sphinx

ADD https://pypi.org/pypi/ninja/json /dev/shm/ninja.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=ubuntu-resolute-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install --break-system-packages -U ninja

ADD https://pypi.org/pypi/meson/json /dev/shm/meson.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=ubuntu-resolute-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install --break-system-packages -U meson
