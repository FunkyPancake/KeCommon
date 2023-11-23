/*
 * PID.h
 *
 *  Created on: Mar 10, 2019
 *      Author: Mati
 */

#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_
#include <type_traits>

namespace KeCommon::Utils
{
    struct PidConfig
    {
        float Kp, Ki, Kd, ItermLimitHigh, ItermLimitLow, OutputMax, OutputMin, ItermActivationThreshold, DeadBand;
    };

    class PidController
    {
        float _errorLast{0}, _iterm{0}, _target{0}, _lastOutput{0};
        PidConfig _config;

        static void Limit(float &value, const float &limitHigh, const float &limitLow)
        {
            if (value > limitHigh) {
                value = limitHigh;
            }
            if (value < limitLow) {
                value = limitLow;
            }
        }

    public:
        explicit PidController(const PidConfig &config)
            : _config{config}
        {
            // static_assert(std::is_floating_point_v<T> || std::is_integral_v<T>);
        }

        void SetTarget(const float &target)
        {
            _target = target;
        }

        float ControlLoop(const float &input)
        {
            const auto error = _target - input;
            if (error < _config.DeadBand) {
                return _lastOutput;
            }

            if (error < _config.ItermActivationThreshold) {
                _iterm += error;
                Limit(_iterm, _config.ItermLimitHigh, _config.ItermLimitLow);
            }
            else {
                _iterm = 0;
            }
            const auto errorDelta = error - _errorLast;
            auto output = error * _config.Kp + _iterm * _config.Ki + errorDelta * _config.Kd;
            Limit(output, _config.OutputMax, _config.OutputMin);
            _errorLast = error;
            _lastOutput = output;
            return output;
        }
    };
}

#endif /* PID_CONTROLLER_H_ */
