FROM ubuntu:20.10

RUN apt update && export DEBIAN_FRONTEND=noninteractive && apt install -y wget pkg-config cmake git php7.4-common php7.4-cli php7.4-dev

RUN wget https://raw.githubusercontent.com/php-opencv/php-opencv-packages/master/opencv_4.5.0_amd64.deb && dpkg -i opencv_4.5.0_amd64.deb && rm opencv_4.5.0_amd64.deb

RUN git clone https://github.com/php-opencv/php-opencv.git

RUN cd php-opencv && phpize && ./configure --with-php-config=/usr/bin/php-config && make && make install && make test