#include <filesystem>
#include <iostream>
#include <libutils/rasserts.h>

#include "blur.h" // TODO реализуйте функцию блюра с произвольной силой размытия в файле blur.cpp

void testSomeBlur(const std::string &name) {
    cv::Mat img = cv::imread("lesson05/data/" + name + ".jpg");
    std::string resultsDir = "lesson06/resultsData/";
    if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
        std::filesystem::create_directory(resultsDir); // то создаем ее
    }
    cv::Mat res = blur(img, 0.8);
    cv::imwrite(resultsDir + name + "_grey.jpg", res);
    // TODO выберите любую картинку и любую силу сглаживания - проверьте что результат - чуть размытая картинка
    // Входные картинки для тестирования возьмите из предыдущего урока (т.е. по пути lesson05/data/*).
    // Результирующие картинки сохарняйте в эту папку (т.е. по пути lesson06/resultsData/*).
}

void testManySigmas(const std::string &name) {
    // TODO возьмите ту же самую картинку но теперь в цикле проведите сглаживание для нескольких разных сигм
    // при этом результирующую картинку сохраняйте с указанием какая сигма использовалась:
    // для того чтобы в название файла добавить значение этой переменной -
    // воспользуйтесь функцией преобразующей числа в строчки - std::to_string(sigma)
    cv::Mat img = cv::imread("lesson05/data/" + name + ".jpg");
    std::string resultsDir = "lesson06/resultsData/";
    if (!std::filesystem::exists(resultsDir)) { // если папка еще не создана
        std::filesystem::create_directory(resultsDir); // то создаем ее
    }
    cv::Mat res;
    for (int i = 0; i < 10; ++i) {
        res = blur(img.clone(), (float)i/10);
        cv::imwrite(resultsDir + name + "_" + std::to_string(i) + ".jpg", res);
    }
//    res = blur(img, 0.8);
//    cv::imwrite(resultsDir + name + "_grey.jpg", res);
}

int main() {
    try {
//        testSomeBlur("line21_water_horizont");
        testManySigmas("line21_water_horizont");

        return 0;
    } catch (const std::exception &e) {
        std::cout << "Exception! " << e.what() << std::endl;
        return 1;
    }
}

