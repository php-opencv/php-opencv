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

#ifndef PHP_OPENCV_OBJDETECT_H
#define PHP_OPENCV_OBJDETECT_H

#include "../../php_opencv.h"
#include <opencv2/objdetect.hpp>

extern zend_class_entry *opencv_cascade_classifier_ce;
extern zend_class_entry *opencv_facedetectoryn_ce;

#define Z_PHP_CASCADE_CLASSIFIER_OBJ_P(zv)  get_cascade_classifier_obj(Z_OBJ_P(zv))
#define Z_PHP_FACEDETECTORYN_OBJ_P(zv)  get_facedetectoryn_obj(Z_OBJ_P(zv))

typedef struct _opencv_cascade_classifier_object{
    CascadeClassifier *cascadeClassifier;
    zend_object std;
}opencv_cascade_classifier_object;

#ifdef OPENCV_OBJDETECT_FACE_HPP
typedef struct _opencv_facedetectoryn_object{
    Ptr<FaceDetectorYN> facedetectoryn;
    zend_object std;
}opencv_facedetectoryn_object;
#endif

extern void opencv_objdetect_init(int module_number);

static inline opencv_cascade_classifier_object* get_cascade_classifier_obj(zend_object *obj) {
    return (opencv_cascade_classifier_object*)((char*)(obj) - XtOffsetOf(opencv_cascade_classifier_object, std));
}

static inline opencv_facedetectoryn_object* get_facedetectoryn_obj(zend_object *obj) {
    return (opencv_facedetectoryn_object*)((char*)(obj) - XtOffsetOf(opencv_facedetectoryn_object, std));
}


#endif //OPENCV_OBJDETECT_H
