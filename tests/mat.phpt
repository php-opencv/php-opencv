--TEST--
Mat __construct
--SKIPIF--
<?php if (!extension_loaded("opencv")) print "skip"; ?>
--FILE--
<?php
use CV\{Mat,Formatter};
use CV\Scalar;
use const CV\{CV_8UC1, CV_8UC3};
$scalar = new Scalar(255,100,100);
$mat = new Mat(5,5,CV_8UC3,$scalar);
//$mat->rows=10;
var_dump($mat);

$mat->print(Formatter::FMT_PYTHON);
$zeros = Mat::zeros(10,10,CV_8UC1);
var_dump($zeros);

$zeros->print(Formatter::FMT_PYTHON);
$ones = Mat::ones(10,10,CV_8UC1);
var_dump($ones);
$ones->print(Formatter::FMT_PYTHON);

$threes = $ones->plus(3);
$threes->print(Formatter::FMT_PYTHON);

$m27 = $threes->divide(1/9);
$m27->print(Formatter::FMT_PYTHON);

$mat2 = Mat::createWithDims(4, [3,5,2,4], CV_8UC3);
var_dump($mat2);

$mat_data = Mat::ones(2,2,CV_8UC1);
var_dump($mat_data->data());

$mat_data2 = Mat::ones(2,2,CV_8UC1);
$mat_data2->setData($mat_data->data());
var_dump($mat_data2->data());

$mat_data_reuse = new Mat(10,10,CV_8UC1);
$mat_data_reuse->useDataFrom($ones,0);
$mat_data_reuse->print(Formatter::FMT_PYTHON);

$mat_data3 = Mat::ones(2,2,CV_8UC1);
$mat_data3->setData([1,2,3,4]);
var_dump($mat_data3->dataAt(3));

$mat_data4 = Mat::ones(4,2,CV_8UC1);
var_dump($mat_data4->total());

# Issue #55
(new Scalar(78.4263377603, 87.7689143744, 114.895847746))->print();
?>
--EXPECT--
object(CV\Mat)#2 (5) {
  ["type":"CV\Mat":private]=>
  int(16)
  ["rows"]=>
  int(5)
  ["cols"]=>
  int(5)
  ["dims"]=>
  int(2)
  ["shape"]=>
  NULL
}
[[[255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100]],
 [[255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100]],
 [[255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100]],
 [[255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100]],
 [[255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100]]]
object(CV\Mat)#3 (5) {
  ["type":"CV\Mat":private]=>
  int(0)
  ["rows"]=>
  int(10)
  ["cols"]=>
  int(10)
  ["dims"]=>
  int(2)
  ["shape"]=>
  NULL
}
[[  0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
 [  0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
 [  0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
 [  0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
 [  0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
 [  0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
 [  0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
 [  0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
 [  0,   0,   0,   0,   0,   0,   0,   0,   0,   0],
 [  0,   0,   0,   0,   0,   0,   0,   0,   0,   0]]
object(CV\Mat)#4 (5) {
  ["type":"CV\Mat":private]=>
  int(0)
  ["rows"]=>
  int(10)
  ["cols"]=>
  int(10)
  ["dims"]=>
  int(2)
  ["shape"]=>
  NULL
}
[[  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1]]
[[  3,   3,   3,   3,   3,   3,   3,   3,   3,   3],
 [  3,   3,   3,   3,   3,   3,   3,   3,   3,   3],
 [  3,   3,   3,   3,   3,   3,   3,   3,   3,   3],
 [  3,   3,   3,   3,   3,   3,   3,   3,   3,   3],
 [  3,   3,   3,   3,   3,   3,   3,   3,   3,   3],
 [  3,   3,   3,   3,   3,   3,   3,   3,   3,   3],
 [  3,   3,   3,   3,   3,   3,   3,   3,   3,   3],
 [  3,   3,   3,   3,   3,   3,   3,   3,   3,   3],
 [  3,   3,   3,   3,   3,   3,   3,   3,   3,   3],
 [  3,   3,   3,   3,   3,   3,   3,   3,   3,   3]]
[[ 27,  27,  27,  27,  27,  27,  27,  27,  27,  27],
 [ 27,  27,  27,  27,  27,  27,  27,  27,  27,  27],
 [ 27,  27,  27,  27,  27,  27,  27,  27,  27,  27],
 [ 27,  27,  27,  27,  27,  27,  27,  27,  27,  27],
 [ 27,  27,  27,  27,  27,  27,  27,  27,  27,  27],
 [ 27,  27,  27,  27,  27,  27,  27,  27,  27,  27],
 [ 27,  27,  27,  27,  27,  27,  27,  27,  27,  27],
 [ 27,  27,  27,  27,  27,  27,  27,  27,  27,  27],
 [ 27,  27,  27,  27,  27,  27,  27,  27,  27,  27],
 [ 27,  27,  27,  27,  27,  27,  27,  27,  27,  27]]
object(CV\Mat)#7 (5) {
  ["type":"CV\Mat":private]=>
  int(16)
  ["rows"]=>
  int(-1)
  ["cols"]=>
  int(-1)
  ["dims"]=>
  int(4)
  ["shape"]=>
  array(4) {
    [0]=>
    int(3)
    [1]=>
    int(5)
    [2]=>
    int(2)
    [3]=>
    int(4)
  }
}
array(4) {
  [0]=>
  int(1)
  [1]=>
  int(1)
  [2]=>
  int(1)
  [3]=>
  int(1)
}
array(4) {
  [0]=>
  int(1)
  [1]=>
  int(1)
  [2]=>
  int(1)
  [3]=>
  int(1)
}
[[  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1],
 [  1,   1,   1,   1,   1,   1,   1,   1,   1,   1]]
int(4)
int(8)
[78.4263, 87.7689, 114.896, 0]

