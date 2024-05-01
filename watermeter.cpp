#include <cmath>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <opencv4/opencv2/core/mat.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>

float distance(cv::Point2f point1, cv::Point2f point2)
{
    return std::sqrt((cv::abs(point1.x - point2.x) * cv::abs(point1.x - point2.x) + cv::abs(point1.y - point2.y) * cv::abs(point1.y - point2.y)));
}

cv::Mat pre_process(cv::Mat img, cv::Size size_of_resize)
{
    cv::Mat ori_img;
    cv::resize(img, ori_img, size_of_resize);
    cv::resize(img, img, size_of_resize);
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
    // cv::blur(img, img, cv::Size(5, 5));
    // cv::adaptiveThreshold(img, img, 255,
    //     cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY,
    //     11, 10);
    cv::medianBlur(img, img, 5);
    // cv::Canny(img, img, 128, 200);

    return img;
}

cv::Point2f point_meter_process(cv::Mat point_meter)
{
    cv::Mat origin_bgr_img = point_meter;
    // cv::resize(point_meter, origin_bgr_img, cv::Size(200, 200));
    cv::Mat hsv;
    cv::cvtColor(origin_bgr_img, hsv, cv::COLOR_BGR2HSV);
    cv::Scalar lower_red_1 = cv::Scalar(0, 50, 50);
    cv::Scalar upper_red_1 = cv::Scalar(10, 255, 255);
    cv::Scalar lower_red_2 = cv::Scalar(156, 50, 50);
    cv::Scalar upper_red_2 = cv::Scalar(180, 255, 255);

    cv::Mat red_mask, red_mask_1, red_mask_2;

    cv::inRange(hsv, lower_red_1, upper_red_1, red_mask_1);
    cv::inRange(hsv, lower_red_2, upper_red_2, red_mask_2);
    red_mask = red_mask_1 + red_mask_2;

    cv::Mat res;
    cv::bitwise_and(origin_bgr_img, origin_bgr_img, res, red_mask);

    cv::threshold(res, res, 20, 255, cv::THRESH_BINARY);
    cv::cvtColor(res, res, cv::COLOR_BGR2GRAY);

    int height = res.rows;
    int width = res.cols;
    float max_dist = 0;
    cv::Point2f point_to(0, 0);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int indexs = i * res.cols + j;
            int data = res.data[indexs];
            if (data != 0) {
                float dist = distance(cv::Point2f((float)j, (float)i), cv::Point2f((float)width / 2.0, (float)height / 2.0));
                if (dist > max_dist) {
                    point_to = cv::Point2f((float)j, (float)i);
                    max_dist = dist;
                }
            }
        }
    }

    return point_to;
}

int main()
{
    cv::Mat watermeter_ori = cv::imread("/workspaces/CppRPG/watermeter/R.jpeg");
    cv::Mat watermeter_dist;
    watermeter_dist = pre_process(watermeter_ori, cv::Size(800, 800));
    cv::resize(watermeter_ori, watermeter_ori, watermeter_dist.size());

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(watermeter_dist, circles, cv::HOUGH_GRADIENT, 1, 60, 200, 40, 40, 80);
    for (int i = 0; i < circles.size(); i++) {

        cv::Mat croped_meter(watermeter_ori, cv::Rect(circles[i][0] - circles[i][2], circles[i][1] - circles[i][2], circles[i][2] * 2, circles[i][2] * 2));
        cv::Point2f point_to = cv::Point2f(point_meter_process(croped_meter).x + (circles[i][0] - circles[i][2]),
            point_meter_process(croped_meter).y + (circles[i][1] - circles[i][2]));
        cv::line(watermeter_ori, cv::Point(circles[i][0], circles[i][1]), point_to, cv::Scalar(0, 0, 255), 3);

        cv::circle(watermeter_ori, cv::Point(circles[i][0], circles[i][1]), circles[i][2], cv::Scalar(20, 20, 255), 2);
    }

    cv::namedWindow("watermeter_result", 0);
    cv::resizeWindow("watermeter_result", cv::Size(400, 400));

    cv::imshow("watermeter_result", watermeter_ori);
    cv::waitKey(0);

    return 0;
}