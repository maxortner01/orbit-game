#pragma once

//#include <numbers>

namespace Math
{
    struct Angle
    {
        template<typename T>
        constexpr static Angle radians(const T& r)
        {
            return Angle(static_cast<double>(r));
        }

        template<typename T>
        constexpr static Angle degrees(const T& d)
        {
            return Angle(toRadians(static_cast<double>(d)));
        }

        constexpr double asRadians() const { return rads; }
        constexpr double asDegrees() const { return toDegrees(rads); }

        constexpr Angle operator+(const Angle& b) const
        {
            return Angle(rads + b.rads);
        }

        Angle& operator+=(const Angle& b) 
        {
            rads += b.rads;
            return *this;
        }

        Angle& operator-=(const Angle& b) 
        {
            rads -= b.rads;
            return *this;
        }

        constexpr Angle operator-(const Angle& b) const
        {
            return Angle(rads + b.rads);
        }

        constexpr Angle operator*(const Angle& b) const
        {
            return Angle(rads * b.rads);
        }

        constexpr Angle operator/(const Angle& b) const
        {
            return Angle(rads / b.rads);
        }

        template<typename T>
        constexpr Angle operator*(const T& b) const
        {
            return Angle(rads * b);
        }

        template<typename T>
        constexpr Angle operator/(const T& b) const
        {
            return Angle(rads / b);
        }

        constexpr bool operator<(const Angle& b) const
        {
            return rads < b.rads;
        }

        static Angle PI;

        constexpr Angle() : rads{0}
        {   }

    private:
        constexpr Angle(double radians) : rads(radians) 
        {   }

        constexpr static double toRadians(const double& d)
        {
            //return d * std::numbers::pi / 180.0;
            return d * 3.14159 / 180.0;
        }

        constexpr static double toDegrees(const double& r)
        {
            //return r * 180.0 / std::numbers::pi;
            return r * 180.0 / 3.14159;
        }

        double rads;
    };
}