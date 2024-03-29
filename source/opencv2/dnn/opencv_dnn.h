/*
 +----------------------------------------------------------------------+
 | PHP-OpenCV                                                           |
 +----------------------------------------------------------------------+
 | This source file is subject to version 2.0 of the Apache license,    |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.apache.org/licenses/LICENSE-2.0.html                      |
 | If you did not receive a copy of the Apache2.0 license and are unable|
 | to obtain it through the world-wide-web, please send a note to       |
 | hihozhou@gmail.com so we can mail you a copy immediately.            |
 +----------------------------------------------------------------------+
 | Author: HaiHao Zhou  <hihozhou@gmail.com>                            |
 +----------------------------------------------------------------------+
 */


#ifndef PHP_OPENCV_DNN_NET_H
#define PHP_OPENCV_DNN_NET_H

#include <opencv2/dnn.hpp>
#include <string>
#include <fstream>
#include <streambuf>


extern void opencv_dnn_init(int module_number);

#endif
PHP_FUNCTION(opencv_dnn_blob_from_image);
PHP_FUNCTION(opencv_dnn_blob_from_images);
PHP_FUNCTION(opencv_dnn_read_net_from_torch);
PHP_FUNCTION(opencv_dnn_read_net_from_caffe);
PHP_FUNCTION(opencv_dnn_read_net_from_tensorflow);
PHP_FUNCTION(opencv_dnn_read_net_from_onnx);
PHP_FUNCTION(opencv_dnn_read_net_from_model_optimizer);
PHP_FUNCTION(opencv_dnn_read_net_from_darknet);
