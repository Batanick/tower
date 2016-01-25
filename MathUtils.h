#pragma once

//
// Created by botanick on 25.01.16.
//

template<class T>
T trim(T min, T max, T value) {
    if (value < min)
        return min;

    return value > max ? max : value;
}

