FROM ubuntu:latest as executor

USER root

RUN apt-get update && \
    apt-get -y install --no-install-recommends \
    make \
    g++ \
    cmake

COPY . /app
COPY . /var/www

WORKDIR /app/build

RUN cmake .. && cmake --build .

CMD ./highload_server

EXPOSE 80
