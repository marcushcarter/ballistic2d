
#pragma once
#include <core/application/application.h>

namespace ballistic2d {

struct GameApplication : Application
{
    Error on_init() override;
    void on_update(float p_dt) override;
    void on_shutdown() override;
};

}