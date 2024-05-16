FROM alpine:3.18 AS build
LABEL authors="Noahrav"
MAINTAINER Noahrav

RUN apk add --no-cache build-base \
  git \
  cmake \
  g++ \
  liblcf-dev \
  rapidjson-dev \
  icu-dev \
  icu-data-full

WORKDIR app

COPY CMakeLists.txt /app/CMakeLists.txt
COPY deps /app/deps
COPY src /app/src

RUN cmake -B /app/build -S /app
RUN cd /app/build && make

ENTRYPOINT ["./build/cu_submitter"]
