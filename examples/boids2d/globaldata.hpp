#ifndef GLOBALDATA_HPP_
#define GLOBALDATA_HPP_

struct GlobalData{
    float margin{0.15f};
    float correction{0.2f};
    float speed{0.30f};
    float viewportWidth;
    float viewportHeight;
    bool m_removePredator{true};
};

#endif