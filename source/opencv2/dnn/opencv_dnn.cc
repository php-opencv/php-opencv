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
#include "../../../php_opencv.h"
#include "opencv_dnn.h"

#ifdef HAVE_OPENCV_DNN

#include "../core/opencv_type.h"
#include "../core/opencv_mat.h"
#include "../../../opencv_exception.h"

using namespace dnn;
using namespace std;

#define Z_PHP_DNN_NET_OBJ_P(zv)  get_dnn_net_obj(Z_OBJ_P(zv))

typedef struct _opencv_dnn_net_object{
    Net DNNNet;
    zend_object std;
}opencv_dnn_net_object;


/**
 * @param obj
 * @return
 */
static inline opencv_dnn_net_object* get_dnn_net_obj(zend_object *obj) {
    return (opencv_dnn_net_object*)((char*)(obj) - XtOffsetOf(opencv_dnn_net_object, std));
}

zend_object_handlers opencv_dnn_net_object_handlers;

zend_class_entry *opencv_dnn_net_ce;

zend_class_entry *opencv_base_dnn_net_ce;

PHP_FUNCTION(opencv_dnn_blob_from_image)
{
    zval *image_zval, *size_zval, *mean_zval;
    double scalefactor = 1.;
    bool swapRB = false, crop = false;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "OdOO|bb",
        &image_zval, opencv_mat_ce,
        &scalefactor,
        &size_zval, opencv_size_ce,
        &mean_zval, opencv_scalar_ce,
        &swapRB,
        &crop
    ) == FAILURE) {
        RETURN_NULL();
    }

    opencv_mat_object *image_object = Z_PHP_MAT_OBJ_P(image_zval);
    opencv_size_object *size_object = Z_PHP_SIZE_OBJ_P(size_zval);
    opencv_scalar_object *mean_object = Z_PHP_SCALAR_OBJ_P(mean_zval);

    Mat im = blobFromImage(*image_object->mat, scalefactor, *size_object->size, *mean_object->scalar, swapRB, crop);

    zval instance;
    object_init_ex(&instance, opencv_mat_ce);
    opencv_mat_object *new_obj = Z_PHP_MAT_OBJ_P(&instance);
    new_obj->mat=new Mat(im);
    opencv_mat_update_property_by_c_mat(&instance, new_obj->mat);
    RETURN_ZVAL(&instance,0,0); //return php Mat object
}

PHP_FUNCTION(opencv_dnn_blob_from_images)
{
    zval *images_zval, *size_zval, *mean_zval;
    double scalefactor = 1.;
    bool swapRB = false, crop = false;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "adOO|bb",
        &images_zval,
        &scalefactor,
        &size_zval, opencv_size_ce,
        &mean_zval, opencv_scalar_ce,
        &swapRB,
        &crop
    ) == FAILURE) {
        RETURN_NULL();
    }

    opencv_size_object *size_object = Z_PHP_SIZE_OBJ_P(size_zval);
    opencv_scalar_object *mean_object = Z_PHP_SCALAR_OBJ_P(mean_zval);

    HashTable *images_ht = Z_ARRVAL_P(images_zval);
    std::vector<cv::Mat> images_vec;

    zval *image_zval;
    ZEND_HASH_FOREACH_VAL(images_ht, image_zval) {
        if(Z_TYPE_P(image_zval) == IS_OBJECT && Z_OBJCE_P(image_zval)==opencv_mat_ce)
            images_vec.push_back(*Z_PHP_MAT_OBJ_P(image_zval)->mat);
    }
    ZEND_HASH_FOREACH_END();


    Mat im = blobFromImages(images_vec, scalefactor, *size_object->size, *mean_object->scalar, swapRB, crop);

    zval instance;
    object_init_ex(&instance, opencv_mat_ce);
    opencv_mat_object *new_obj = Z_PHP_MAT_OBJ_P(&instance);
    new_obj->mat=new Mat(im);
    opencv_mat_update_property_by_c_mat(&instance, new_obj->mat);
    RETURN_ZVAL(&instance,0,0); //return php Mat object
}

PHP_FUNCTION(opencv_dnn_read_net_from_torch)
{
    char *filename;
    size_t filename_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &filename, &filename_len) == FAILURE) {
        RETURN_NULL();
    }

    zval instance;
    object_init_ex(&instance, opencv_dnn_net_ce);
    opencv_dnn_net_object *obj = Z_PHP_DNN_NET_OBJ_P(&instance);
    obj->DNNNet = readNetFromTorch(filename, true);

    RETURN_ZVAL(&instance,0,0);
}

PHP_FUNCTION(opencv_dnn_read_net_from_caffe)
{
    char *prototxt, *caffeModel;
    size_t prototxt_len, caffeModel_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss", &prototxt, &prototxt_len, &caffeModel, &caffeModel_len) == FAILURE) {
        RETURN_NULL();
    }

    ifstream protoFile;
    protoFile.open(prototxt);//open the input file
    stringstream protoStream;
    protoStream << protoFile.rdbuf();//read the file
    string protoData = protoStream.str();

    ifstream modelFile;
    modelFile.open(caffeModel, ios::binary);//open the input file
    stringstream modelStream;
    modelStream << modelFile.rdbuf();//read the file
    string modelData = modelStream.str();

    zval instance;
    object_init_ex(&instance, opencv_dnn_net_ce);
    opencv_dnn_net_object *obj = Z_PHP_DNN_NET_OBJ_P(&instance);

    //obj->DNNNet = readNetFromCaffe(protoData.c_str(), protoData.size());
    obj->DNNNet = readNetFromCaffe(protoData.c_str(), protoData.size(), modelData.c_str(), modelData.size());

    RETURN_ZVAL(&instance,0,0);
}

PHP_FUNCTION(opencv_dnn_read_net_from_tensorflow)
{
    char *model, *config;
    size_t model_len, config_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss", &model, &model_len, &config, &config_len) == FAILURE) {
        RETURN_NULL();
    }

    ifstream modelFile;
    modelFile.open(model);//open the input file
    stringstream modelStream;
    modelStream << modelFile.rdbuf();//read the file
    string modelData = modelStream.str();

    ifstream configFile;
    configFile.open(config, ios::binary);//open the input file
    stringstream configStream;
    configStream << configFile.rdbuf();//read the file
    string configData = configStream.str();

    zval instance;
    object_init_ex(&instance, opencv_dnn_net_ce);
    opencv_dnn_net_object *obj = Z_PHP_DNN_NET_OBJ_P(&instance);

    //obj->DNNNet = readNetFromTensorflow(modelData.c_str(), modelData.size());
    obj->DNNNet = readNetFromTensorflow(modelData.c_str(), modelData.size(), configData.c_str(), configData.size());

    RETURN_ZVAL(&instance,0,0);
}

PHP_FUNCTION(opencv_dnn_read_net_from_onnx)
{
    char *filename;
    size_t filename_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &filename, &filename_len) == FAILURE) {
        RETURN_NULL();
    }

    zval instance;
    object_init_ex(&instance, opencv_dnn_net_ce);
    opencv_dnn_net_object *obj = Z_PHP_DNN_NET_OBJ_P(&instance);
    obj->DNNNet = readNetFromONNX(filename);

    RETURN_ZVAL(&instance,0,0);
}

PHP_FUNCTION(opencv_dnn_read_net_from_model_optimizer)
{
    char *xml, *bin;
    size_t xml_len, bin_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss", &xml, &xml_len, &bin, &bin_len) == FAILURE) {
        RETURN_NULL();
    }

    ifstream xmlFile;
    xmlFile.open(xml);//open the input file
    stringstream xmlStream;
    xmlStream << xmlFile.rdbuf();//read the file
    string xmlData = xmlStream.str();

    ifstream binFile;
    binFile.open(bin, ios::binary);//open the input file
    stringstream binStream;
    binStream << binFile.rdbuf();//read the file
    string binData = binStream.str();

    zval instance;
    object_init_ex(&instance, opencv_dnn_net_ce);
    opencv_dnn_net_object *obj = Z_PHP_DNN_NET_OBJ_P(&instance);

    obj->DNNNet = readNetFromModelOptimizer(xmlData, binData);

    RETURN_ZVAL(&instance,0,0);
}

PHP_FUNCTION(opencv_dnn_read_net_from_darknet)
{
    char *cfg, *bin;
    size_t cfg_len, bin_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss", &cfg, &cfg_len, &bin, &bin_len) == FAILURE) {
        RETURN_NULL();
    }

    ifstream cfgFile;
    cfgFile.open(cfg);//open the input file
    stringstream cfgStream;
    cfgStream << cfgFile.rdbuf();//read the file
    string cfgData = cfgStream.str();

    ifstream binFile;
    binFile.open(bin, ios::binary);//open the input file
    stringstream binStream;
    binStream << binFile.rdbuf();//read the file
    string binData = binStream.str();

    zval instance;
    object_init_ex(&instance, opencv_dnn_net_ce);
    opencv_dnn_net_object *obj = Z_PHP_DNN_NET_OBJ_P(&instance);

    //obj->DNNNet = readNetFromDarknet(cfgData.c_str(), cfgData.size());
    obj->DNNNet = readNetFromDarknet(cfgData.c_str(), cfgData.size(), binData.c_str(), binData.size());

    RETURN_ZVAL(&instance,0,0);
}

PHP_METHOD(opencv_dnn_net, setInput)
{
    zval *image_zval;
    char *name = (char*) "";
    size_t name_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "O|s", &image_zval, opencv_mat_ce, &name, &name_len) == FAILURE) {
        RETURN_NULL();
    }

    opencv_mat_object *image_object = Z_PHP_MAT_OBJ_P(image_zval);

    opencv_dnn_net_object *obj = Z_PHP_DNN_NET_OBJ_P(getThis());
    obj->DNNNet.setInput(*(image_object->mat), name);

    RETURN_NULL();
}

PHP_METHOD(opencv_dnn_net, forward)
{
    zval *image_zval;
    char *name = (char*) "";
    size_t name_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "|s", &name, &name_len) == FAILURE) {
        RETURN_NULL();
    }

    opencv_dnn_net_object *obj = Z_PHP_DNN_NET_OBJ_P(getThis());
    Mat image = obj->DNNNet.forward(name);

    zval instance;
    object_init_ex(&instance, opencv_mat_ce);
    opencv_mat_object *new_obj = Z_PHP_MAT_OBJ_P(&instance);
    new_obj->mat=new Mat(image);
    opencv_mat_update_property_by_c_mat(&instance, new_obj->mat);
    RETURN_ZVAL(&instance,0,0); //return php Mat object
}

PHP_METHOD(opencv_dnn_net, forwardMulti)
{
    zval *layers_zval;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "a", &layers_zval) == FAILURE) {
        RETURN_NULL();
    }

    unsigned long layers_count = zend_hash_num_elements(Z_ARRVAL_P(layers_zval));

    if (layers_count == 0) {
        opencv_throw_exception("array lenght must be >=1");
        RETURN_NULL();
    }

    std::vector<String> layers;
    std::vector<std::vector<Mat>> outputBlobs;

    zval *array_val_zval;
    zend_ulong _h;

    ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(layers_zval),_h,array_val_zval){
                again:
                if(Z_TYPE_P(array_val_zval) == IS_STRING){
                    layers.push_back((String)ZSTR_VAL(zval_get_string(array_val_zval)));
                }else if(Z_TYPE_P(array_val_zval) == IS_REFERENCE){
                    array_val_zval = Z_REFVAL_P(array_val_zval);
                    goto again;
                } else {
                    opencv_throw_exception("array value just string.");
                    RETURN_NULL();
                }
    }ZEND_HASH_FOREACH_END();

    opencv_dnn_net_object *obj = Z_PHP_DNN_NET_OBJ_P(getThis());

    obj->DNNNet.forward(outputBlobs, layers);

    zval arr_zval;
    array_init_size(&arr_zval, outputBlobs.size());

    for(std::vector<int>::size_type i = 0; i != outputBlobs.size(); i++) {
        Mat output = outputBlobs[i][0];

        zval instance;
        object_init_ex(&instance, opencv_mat_ce);
        opencv_mat_object *new_obj = Z_PHP_MAT_OBJ_P(&instance);
        new_obj->mat=new Mat(output);
        opencv_mat_update_property_by_c_mat(&instance, new_obj->mat);

        add_index_zval(&arr_zval, i, &instance);
        //add_assoc_zval(&arr_zval, layers[i].c_str(), &instance);
    }

    RETURN_ZVAL(&arr_zval,0,0);
}

PHP_METHOD(opencv_dnn_net, getLayerNames)
{
    std::vector<String> layers;
    zval arr_zval;

    opencv_dnn_net_object *obj = Z_PHP_DNN_NET_OBJ_P(getThis());
    layers = obj->DNNNet.getLayerNames();

    array_init_size(&arr_zval, layers.size());


    for(std::vector<int>::size_type i = 0; i != layers.size(); i++) {
        add_index_string(&arr_zval, i, layers[i].c_str());
    }

    RETURN_ZVAL(&arr_zval,0,0);
}

PHP_METHOD(opencv_dnn_net, getLayersCount)
{
    char *type;
    size_t type_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &type, &type_len) == FAILURE) {
        RETURN_NULL();
    }

    opencv_dnn_net_object *obj = Z_PHP_DNN_NET_OBJ_P(getThis());
    long count = obj->DNNNet.getLayersCount(type);
    
    RETURN_LONG(count);
}


/**
 * opencv_dnn_net_methods[]
 */
const zend_function_entry opencv_dnn_net_methods[] = {
        PHP_ME(opencv_dnn_net, setInput, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(opencv_dnn_net, forward, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(opencv_dnn_net, forwardMulti, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(opencv_dnn_net, getLayerNames, NULL, ZEND_ACC_PUBLIC)
        PHP_ME(opencv_dnn_net, getLayersCount, NULL, ZEND_ACC_PUBLIC)
        PHP_FE_END
};
/* }}} */

/**
 * @param type
 * @return
 */
zend_object* opencv_dnn_net_handler(zend_class_entry *type)
{
    size_t size = sizeof(opencv_dnn_net_object)+zend_object_properties_size(type);
    opencv_dnn_net_object *obj = (opencv_dnn_net_object *)ecalloc(1,size);
    memset(obj, 0, size);
    zend_object_std_init(&obj->std, type);
    object_properties_init(&obj->std, type);
    obj->std.ce = type;
    obj->std.handlers = &opencv_dnn_net_object_handlers;
    return &obj->std;
}

void opencv_dnn_init(int module_number){
    zend_class_entry ce;
    INIT_NS_CLASS_ENTRY(ce,OPENCV_DNN_NS, "Net", opencv_dnn_net_methods);
    opencv_dnn_net_ce = zend_register_internal_class_ex(&ce, opencv_dnn_net_ce);

    opencv_dnn_net_ce->create_object = opencv_dnn_net_handler;
    memcpy(&opencv_dnn_net_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    opencv_dnn_net_object_handlers.offset = XtOffsetOf(opencv_dnn_net_object, std);
}

#else

void opencv_dnn_init(int module_number){

}

#endif
