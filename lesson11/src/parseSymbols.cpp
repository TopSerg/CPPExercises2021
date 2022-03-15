#include <opencv2/imgproc.hpp>
#include <iostream>
#include "parseSymbols.h"

cv::Scalar randColor() {
    return cv::Scalar(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128); // можно было бы брать по модулю 255, но так цвета будут светлее и контрастнее
}

cv::Mat drawContours(int rows, int cols, std::vector<std::vector<cv::Point>> contoursPoints) {

    // TODO 06 реализуйте функцию которая покажет вам как выглядят найденные контура:

    // создаем пустую черную картинку
    cv::Mat blackImage(rows, cols, CV_8UC3, cv::Scalar(0, 0, 0));
    // теперь мы на ней хотим нарисовать контуры
    cv::Mat imageWithContoursPoints = blackImage.clone();
    for (int contourI = 0; contourI < contoursPoints.size(); ++contourI) {
        // сейчас мы смотрим на контур номер contourI

        cv::Scalar contourColor = randColor(); // выберем для него случайный цвет
        std::vector<cv::Point> points = contoursPoints[contourI]; // TODO 06 вытащите вектор из точек-пикселей соответствующих текущему контуру который мы хотим нарисовать
        for (int i = 0; i < points.size(); i++) { // TODO 06 пробегите по всем точкам-пикселям этого контура
            cv::Point point = points[i]; // TODO 06 и взяв очередную точку-пиксель - нарисуйте выбранный цвет в этом пикселе картинки:
            imageWithContoursPoints.at<cv::Vec3b>(point.y, point.x) = cv::Vec3b(contourColor[0], contourColor[1], contourColor[2]);
        }

    }

    return imageWithContoursPoints;
}


std::vector<cv::Mat> splitSymbols(cv::Mat img)
{
    cv::Mat original = img;
    std::vector<cv::Mat> symbols;
//     TODO 101: чтобы извлечь кусок картинки (для каждого прямоугольника cv::Rect вокруг символа) - загуглите "opencv how to extract subimage"
    cv::cvtColor(img.clone(), img, cv::COLOR_BGR2GRAY);

    std::cout << "Processing 01 ..." << std::endl;
    // TODO 01 выполните бинарный трешолдинг картинки, прочитайте документацию по функции cv::threshold и выберите значения аргументов
    cv::Mat binary;
    cv::threshold(img, binary, 127, 255, cv::THRESH_BINARY);

    std::cout << "Processing 02 ..." << std::endl;
    // TODO 02 выполните адаптивный бинарный трешолдинг картинки, прочитайте документацию по cv::adaptiveThreshold
    cv::adaptiveThreshold(img, binary, 255, cv::ADAPTIVE_THRESH_MEAN_C , cv::THRESH_BINARY_INV, 5, 15);

    std::cout << "Processing 03e ..." << std::endl;
    // TODO 03 чтобы буквы не разваливались на кусочки - морфологическое расширение (эрозия)
    cv::Mat binary_eroded;
    cv::erode(binary, binary_eroded, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4)));

    std::cout << "Processing 03d ..." << std::endl;
    // TODO 03 заодно давайте посмотрим что делает морфологическое сужение (диляция)
    cv::Mat binary_dilated;
    cv::dilate(binary, binary_dilated, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4, 4)));

    std::cout << "Processing 04 ..." << std::endl;
    // TODO 04 дальше работаем с картинкой после морфологичесокго рашсирения или морфологического сжатия - на ваш выбор, подумайте и посмотрите на картинки
    binary = binary_dilated;

    // TODO 05
    std::vector<std::vector<cv::Point>> contoursPoints; // по сути это вектор, где каждый элемент - это одна связная компонента-контур,
    // а что такое компонента-контур? это вектор из точек (из пикселей)
    cv::findContours(binary, contoursPoints, cv::RETR_LIST, cv::RETR_LIST); // TODO подумайте, какие нужны два последних параметра? прочитайте документацию, после реализации отрисовки контура - поиграйте с этими параметрами чтобы посмотреть как меняется результат
    std::cout << "Contours: " << contoursPoints.size() << std::endl;
    cv::Mat imageWithContoursPoints = drawContours(img.rows, img.cols, contoursPoints); // TODO 06 реализуйте функцию которая покажет вам как выглядят найденные контура
    std::cout << "Saving" << std::endl;

    std::vector<std::vector<cv::Point>> contoursPoints2;
    cv::findContours(binary, contoursPoints2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    // TODO:
    // Обратите внимание на кромку картинки - она всё победила, т.к. черное - это ноль - пустота, а белое - это 255 - сам объект интереса
    // как перевернуть ситуацию чтобы периметр не был засчитан как контур?
    // когда подумаете - замрите! и прежде чем кодить:
    // Посмотрите в документации у функций cv::threshold и cv::adaptiveThreshold
    // про некоего cv::THRESH_BINARY_INV, чем он отличается от cv::THRESH_BINARY?
    // Посмотрите как изменились все картинки.
    std::cout << "Contours2: " << contoursPoints2.size() << std::endl;
    cv::Mat imageWithContoursPoints2 = drawContours(img.rows, img.cols, contoursPoints2);
    cv::threshold(imageWithContoursPoints2.clone(), imageWithContoursPoints2, 127, 255, cv::THRESH_BINARY_INV);
    std::cout << "Saving" << std::endl;
//    for (int i = 0; i < imageWithContoursPoints2.cols; ++i) {
//        for (int j = 0; j < imageWithContoursPoints2.rows; ++j) {
//            if(imageWithContoursPoints2.at<cv::Vec3b>(j,i)[0] == 0){
//                imageWithContoursPoints.at<cv::Vec3b>(j,i) = imageWithContoursPoints2.at<cv::Vec3b>(j,i);
//            }
//        }
//    }
//    std::cout << "Saving" << std::endl;
//    cv::imwrite(out_path + "/071_contours_points.jpg", imageWithContoursPoints);

    // TODO 06 наконец давайте посмотрим какие буковки нашлись - обрамим их прямоугольниками
    cv::Mat imgWithBoxes = original.clone();
    for (int contourI = 0; contourI < contoursPoints2.size(); ++contourI) {
        std::vector<cv::Point> points = contoursPoints2[contourI]; // берем очередной контур
        cv::Rect box = cv::boundingRect(points); // строим прямоугольник по всем пикселям контура (bounding box = бокс ограничивающий объект)
        cv::Scalar blackColor(0, 0, 0);
        // TODO прочитайте документацию cv::rectangle чтобы нарисовать прямоугольник box с толщиной 2 и черным цветом (обратите внимание какие есть поля у box)
        cv::rectangle(imgWithBoxes, box, blackColor, 1, cv::LINE_8, 0);
        cv::Mat rez = imgWithBoxes(box);
        symbols.push_back(rez);
    }
    return symbols;
}
