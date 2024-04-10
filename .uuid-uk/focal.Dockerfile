# Ubuntu 20.04
ARG SPHINX=4
FROM ubuntu:focal AS dtee-ubuntu-focal-base
ENV DEBIAN_FRONTEND=noninteractive

RUN rm /etc/apt/apt.conf.d/docker-clean
RUN echo 'APT::Keep-Downloaded-Packages "true";' >/etc/apt/apt.conf.d/keep-downloaded-packages

RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=ubuntu-focal-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=ubuntu-focal-var-lib-apt \
	apt-get update
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=ubuntu-focal-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=ubuntu-focal-var-lib-apt \
	apt-get install -y build-essential git procps python3-pip
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=ubuntu-focal-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=ubuntu-focal-var-lib-apt \
	apt-get install -y libboost-all-dev gettext lcov
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=ubuntu-focal-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=ubuntu-focal-var-lib-apt \
	apt-get install -y \
		gcc-7 gcc-8 gcc-9 gcc-10 \
		g++-7 g++-8 g++-9 g++-10 \
		clang-6.0 clang-7 clang-8 clang-9 clang-10 clang-11 clang-12 libc++abi-dev

# A non-C locale is required for testing gettext()
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=ubuntu-focal-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=ubuntu-focal-var-lib-apt \
	apt-get install -y locales

RUN sed -E 's/^# (en_AU.+ UTF-8)$/\1/' -i /etc/locale.gen
RUN locale-gen

ADD https://pypi.org/pypi/gcovr/json /dev/shm/gcovr.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=ubuntu-focal-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install -U gcovr

ADD https://pypi.org/pypi/ninja/json /dev/shm/ninja.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=ubuntu-focal-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install -U ninja

ADD https://pypi.org/pypi/meson/json /dev/shm/meson.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=ubuntu-focal-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install -U meson

ADD https://pypi.org/pypi/Sphinx/json /dev/shm/Sphinx.json


FROM dtee-ubuntu-focal-base AS dtee-ubuntu-focal-sphinx-4

RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=ubuntu-focal-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install -U "Sphinx~=4.0" \
		"sphinxcontrib-applehelp<1.0.5" "sphinxcontrib-devhelp<1.0.3" \
		"sphinxcontrib-htmlhelp<2.0.2" "sphinxcontrib-jsmath<1.0.2" \
		"sphinxcontrib-qthelp<1.0.4" "sphinxcontrib-serializinghtml<1.1.6"

FROM dtee-ubuntu-focal-sphinx-${SPHINX} AS dtee-ubuntu-focal-final
