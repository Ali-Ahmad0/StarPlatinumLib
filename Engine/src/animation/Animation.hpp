#pragma once
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <stdio.h>

enum AnimationType
{
    ONE_TIME,
    LOOP,
    FORWARD_BACKWARD
};

template <typename T>
class Animation
{
public:
    Animation()
        : m_propertyReference(nullptr), m_duration(0.0), m_currentTime(0.0), 
            m_animationType(ONE_TIME), m_finished(true), m_reversing(false) {}
    
    Animation(T* property, double duration, AnimationType type = ONE_TIME) 
        : m_currentTime(0.0), m_animationType(type), m_finished(false), m_reversing(false)
    {
        m_propertyReference = nullptr;
        m_duration = 0.0;

        if (!property)
        {
            fprintf(stderr, "[ERROR]: Cannot create animation on NULL property reference\n");
            return;
        }
        m_propertyReference = property;

        if (duration <= 0.0)
        {
            fprintf(stderr, "[ERROR]: Duration of animation should be greater than zero\n");
            return;
        }
        m_duration = duration;
    }

    size_t AddKeyFrame(double timeframe, const T& value)
    {
        if (timeframe < 0.0 || timeframe > m_duration)
        {
            fprintf(stderr, "[ERROR]: Timeframe must be within 0.0 and %.2lf seconds\n", m_duration);
            return std::numeric_limits<size_t>::max();
        }

        // Add keyframe
        m_keyframes.emplace_back(timeframe, value);

        // Sort by time 
        std::sort(m_keyframes.begin(), m_keyframes.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });

        return m_keyframes.size() - 1;
    }

    void Play(double delta)
    {
        if (m_finished || !m_propertyReference || m_keyframes.empty())
        {
            return;
        }

        // Update time
        if (m_animationType == FORWARD_BACKWARD && m_reversing)
        {
            m_currentTime -= delta;
        }
        else
        {
            m_currentTime += delta;
        }

        // Handle animation types
        switch (m_animationType)
        {
            // Finish animation once completed
        case ONE_TIME:
            if (m_currentTime >= m_duration)
            {
                m_currentTime = m_duration;
                m_finished = true;
            }
            updateProperty(m_currentTime);
            break;

            // Reset animation once completed
        case LOOP:
            if (m_currentTime >= m_duration)
            {
                m_currentTime = fmod(m_currentTime, m_duration);
            }
            updateProperty(m_currentTime);
            break;

            // Play in reverse direction once completed
        case FORWARD_BACKWARD:
            if (m_reversing)
            {
                if (m_currentTime <= 0.0)
                {
                    m_currentTime = 0.0;
                    m_reversing = false;
                }
            }
            else
            {
                if (m_currentTime >= m_duration)
                {
                    m_currentTime = m_duration;
                    m_reversing = true;
                }
            }
            updateProperty(m_currentTime);
            break;
        }
    }
    
    void Reset()
    {
        m_currentTime = 0.0;
        m_finished = false;
        m_reversing = false;
    }
    
    bool IsFinished() { return m_finished; }

private:
    T* m_propertyReference;
    double m_duration;
    double m_currentTime;

    AnimationType m_animationType;
    
    bool m_finished;
    bool m_reversing;
    
    std::vector<std::pair<double, T>> m_keyframes{};

    // Floating point interpolation
    template<typename U = T>
    typename std::enable_if_t<std::is_floating_point_v<U>, U>
        interpolate(const U& a, const U& b, double t)
    {
        return a + (b - a) * t;
    }

    // Integer types interpolation
    template<typename U = T>
    typename std::enable_if_t<std::is_integral_v<U>, U>
        interpolate(const U& a, const U& b, double t)
    {
        return static_cast<U>(a + (b - a) * t + 0.5);
    }

    // Discrete types interpolation
    template<typename U = T>
    typename std::enable_if_t<!std::is_arithmetic_v<U>, U>
        interpolate(const U& a, const U& b, double t)
    {
        return a;
    }

    void updateProperty(double time)
    {
        if (m_keyframes.empty()) 
        { 
            return; 
        }

        time = std::max(0.0, std::min(time, m_duration));

        // Find the two keyframes to interpolate between
        size_t nextIndex = 0;
        for (size_t i = 0; i < m_keyframes.size(); i++) 
        {
            if (m_keyframes[i].first > time) 
            {
                nextIndex = i;
                break;
            }
            nextIndex = i + 1;
        }

        // Return if still before first keyframe
        if (nextIndex == 0) 
        {
            return;
        }

        // Set final value if after last keyframe
        if (nextIndex >= m_keyframes.size()) 
        {
            *m_propertyReference = m_keyframes.back().second;
            return;
        }

        // Cannot interpolate if only one frame
        if (m_keyframes.size() <= 1) 
        {
            return;
        }

        // Interpolate between two keyframes
        const auto& prevFrame = m_keyframes[nextIndex - 1];
        const auto& nextFrame = m_keyframes[nextIndex];

        double timeDiff = nextFrame.first - prevFrame.first;
        if (timeDiff > 0.0) 
        {
            double t = (time - prevFrame.first) / timeDiff;
            *m_propertyReference = interpolate(prevFrame.second, nextFrame.second, t);
        }
        else 
        {
            *m_propertyReference = nextFrame.second;
        }
    }
};