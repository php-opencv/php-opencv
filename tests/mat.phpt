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
$ones = $ones->plus(3);
$ones->print(Formatter::FMT_PYTHON);
$ones = $ones->divide(1/9);
$ones->print(Formatter::FMT_PYTHON);
$mat2 = Mat::createWithDims(4, [3,5,2,4], CV_8UC3);
var_dump($mat2);
?>
--EXPECT--
object(CV\Mat)#2 (4) {
  ["type":"CV\Mat":private]=>
  int(16)
  ["rows"]=>
  int(5)
  ["cols"]=>
  int(5)
  ["dims"]=>
  int(2)
}
[[[255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100]],
 [[255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100]],
 [[255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100]],
 [[255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100]],
 [[255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100], [255, 100, 100]]]
object(CV\Mat)#3 (4) {
  ["type":"CV\Mat":private]=>
  int(0)
  ["rows"]=>
  int(10)
  ["cols"]=>
  int(10)
  ["dims"]=>
  int(2)
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
object(CV\Mat)#4 (4) {
  ["type":"CV\Mat":private]=>
  int(0)
  ["rows"]=>
  int(10)
  ["cols"]=>
  int(10)
  ["dims"]=>
  int(2)
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
 object(CV\Mat)#5 (5) {
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