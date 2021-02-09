FROM morozovsk/php-opencv:travis

#FROM ubuntu:20.04
#RUN apt update && export DEBIAN_FRONTEND=noninteractive && apt install -y wget cmake git php-common pkg-config
#RUN apt install -y software-properties-common
#RUN add-apt-repository ppa:ondrej/php
#RUN apt update
#RUN export DEBIAN_FRONTEND=noninteractive && apt install -y php8.0 php8.0-dev
#RUN wget https://raw.githubusercontent.com/php-opencv/php-opencv-packages/master/opencv_4.5.0_amd64.deb && dpkg -i opencv_4.5.0_amd64.deb && rm opencv_4.5.0_amd64.deb

RUN git clone https://github.com/php-opencv/php-opencv.git

RUN cd php-opencv && phpize && ./configure --with-php-config=/usr/bin/php-config && make && make install && make test

RUN echo "extension=opencv.so" > /etc/php/8.0/cli/conf.d/opencv.ini