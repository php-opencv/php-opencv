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

#include "../../php_opencv.h"
#include <opencv2/objdetect.hpp>
#include "opencv_objdetect.h"
#include "core/opencv_type.h"
#include "core/opencv_mat.h"

zend_class_entry *opencv_cascade_classifier_ce;
zend_class_entry *opencv_facedetectoryn_ce;
zend_class_entry *opencv_facerecognizersf_ce;

/**
 * -----------------------------------【CV\Cascadeclassifier】--------------------------
 *
 * -------------------------------------------------------------------------------------
 */


zend_object_handlers opencv_cascade_classifier_object_handlers;

/**
 * @param type
 * @return
 */
zend_object* opencv_cascade_classifier_create_handler(zend_class_entry *type)
{
    int size = sizeof(opencv_cascade_classifier_object)+zend_object_properties_size(type);
    opencv_cascade_classifier_object *obj = (opencv_cascade_classifier_object *)ecalloc(1,size);
    memset(obj, 0, size);
    zend_object_std_init(&obj->std, type);
    object_properties_init(&obj->std, type);
    obj->std.ce = type;
    obj->std.handlers = &opencv_cascade_classifier_object_handlers;
    return &obj->std;
}

/**
 * CascadeClassifier __construct
 * @param execute_data
 * @param return_value
 */
PHP_METHOD(opencv_cascade_classifier, __construct)
{
    opencv_cascade_classifier_object *obj = Z_PHP_CASCADE_CLASSIFIER_OBJ_P(getThis());
    obj->cascadeClassifier = new CascadeClassifier();
}

PHP_METHOD(opencv_cascade_classifier, load)
{
    char *filename;
    long filename_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &filename, &filename_len) == FAILURE) {
        RETURN_NULL();
    }
    opencv_cascade_classifier_object *this_obj = Z_PHP_CASCADE_CLASSIFIER_OBJ_P(getThis());
    bool result = this_obj->cascadeClassifier->load(filename);
    RETURN_BOOL(result);
}

ZEND_BEGIN_ARG_INFO_EX(opencv_cascade_classifier_detect_multi_scale_arginfo, 0, 0, 7)
                ZEND_ARG_INFO(0, image)
                ZEND_ARG_INFO(1, objects)
                ZEND_ARG_INFO(0, scaleFactor)
                ZEND_ARG_INFO(0, minNeighbors)
                ZEND_ARG_INFO(0, flags)
                ZEND_ARG_INFO(0, minSize)
                ZEND_ARG_INFO(0, maxSize)
ZEND_END_ARG_INFO()

/**
 * CascadeClassifier->detectMultiScale function
 * @param execute_data
 * @param return_value
 */
PHP_METHOD(opencv_cascade_classifier, detect_multi_scale)
{
    zval *image_zval, *objects_zval, *min_size_zval = NULL, *max_size_zval = NULL;
    double scale_factor = 1.1;
    long min_neighbors = 3, flags = 0;
    Size min_size = Size(), max_size = Size();
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "Oz|dllOO",
                              &image_zval, opencv_mat_ce,
                              &objects_zval, &scale_factor, &min_neighbors, &flags,
                              &min_size_zval, opencv_size_ce,
                              &max_size_zval, opencv_size_ce

    ) == FAILURE) {
        RETURN_NULL();
    }
    std::vector<Rect> objects;
    opencv_mat_object *image_object = Z_PHP_MAT_OBJ_P(image_zval);
    opencv_cascade_classifier_object *this_obj = Z_PHP_CASCADE_CLASSIFIER_OBJ_P(getThis());
    if(min_size_zval != NULL){
        opencv_size_object *min_size_object = Z_PHP_SIZE_OBJ_P(min_size_zval);
        min_size = *min_size_object->size;
    }

    if(max_size_zval != NULL){
        opencv_size_object *max_size_object = Z_PHP_SIZE_OBJ_P(max_size_zval);
        max_size = *max_size_object->size;
    }

    this_obj->cascadeClassifier->detectMultiScale(
            *(image_object->mat), objects, scale_factor,
            (int)min_neighbors, (int)flags,
            min_size, max_size);

    zval *objects_real_zval = Z_REFVAL_P(objects_zval);

    zval_dtor(objects_real_zval);//if real_zval value not eq null ,free real_zval to avoid memory leaks detected

    array_init(objects_real_zval);
    for(unsigned long i=0; i < objects.size(); i++){
        zval OPENCV_CONNECT(zval,i);
        Rect OPENCV_CONNECT(rect,i);
        opencv_rect_object *OPENCV_CONNECT(rect_object,i);
        object_init_ex(&OPENCV_CONNECT(zval,i), opencv_rect_ce);
        OPENCV_CONNECT(rect_object,i) = Z_PHP_RECT_OBJ_P(&OPENCV_CONNECT(zval,i));
        OPENCV_CONNECT(rect,i) = objects.at(i);
        OPENCV_CONNECT(rect_object,i)->rect = new Rect(OPENCV_CONNECT(rect,i));
        opencv_rect_update_property_by_c_rect(&OPENCV_CONNECT(zval,i), OPENCV_CONNECT(rect_object,i)->rect);

        add_next_index_zval(objects_real_zval,&OPENCV_CONNECT(zval,i));
    }

    RETURN_NULL();
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_void, 0, 0, 0)
ZEND_END_ARG_INFO()

/**
 * opencv_cascade_classifier_methods[]
 */
const zend_function_entry opencv_cascade_classifier_methods[] = {
        PHP_ME(opencv_cascade_classifier, __construct, arginfo_void, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
        PHP_ME(opencv_cascade_classifier, load, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_MALIAS(opencv_cascade_classifier, detectMultiScale ,detect_multi_scale, opencv_cascade_classifier_detect_multi_scale_arginfo, ZEND_ACC_PUBLIC)
        PHP_FE_END
};
/* }}} */

void opencv_cascade_classifier_free_obj(zend_object *object)
{
    opencv_cascade_classifier_object *obj;
    obj = get_cascade_classifier_obj(object);
    delete obj->cascadeClassifier;
    zend_object_std_dtor(object);
}


/**
 * opencv_cascade_classifier_init
 * @param module_number
 */
void opencv_cascade_classifier_init(int module_number){
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce,OPENCV_NS, "CascadeClassifier", opencv_cascade_classifier_methods);
    opencv_cascade_classifier_ce = zend_register_internal_class(&ce);

    opencv_cascade_classifier_ce->create_object = opencv_cascade_classifier_create_handler;
    memcpy(&opencv_cascade_classifier_object_handlers,
           zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    opencv_cascade_classifier_object_handlers.clone_obj = NULL;
//    opencv_cascade_classifier_object_handlers.write_property = opencv_mat_write_property;
    opencv_cascade_classifier_object_handlers.free_obj = opencv_cascade_classifier_free_obj;
    opencv_cascade_classifier_object_handlers.offset = XtOffsetOf(opencv_cascade_classifier_object, std);
}


/**
 * -----------------------------------【CV\FaceDetectorYN】-----------------------------
 *
 * -------------------------------------------------------------------------------------
 */

#ifdef OPENCV_OBJDETECT_FACE_HPP

zend_object_handlers opencv_facedetectoryn_object_handlers;


PHP_METHOD(opencv_facedetectoryn, create)
{
    char *model;
    size_t model_length;
    char *config;
    size_t config_length;
    zval *size_zval;
    double score_threshold = 0.9;
    double nms_threshold = 0.3;
    long top_k = 5000;
    long backend_id = 0;
    long target_id = 0;

    Size size = Size();


    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ssO|ddlll",
                              &model, &model_length,
                              &config, &config_length,
                              &size_zval, opencv_size_ce,
                              &score_threshold, &nms_threshold, &top_k, &backend_id, &target_id) == FAILURE) {
        RETURN_NULL();
    }

    zval instance;
    object_init_ex(&instance, opencv_facedetectoryn_ce);
    opencv_facedetectoryn_object *facedetectoryn_obj = Z_PHP_FACEDETECTORYN_OBJ_P(&instance);

    opencv_size_object *size_object = Z_PHP_SIZE_OBJ_P(size_zval);
    size = *size_object->size;

    facedetectoryn_obj->facedetectoryn = FaceDetectorYN::create(model, config, size, score_threshold, nms_threshold, top_k, backend_id, target_id);

    RETURN_ZVAL(&instance,0,0); //return php FaceDetectorYN object
}

PHP_METHOD(opencv_facedetectoryn, detect)
{
    zval *image_zval;
    Mat faces;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "O",
                              &image_zval, opencv_mat_ce) == FAILURE) {
        RETURN_NULL();
    }

    opencv_mat_object *image_obj = Z_PHP_MAT_OBJ_P(image_zval);

    opencv_facedetectoryn_object *obj = Z_PHP_FACEDETECTORYN_OBJ_P(getThis());
    obj->facedetectoryn->detect(*image_obj->mat, faces);


    zval faces_zval;
    object_init_ex(&faces_zval, opencv_mat_ce);
    opencv_mat_object *faces_obj = Z_PHP_MAT_OBJ_P(&faces_zval);
    faces_obj->mat = new Mat(faces);

    opencv_mat_update_property_by_c_mat(&faces_zval, faces_obj->mat);

    RETURN_ZVAL(&faces_zval,0,0); //return php Mat object
}

PHP_METHOD(opencv_facedetectoryn, setInputSize)
{
    zval *size_zval;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "O",
                              &size_zval, opencv_size_ce) == FAILURE) {
        RETURN_NULL();
    }

    opencv_size_object *size_object = Z_PHP_SIZE_OBJ_P(size_zval);
    Size size = *size_object->size;

    opencv_facedetectoryn_object *obj = Z_PHP_FACEDETECTORYN_OBJ_P(getThis());
    obj->facedetectoryn->setInputSize(size);

    RETURN_NULL();
}

PHP_METHOD(opencv_facedetectoryn, setNMSThreshold)
{
    double nms_threshold;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d",
                              &nms_threshold) == FAILURE) {
        RETURN_NULL();
    }

    opencv_facedetectoryn_object *obj = Z_PHP_FACEDETECTORYN_OBJ_P(getThis());
    obj->facedetectoryn->setNMSThreshold(nms_threshold);

    RETURN_NULL();
}

PHP_METHOD(opencv_facedetectoryn, setScoreThreshold)
{
    double score_threshold;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d",
                              &score_threshold) == FAILURE) {
        RETURN_NULL();
    }

    opencv_facedetectoryn_object *obj = Z_PHP_FACEDETECTORYN_OBJ_P(getThis());
    obj->facedetectoryn->setScoreThreshold(score_threshold);

    RETURN_NULL();
}

PHP_METHOD(opencv_facedetectoryn, setTopK)
{
    long top_k;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l",
                              &top_k) == FAILURE) {
        RETURN_NULL();
    }

    opencv_facedetectoryn_object *obj = Z_PHP_FACEDETECTORYN_OBJ_P(getThis());
    obj->facedetectoryn->setTopK(top_k);

    RETURN_NULL();
}

PHP_METHOD(opencv_facedetectoryn, getInputSize)
{
    zval *size_zval;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "") == FAILURE) {
        RETURN_NULL();
    }

    opencv_facedetectoryn_object *obj = Z_PHP_FACEDETECTORYN_OBJ_P(getThis());
    Size size = obj->facedetectoryn->getInputSize();

    zval instance;
    object_init_ex(&instance, opencv_size_ce);
    opencv_size_object *size_object = Z_PHP_SIZE_OBJ_P(&instance);
    size_object->size = new Size(size);

    opencv_size_update_property_by_c_size(&instance, size_object->size);
    RETURN_ZVAL(&instance,0,0);
}

PHP_METHOD(opencv_facedetectoryn, getNMSThreshold)
{
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "") == FAILURE) {
        RETURN_NULL();
    }

    opencv_facedetectoryn_object *obj = Z_PHP_FACEDETECTORYN_OBJ_P(getThis());
    double nms_threshold = obj->facedetectoryn->getNMSThreshold();

    RETURN_DOUBLE(nms_threshold);
}

PHP_METHOD(opencv_facedetectoryn, getScoreThreshold)
{
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "") == FAILURE) {
        RETURN_NULL();
    }

    opencv_facedetectoryn_object *obj = Z_PHP_FACEDETECTORYN_OBJ_P(getThis());
    double score_threshold = obj->facedetectoryn->getScoreThreshold();

    RETURN_DOUBLE(score_threshold);
}

PHP_METHOD(opencv_facedetectoryn, getTopK)
{
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "") == FAILURE) {
        RETURN_NULL();
    }

    opencv_facedetectoryn_object *obj = Z_PHP_FACEDETECTORYN_OBJ_P(getThis());
    long top_k = obj->facedetectoryn->getTopK();

    RETURN_LONG(top_k);
}

/**
 * opencv_facedetectoryn_methods[]
 */
const zend_function_entry opencv_facedetectoryn_methods[] = {
        PHP_ME(opencv_facedetectoryn, create, arginfo_void, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(opencv_facedetectoryn, detect, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_ME(opencv_facedetectoryn, getInputSize, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_ME(opencv_facedetectoryn, getNMSThreshold, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_ME(opencv_facedetectoryn, getScoreThreshold, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_ME(opencv_facedetectoryn, getTopK, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_ME(opencv_facedetectoryn, setInputSize, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_ME(opencv_facedetectoryn, setNMSThreshold, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_ME(opencv_facedetectoryn, setScoreThreshold, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_ME(opencv_facedetectoryn, setTopK, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_FE_END
};
/* }}} */

zend_object* opencv_facedetectoryn_create_handler(zend_class_entry *type)
{
    size_t size = sizeof(opencv_facedetectoryn_object)+zend_object_properties_size(type);
    opencv_facedetectoryn_object *obj = (opencv_facedetectoryn_object *)ecalloc(1, size);
    memset(obj, 0, size);
    zend_object_std_init(&obj->std, type);
    object_properties_init(&obj->std, type);
    obj->std.ce = type;
    obj->std.handlers = &opencv_facedetectoryn_object_handlers;
    return &obj->std;
}

void opencv_facedetectoryn_free_obj(zend_object *object)
{
    opencv_facedetectoryn_object *obj;
    obj = get_facedetectoryn_obj(object);
    zend_object_std_dtor(object);
}

void opencv_facedetectoryn_init(int module_number)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, OPENCV_NS, "FaceDetectorYN", opencv_facedetectoryn_methods);
    opencv_facedetectoryn_ce = zend_register_internal_class(&ce);

    opencv_facedetectoryn_ce->create_object = opencv_facedetectoryn_create_handler;
    memcpy(&opencv_facedetectoryn_object_handlers,
           zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    opencv_facedetectoryn_object_handlers.clone_obj = NULL;
    opencv_facedetectoryn_object_handlers.free_obj = opencv_facedetectoryn_free_obj;
    opencv_facedetectoryn_object_handlers.offset = XtOffsetOf(opencv_facedetectoryn_object, std);
}

zend_object_handlers opencv_facerecognizersf_object_handlers;


PHP_METHOD(opencv_facerecognizersf, create)
{
    char *model;
    size_t model_length;
    char *config;
    size_t config_length;
    long backend_id = 0;
    long target_id = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss|ll",
                              &model, &model_length,
                              &config, &config_length,
                              &backend_id, &target_id) == FAILURE) {
        RETURN_NULL();
    }

    zval instance;
    object_init_ex(&instance, opencv_facerecognizersf_ce);
    opencv_facerecognizersf_object *facerecognizersf_obj = Z_PHP_FACERECOGNIZERSF_OBJ_P(&instance);

    facerecognizersf_obj->facerecognizersf = FaceRecognizerSF::create(model, config, backend_id, target_id);

    RETURN_ZVAL(&instance,0,0); //return php FaceRecognizerSF object
}

PHP_METHOD(opencv_facerecognizersf, alignCrop)
{
    zval *image_zval, *face_zval;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "OO",
                              &image_zval, opencv_mat_ce,
                              &face_zval, opencv_mat_ce) == FAILURE) {
        RETURN_NULL();
    }

    opencv_mat_object *image_obj = Z_PHP_MAT_OBJ_P(image_zval);
    opencv_mat_object *face_obj = Z_PHP_MAT_OBJ_P(face_zval);

    opencv_facerecognizersf_object *obj = Z_PHP_FACERECOGNIZERSF_OBJ_P(getThis());

    Mat aligned_img;
    obj->facerecognizersf->alignCrop(*image_obj->mat, *face_obj->mat, aligned_img);

    zval instance;
    object_init_ex(&instance, opencv_mat_ce);
    opencv_mat_object *new_obj = Z_PHP_MAT_OBJ_P(&instance);
    new_obj->mat=new Mat(aligned_img);
    opencv_mat_update_property_by_c_mat(&instance, new_obj->mat);
    RETURN_ZVAL(&instance,0,0);
}

PHP_METHOD(opencv_facerecognizersf, feature)
{
    zval *image_zval;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "O",
                              &image_zval, opencv_mat_ce) == FAILURE) {
        RETURN_NULL();
    }

    opencv_mat_object *image_obj = Z_PHP_MAT_OBJ_P(image_zval);

    opencv_facerecognizersf_object *obj = Z_PHP_FACERECOGNIZERSF_OBJ_P(getThis());

    Mat face_feature;
    obj->facerecognizersf->feature(*image_obj->mat, face_feature);

    zval instance;
    object_init_ex(&instance, opencv_mat_ce);
    opencv_mat_object *new_obj = Z_PHP_MAT_OBJ_P(&instance);
    new_obj->mat=new Mat(face_feature);
    opencv_mat_update_property_by_c_mat(&instance, new_obj->mat);
    RETURN_ZVAL(&instance,0,0);
}

PHP_METHOD(opencv_facerecognizersf, match)
{
    zval *face_feature1_zval, *face_feature2_zval;
    long dis_type = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "OO|l",
                              &face_feature1_zval, opencv_mat_ce,
                              &face_feature2_zval, opencv_mat_ce,
                              &dis_type) == FAILURE) {
        RETURN_NULL();
    }

    opencv_mat_object *face_feature1_obj = Z_PHP_MAT_OBJ_P(face_feature1_zval);
    opencv_mat_object *face_feature2_obj = Z_PHP_MAT_OBJ_P(face_feature2_zval);

    opencv_facerecognizersf_object *obj = Z_PHP_FACERECOGNIZERSF_OBJ_P(getThis());

    double score = obj->facerecognizersf->match(*face_feature1_obj->mat, *face_feature2_obj->mat, dis_type);

    RETURN_DOUBLE(score);
}

/**
 * opencv_facerecognizersf_methods[]
 */
const zend_function_entry opencv_facerecognizersf_methods[] = {
        PHP_ME(opencv_facerecognizersf, create, arginfo_void, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
        PHP_ME(opencv_facerecognizersf, alignCrop, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_ME(opencv_facerecognizersf, feature, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_ME(opencv_facerecognizersf, match, arginfo_void, ZEND_ACC_PUBLIC)
        PHP_FE_END
};
/* }}} */

zend_object* opencv_facerecognizersf_create_handler(zend_class_entry *type)
{
    size_t size = sizeof(opencv_facerecognizersf_object)+zend_object_properties_size(type);
    opencv_facerecognizersf_object *obj = (opencv_facerecognizersf_object *)ecalloc(1, size);
    memset(obj, 0, size);
    zend_object_std_init(&obj->std, type);
    object_properties_init(&obj->std, type);
    obj->std.ce = type;
    obj->std.handlers = &opencv_facerecognizersf_object_handlers;
    return &obj->std;
}

void opencv_facerecognizersf_free_obj(zend_object *object)
{
    opencv_facerecognizersf_object *obj;
    obj = get_facerecognizersf_obj(object);
    zend_object_std_dtor(object);
}

void opencv_facerecognizersf_init(int module_number)
{
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce, OPENCV_NS, "FaceRecognizerSF", opencv_facerecognizersf_methods);
    opencv_facerecognizersf_ce = zend_register_internal_class(&ce);

    opencv_facerecognizersf_ce->create_object = opencv_facerecognizersf_create_handler;
    memcpy(&opencv_facerecognizersf_object_handlers,
           zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    opencv_facerecognizersf_object_handlers.clone_obj = NULL;
    opencv_facerecognizersf_object_handlers.free_obj = opencv_facerecognizersf_free_obj;
    opencv_facerecognizersf_object_handlers.offset = XtOffsetOf(opencv_facerecognizersf_object, std);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "FaceRecognizerSF_DISTYPE_FR_COSINE", FaceRecognizerSF::DisType::FR_COSINE, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "FaceRecognizerSF_DISTYPE_FR_NORM_L2", FaceRecognizerSF::DisType::FR_NORM_L2, CONST_CS | CONST_PERSISTENT);
}

#endif

/**
 * -------------------------------
 */


void opencv_objdetect_constants_init(int module_number)
{

//    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "CV_HAAR_DO_CANNY_PRUNING", CV_HAAR_DO_CANNY_PRUNING, CONST_CS | CONST_PERSISTENT);
//    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "CV_HAAR_SCALE_IMAGE", CV_HAAR_SCALE_IMAGE, CONST_CS | CONST_PERSISTENT);
//    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "CV_HAAR_FIND_BIGGEST_OBJECT", CV_HAAR_FIND_BIGGEST_OBJECT, CONST_CS | CONST_PERSISTENT);
//    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "CV_HAAR_DO_ROUGH_SEARCH", CV_HAAR_DO_ROUGH_SEARCH, CONST_CS | CONST_PERSISTENT);

    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "CASCADE_DO_CANNY_PRUNING", CASCADE_DO_CANNY_PRUNING, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "CASCADE_SCALE_IMAGE", CASCADE_SCALE_IMAGE, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "CASCADE_FIND_BIGGEST_OBJECT", CASCADE_FIND_BIGGEST_OBJECT, CONST_CS | CONST_PERSISTENT);
    REGISTER_NS_LONG_CONSTANT(OPENCV_NS, "CASCADE_DO_ROUGH_SEARCH", CASCADE_DO_ROUGH_SEARCH, CONST_CS | CONST_PERSISTENT);
}

void opencv_objdetect_init(int module_number){
    opencv_cascade_classifier_init(module_number);
    opencv_objdetect_constants_init(module_number);

#ifdef OPENCV_OBJDETECT_FACE_HPP
    opencv_facedetectoryn_init(module_number);
    opencv_facerecognizersf_init(module_number);
#endif
}
