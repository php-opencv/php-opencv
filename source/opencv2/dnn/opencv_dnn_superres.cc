#include "../../../php_opencv.h"
#include "opencv_dnn_superres.h"


#include "../core/opencv_mat.h"
#include "../../../opencv_exception.h"

using namespace cv::dnn_superres;
using namespace std;
zend_class_entry *opencv_dnn_superres_ce;

zend_object_handlers opencv_dnn_superres_object_handlers;

/**
 * 
*/

PHP_METHOD(opencv_dnn_superres,create){
    zval instance ;
    object_init_ex(&instance,opencv_dnn_superres_ce);
    opencv_dnn_superres_object *obj = Z_PHP_DNN_SUPERRES_OBJ_P(&instance);
    obj->dnsr = DnnSuperResImpl::create();
    RETURN_ZVAL(&instance,0,0);
}


PHP_METHOD(opencv_dnn_superres,readModel){
        char *path ;
        size_t path_len;
        if(zend_parse_parameters(ZEND_NUM_ARGS(),"s",&path,&path_len) == FAILURE){
            RETURN_NULL();
        }

        opencv_dnn_superres_object *obj = Z_PHP_DNN_SUPERRES_OBJ_P(getThis());
        obj->dnsr->readModel(path);
        RETURN_NULL();
}
ZEND_BEGIN_ARG_INFO_EX(opencv_readModel_arginfo, 0, 0, 1)
                ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()
PHP_METHOD(opencv_dnn_superres, setModel){
        char *algo ;
        size_t algo_len;
        long  scale; 
        if(zend_parse_parameters(ZEND_NUM_ARGS(),"s|l",&algo,&algo_len,&scale) == FAILURE){
            RETURN_NULL();
        }

        opencv_dnn_superres_object *obj = Z_PHP_DNN_SUPERRES_OBJ_P(getThis());
        obj->dnsr->setModel(algo,scale);
        RETURN_NULL();
}
ZEND_BEGIN_ARG_INFO_EX(opencv_setModel_arginfo, 0, 0, 2)
                ZEND_ARG_INFO(0, algo)
                ZEND_ARG_INFO(0, scale)
ZEND_END_ARG_INFO()
PHP_METHOD(opencv_dnn_superres, upsample){
   zval *image_zval ,*result_zval;
       if (zend_parse_parameters(ZEND_NUM_ARGS(), "Oz",
                                &image_zval, opencv_mat_ce,
                                &result_zval
                                
    ) == FAILURE) {
        RETURN_NULL();
    }
    opencv_mat_object *image_obj ,*result_object;

    image_obj = Z_PHP_MAT_OBJ_P(image_zval);
     zval *result_real_zval = Z_REFVAL_P(result_zval);
    if(Z_TYPE_P(result_real_zval) == IS_OBJECT && Z_OBJCE_P(result_real_zval)==opencv_mat_ce){
        result_object = Z_PHP_MAT_OBJ_P(result_real_zval);
    } else{
        zval_ptr_dtor(result_real_zval);
        zval instance;
        Mat result;
        object_init_ex(&instance,opencv_mat_ce);
        ZVAL_COPY_VALUE(result_real_zval, &instance);
        result_object = Z_PHP_MAT_OBJ_P(result_real_zval);
        result_object->mat =  new Mat(result);
    }
        try {
        opencv_dnn_superres_object *obj = Z_PHP_DNN_SUPERRES_OBJ_P(getThis());
        obj->dnsr->upsample(*image_obj->mat,*result_object->mat);
        opencv_mat_update_property_by_c_mat(result_real_zval, result_object->mat);

    }catch (Exception e){
        opencv_throw_exception(e.what());
    }
    RETURN_NULL();
}
ZEND_BEGIN_ARG_INFO_EX(opencv_upsample_arginfo, 0, 0, 2)
                ZEND_ARG_INFO(0, image)
                ZEND_ARG_INFO(1, result)
ZEND_END_ARG_INFO()
/**
 * opencv_dnn_net_methods[]
*/
ZEND_BEGIN_ARG_INFO_EX(arginfo_void, 0, 0, 0)
ZEND_END_ARG_INFO()
const zend_function_entry opencv_dnn_superres_methods[] ={
    PHP_ME(opencv_dnn_superres,create,arginfo_void, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    PHP_ME(opencv_dnn_superres,readModel, opencv_readModel_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(opencv_dnn_superres, setModel, opencv_setModel_arginfo, ZEND_ACC_PUBLIC)
    PHP_ME(opencv_dnn_superres, upsample, opencv_upsample_arginfo, ZEND_ACC_PUBLIC)
    PHP_FE_END
};



zend_object* opencv_dnn_superres_handlers(zend_class_entry *type)
{
    size_t size = sizeof(opencv_dnn_superres_object)+zend_object_properties_size(type);
    opencv_dnn_superres_object *obj = (opencv_dnn_superres_object *)ecalloc(1,size);
    memset(obj, 0, size);
    zend_object_std_init(&obj->std, type);
    object_properties_init(&obj->std, type);
    obj->std.ce = type;
    obj->std.handlers = &opencv_dnn_superres_object_handlers;
    return &obj->std;
}

/**
 * 
*/
void opencv_dnn_superres_free_obj(zend_object *object)
{
    opencv_dnn_superres_object *obj;
    obj = get_dnn_superres_obj(object);
    delete obj->dnsr;
    zend_object_std_dtor(object);
}

void opencv_dnn_superres_init(int module_number){
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce,OPENCV_DNN_SUPERRES_NS,"DnnSuperResImpl",opencv_dnn_superres_methods);
    opencv_dnn_superres_ce = zend_register_internal_class_ex(&ce,opencv_dnn_superres_ce);
    opencv_dnn_superres_ce->create_object = opencv_dnn_superres_handlers;
    memcpy(&opencv_dnn_superres_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

    opencv_dnn_superres_object_handlers.clone_obj = NULL;
    opencv_dnn_superres_object_handlers.free_obj = opencv_dnn_superres_free_obj;

    opencv_dnn_superres_object_handlers.offset = XtOffsetOf(opencv_dnn_superres_object,std);
}