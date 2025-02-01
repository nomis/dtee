# Debian 12
ARG SPHINX=7
FROM debian:bookworm AS dtee-debian-bookworm-base
ENV DEBIAN_FRONTEND=noninteractive

RUN rm /etc/apt/apt.conf.d/docker-clean
RUN echo 'APT::Keep-Downloaded-Packages "true";' >/etc/apt/apt.conf.d/keep-downloaded-packages

RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=debian-bookworm-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=debian-bookworm-var-lib-apt \
	apt-get update && apt-get install -y build-essential git procps python3-pip
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=debian-bookworm-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=debian-bookworm-var-lib-apt \
	apt-get update && apt-get install -y libboost-all-dev gettext lcov
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=debian-bookworm-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=debian-bookworm-var-lib-apt \
	apt-get update && apt-get install -y \
		gcc-11 gcc-12 \
		g++-11 g++-12 \
		clang-13 clang-14 clang-15 clang-16 libc++abi-dev

# A non-C locale is required for testing gettext()
RUN \
	--mount=type=cache,sharing=locked,target=/var/cache/apt,id=debian-bookworm-var-cache-apt \
	--mount=type=cache,sharing=locked,target=/var/lib/apt,id=debian-bookworm-var-lib-apt \
	apt-get update && apt-get install -y locales

RUN sed -E 's/^# (en_AU.+ UTF-8)$/\1/' -i /etc/locale.gen
RUN locale-gen

ADD https://pypi.org/pypi/gcovr/json /dev/shm/gcovr.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=debian-bookworm-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install --break-system-packages -U gcovr

ADD https://pypi.org/pypi/ninja/json /dev/shm/ninja.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=debian-bookworm-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install --break-system-packages -U ninja

ADD https://pypi.org/pypi/meson/json /dev/shm/meson.json
RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=debian-bookworm-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install --break-system-packages -U meson

ADD https://pypi.org/pypi/Sphinx/json /dev/shm/Sphinx.json


FROM dtee-debian-bookworm-base AS dtee-debian-bookworm-sphinx-5

RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=debian-bookworm-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install --break-system-packages -U "Sphinx~=5.0"

FROM dtee-debian-bookworm-base AS dtee-debian-bookworm-sphinx-6

RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=debian-bookworm-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install --break-system-packages -U "Sphinx~=6.0"

FROM dtee-debian-bookworm-base AS dtee-debian-bookworm-sphinx-7

RUN \
	--mount=type=cache,target=/root/.cache/pip,sharing=locked,id=debian-bookworm-root-cache-pip \
	python3 --version && \
	pip3 --version && \
	pip3 install --break-system-packages -U "Sphinx~=7.0"

FROM dtee-debian-bookworm-sphinx-${SPHINX} AS dtee-debian-bookworm-final
