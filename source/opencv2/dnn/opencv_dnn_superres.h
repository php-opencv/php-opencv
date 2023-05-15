#ifndef PHP_OPENCV_DNN_SUPERRES_H 
#define PHP_OPENCV_DNN_SUPERRES_H 
#include <opencv2/dnn_superres.hpp>

extern void opencv_dnn_superres_init(int module_number) ;


extern zend_class_entry *opencv_dnn_superres_ce;

typedef struct _opencv_dnn_superres_object{
    Ptr<cv::dnn_superres::DnnSuperResImpl> dnsr;
    zend_object std;
}opencv_dnn_superres_object;

#define Z_PHP_DNN_SUPERRES_OBJ_P(zv) get_dnn_superres_obj(Z_OBJ_P(zv))

static inline opencv_dnn_superres_object* get_dnn_superres_obj(zend_object *obj){
    return (opencv_dnn_superres_object*)((char*)(obj) - XtOffsetOf(opencv_dnn_superres_object,std));
}

#endif



