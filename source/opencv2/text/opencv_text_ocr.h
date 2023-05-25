#ifndef PHP_OPENCV_TEXT_OCR_H
#ifndef PHP_OPENCV_TEXT_OCR_H
#define PHP_OPENCV_TEXT_OCR_H
#include<opencv2/text/ocr.hpp>

extern void opencv_text_ocr_init(int module_number);

extern zend_class_entry  *opencv_text_ocr_ce;

typedef struct _opencv_text_ocr_object{
    Ptr<cv::text::OCRTesseract> ocr;
    zend_object std;
}opencv_text_ocr_object;

#define Z_PHP_TEXT_OCR_OBJ_P(zv) get_text_ocr_obj(Z_OBJ_P(zv))

static inline opencv_text_ocr_object* get_text_ocr_obj(zend_object *obj){
    return (opencv_text_ocr_object*)((char*)(obj) - XtOffsetOf(opencv_text_ocr_object,std));
}


#endif