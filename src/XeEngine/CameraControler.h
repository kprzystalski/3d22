//
// Created by Piotr Białas on 12/11/2021.
//

#pragma once

#include "Camera.h"

namespace xe {
    class CameraControler {
    public:

        CameraControler(Camera *camera) : camera_(camera), scale_(0.025), LMB_pressed_(false) {}

        CameraControler() : CameraControler(nullptr) {}


        void set_camera(Camera *camera) { camera_ = camera; }


        void rotate_camera(float dx, float dy) {
            camera_->rotate_around_center(-scale_ * dy, camera_->x());
            camera_->rotate_around_center(-scale_ * dx, camera_->y());
        }

        void mouse_moved(float x, float y) {
            if (LMB_pressed_) {
                auto dx = x - x_;
                auto dy = y - y_;
                x_ = x;
                y_ = y;

                rotate_camera(dx, dy);
            }
        };

        void LMB_pressed(float x, float y) {
            LMB_pressed_ = true;
            x_ = x;
            y_ = y;
        };

        void LMB_released(float x, float y) {
            LMB_pressed_ = false;
            auto dx = x - x_;
            auto dy = y - y_;

            rotate_camera(dx, dy);
        };

    private:
        float scale_;
        Camera *camera_;
        bool LMB_pressed_;
        float x_, y_;

    };

}


