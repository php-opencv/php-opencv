#include "../../../php_opencv.h"
#include "opencv_text_ocr.h"

#include "../core/opencv_mat.h"
#include "../../../opencv_exception.h"
using namespace cv::text;
using namespace std;
zend_class_entry  *opencv_text_ocr_ce;
zend_object_handlers opencv_text_ocr_object_handlers;

/***
 * 
*/

PHP_METHOD(opencv_text_ocr,create){
    char *datapath,*language,*char_whitelist;
    long datapath_len,language_len,char_whitelist_len;
    long oem=3,psmode=3 ;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "sssll",
                                &datapath, &datapath_len,
                                &language, &language_len,
                                &char_whitelist,&char_whitelist_len,
                                &oem,
                                &psmode
                                
    ) == FAILURE) {
        RETURN_NULL();
    }

    zval instance ;
    object_init_ex(&instance,opencv_text_ocr_ce);
    opencv_text_ocr_object *obj = Z_PHP_TEXT_OCR_OBJ_P(&instance);
    obj->ocr = OCRTesseract::create(datapath,language,char_whitelist,(int)oem,(int)psmode);
    RETURN_ZVAL(&instance,0,0);
}
ZEND_BEGIN_ARG_INFO_EX(opencv_create_arginfo, 0, 0, 5)
                ZEND_ARG_INFO(0, datapath)
                ZEND_ARG_INFO(0, language)
                ZEND_ARG_INFO(0, char_whitelist)
                ZEND_ARG_INFO(0, oem)
                ZEND_ARG_INFO(0, psmode)
ZEND_END_ARG_INFO()

PHP_METHOD(opencv_text_ocr,run){
    zval *image_zval;
    std::string output ;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "O",
                                &image_zval, opencv_mat_ce
    ) == FAILURE) {
        RETURN_NULL();
    }
        opencv_mat_object *image_obj ;
        image_obj = Z_PHP_MAT_OBJ_P(image_zval);
        opencv_text_ocr_object *obj =Z_PHP_TEXT_OCR_OBJ_P(getThis());
        try {
            obj->ocr->run(*image_obj->mat , output);
            RETURN_STRING(output.c_str());
        }catch (Exception e){
            opencv_throw_exception(e.what());
        }


}

ZEND_BEGIN_ARG_INFO_EX(opencv_run_arginfo, 0, 0, 1)
                ZEND_ARG_INFO(0, image)
ZEND_END_ARG_INFO()

const zend_function_entry opencv_text_ocr_methods[] ={
    PHP_ME(opencv_text_ocr,create,opencv_create_arginfo,ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    PHP_ME(opencv_text_ocr,run,opencv_run_arginfo,ZEND_ACC_PUBLIC)
    PHP_FE_END

};

/***
 * 
 * 
*/
zend_object* opencv_text_ocr_handlers(zend_class_entry *type){
    size_t size = sizeof(opencv_text_ocr_object)+zend_object_properties_size(type);
    opencv_text_ocr_object *obj = (opencv_text_ocr_object *)ecalloc(1,size);
    memset(obj, 0, size);
    zend_object_std_init(&obj->std, type);
    object_properties_init(&obj->std, type);
    obj->std.ce = type;
    obj->std.handlers = &opencv_text_ocr_object_handlers;
    return &obj->std;
}


/**
 * 
*/

void opencv_text_ocr_free_obj(zend_object *object)
{
    opencv_text_ocr_object *obj;
    obj = get_text_ocr_obj(object);
    delete obj->ocr;
    zend_object_std_dtor(object);
}

void opencv_text_ocr_init(int module_number){
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce,OPENCV_TEXT_OCR_NS,"OCRTesseract",opencv_text_ocr_methods);
    opencv_text_ocr_ce = zend_register_internal_class_ex(&ce,opencv_text_ocr_ce);
    opencv_text_ocr_ce->create_object = opencv_text_ocr_handlers;
    memcpy(&opencv_text_ocr_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    opencv_text_ocr_object_handlers.clone_obj = NULL;
    opencv_text_ocr_object_handlers.free_obj = opencv_text_ocr_free_obj;

    opencv_text_ocr_object_handlers.offset = XtOffsetOf(opencv_text_ocr_object,std);

}