FROM ubuntu:latest as executor

USER root

RUN apt-get update && \
    apt-get -y install --no-install-recommends \
    make \
    g++ \
    cmake

COPY . /app
COPY . /var/www

RUN cmake .. && cmake --build .

WORKDIR /app/build

EXPOSE 80

CMD ./Highload_static_server
