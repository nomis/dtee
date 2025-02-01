# Ubuntu 18.04
FROM ubuntu:bionic
ENV DEBIAN_FRONTEND=noninteractive

RUN rm /etc/apt/apt.conf.d/docker-clean
RUN echo 'APT::Keep-Downloaded-Packages "true";' >/etc/apt/apt.conf.d/keep-downloaded-packages

RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=ubuntu-bionic-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=ubuntu-bionic-var-lib-apt \
	apt-get update && apt-get install -y build-essential git procps python3-pip
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=ubuntu-bionic-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=ubuntu-bionic-var-lib-apt \
	apt-get update && apt-get install -y libboost-all-dev gettext lcov
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=ubuntu-bionic-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=ubuntu-bionic-var-lib-apt \
	apt-get update && apt-get install -y \
		gcc-5 gcc-6 \
		g++-5 g++-6 \
		clang-5.0 libc++abi-dev

# Use Python 3.7 for Meson and Ninja
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=ubuntu-bionic-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=ubuntu-bionic-var-lib-apt \
	apt-get update && apt-get install -y python3.7-dev gcovr

ENV PYTHON=python3.7
ENV PIP="python3.7 -m pip"

# Upgrade pip because it's too old to install ninja 1.11.1.2
ADD https://pypi.org/pypi/pip/json /dev/shm/pip.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=ubuntu-bionic-root-cache-pip \
	python3.7 --version && \
	python3.7 -m pip --version && \
	python3.7 -m pip install -U pip

# A non-C locale is required for testing gettext()
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=ubuntu-bionic-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=ubuntu-bionic-var-lib-apt \
	apt-get update && apt-get install -y locales

RUN sed -E 's/^# (en_AU.+ UTF-8)$/\1/' -i /etc/locale.gen
RUN locale-gen

ADD https://pypi.org/pypi/ninja/json /dev/shm/ninja.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=ubuntu-bionic-root-cache-pip \
	python3.7 --version && \
	python3.7 -m pip --version && \
	python3.7 -m pip install -U ninja

ADD https://pypi.org/pypi/meson/json /dev/shm/meson.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=ubuntu-bionic-root-cache-pip \
	python3.7 --version && \
	python3.7 -m pip --version && \
	python3.7 -m pip install -U meson

ADD https://pypi.org/pypi/Sphinx/json /dev/shm/Sphinx.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=ubuntu-bionic-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install -U Sphinx
