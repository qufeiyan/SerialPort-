#include "MahonyAHRS.h"
#include <QtCore/qmath.h>
#include <QDebug>

MahonyAHRS::MahonyAHRS()
{

}

MahonyAHRS::MahonyAHRS(float samplePeriod)
{
    MahonyAHRS(samplePeriod, 1.0f, 0.0f);
}

MahonyAHRS::MahonyAHRS(float samplePeriod, float kp)
{
    MahonyAHRS(samplePeriod, kp, 0.0f);
}

MahonyAHRS::MahonyAHRS(float samplePeriod, float kp, float ki)
{
    this->samplePeriod = samplePeriod;
    this->kp = kp;
    this->ki = ki;
    this->quaternion[0] = 1.0f;
    this->quaternion[1] = 0.0f;
    this->quaternion[2] = 0.0f;
    this->quaternion[3] = 0.0f;
    this->eInt[0] = 0.0f;
    this->eInt[1] = 0.0f;
    this->eInt[2] = 0.0f;
}

void MahonyAHRS::setSamplePeriod(float samplePeriod)
{
    this->samplePeriod = samplePeriod;
}

float MahonyAHRS::getSamplePeriod()
{
    return this->samplePeriod;
}

void MahonyAHRS::setKi(float ki)
{
    this->ki = ki;
}

void MahonyAHRS::setKp(float kp)
{
    this->kp = kp;
}

void MahonyAHRS::seteInt(float eInt[])
{
    this->eInt[0] = eInt[0];
    this->eInt[1] = eInt[1];
    this->eInt[2] = eInt[2];
}

float* MahonyAHRS::geteInt()
{
    return this->eInt;
}

float *MahonyAHRS::getQuaternion()
{
    return this->quaternion;
}

void MahonyAHRS::update(float gx, float gy, float gz, float ax, float ay, float az)
{
    float q1 = quaternion[0], q2 = quaternion[1], q3 = quaternion[2], q4 = quaternion[3];   // short name local variable for readability
    float norm;
    float vx, vy, vz;
    float ex, ey, ez;
    float pa, pb, pc;

    // Normalise accelerometer measurement
    norm = (float)sqrt(ax * ax + ay * ay + az * az);
    if (norm == 0.0f) return; // handle NaN
    norm = 1.0 / norm;        // use reciprocal for division
    ax *= norm;
    ay *= norm;
    az *= norm;

    // Estimated direction of gravity
    vx = 2.0f * (q2 * q4 - q1 * q3);
    vy = 2.0f * (q1 * q2 + q3 * q4);
    vz = q1 * q1 - q2 * q2 - q3 * q3 + q4 * q4;

    // Error is cross product between estimated direction and measured direction of gravity
    ex = (ay * vz - az * vy);
    ey = (az * vx - ax * vz);
    ez = (ax * vy - ay * vx);
    if (ki > 0.0f)
    {
      eInt[0] += ex;      // accumulate integral error
      eInt[1] += ey;
      eInt[2] += ez;
    }
    else
    {
      eInt[0] = 0.0f;     // prevent integral wind up
      eInt[1] = 0.0f;
      eInt[2] = 0.0f;
    }

    // Apply feedback terms
    gx = gx + kp * ex + ki * eInt[0];
    gy = gy + kp * ey + ki * eInt[1];
    gz = gz + kp * ez + ki * eInt[2];

    // Integrate rate of change of quaternion
    pa = q2;
    pb = q3;
    pc = q4;
    q1 = q1 + (-q2 * gx - q3 * gy - q4 * gz) * (0.5f * samplePeriod);
    q2 = pa + (q1 * gx + pb * gz - pc * gy) * (0.5f * samplePeriod);
    q3 = pb + (q1 * gy - pa * gz + pc * gx) * (0.5f * samplePeriod);
    q4 = pc + (q1 * gz + pa * gy - pb * gx) * (0.5f * samplePeriod);

    // Normalise quaternion
    norm = (float)sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);
    norm = 1.0f / norm;
    quaternion[0] = q1 * norm;
    quaternion[1] = q2 * norm;
    quaternion[2] = q3 * norm;
    quaternion[3] = q4 * norm;
}

void MahonyAHRS::update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz)
{
    float q1 = quaternion[0], q2 = quaternion[1], q3 = quaternion[2], q4 = quaternion[3];   // short name local variable for readability
          float norm;
          float hx, hy, bx, bz;
          float vx, vy, vz, wx, wy, wz;
          float ex, ey, ez;
          float pa, pb, pc;

          // Auxiliary variables to avoid repeated arithmetic
          float q1q1 = q1 * q1;
          float q1q2 = q1 * q2;
          float q1q3 = q1 * q3;
          float q1q4 = q1 * q4;
          float q2q2 = q2 * q2;
          float q2q3 = q2 * q3;
          float q2q4 = q2 * q4;
          float q3q3 = q3 * q3;
          float q3q4 = q3 * q4;
          float q4q4 = q4 * q4;

          // Normalise accelerometer measurement
          norm = (float)sqrt(ax * ax + ay * ay + az * az);
          if (norm == 0.0f) return; // handle NaN
          norm = 1.0 / norm;        // use reciprocal for division
          ax *= norm;
          ay *= norm;
          az *= norm;

          // Normalise magnetometer measurement
          norm = (float)sqrt(mx * mx + my * my + mz * mz);
          if (norm == 0.0f) return; // handle NaN
          norm = 1.0 / norm;        // use reciprocal for division
          mx *= norm;
          my *= norm;
          mz *= norm;

          // Reference direction of Earth's magnetic field
          hx = 2.0f * mx * (0.5f - q3q3 - q4q4) + 2.0f * my * (q2q3 - q1q4) + 2.0f * mz * (q2q4 + q1q3);
          hy = 2.0f * mx * (q2q3 + q1q4) + 2.0f * my * (0.5f - q2q2 - q4q4) + 2.0f * mz * (q3q4 - q1q2);
          bx = (float)sqrt((hx * hx) + (hy * hy));
          bz = 2.0f * mx * (q2q4 - q1q3) + 2.0f * my * (q3q4 + q1q2) + 2.0f * mz * (0.5f - q2q2 - q3q3);

          // Estimated direction of gravity and magnetic field
          vx = 2.0f * (q2q4 - q1q3);
          vy = 2.0f * (q1q2 + q3q4);
          vz = q1q1 - q2q2 - q3q3 + q4q4;
          wx = 2.0f * bx * (0.5f - q3q3 - q4q4) + 2.0f * bz * (q2q4 - q1q3);
          wy = 2.0f * bx * (q2q3 - q1q4) + 2.0f * bz * (q1q2 + q3q4);
          wz = 2.0f * bx * (q1q3 + q2q4) + 2.0f * bz * (0.5f - q2q2 - q3q3);

          // Error is cross product between estimated direction and measured direction of gravity
          ex = (ay * vz - az * vy) + (my * wz - mz * wy);
          ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
          ez = (ax * vy - ay * vx) + (mx * wy - my * wx);
          if (ki > 0.0f)
          {
              eInt[0] += ex;      // accumulate integral error
              eInt[1] += ey;
              eInt[2] += ez;
          }
          else
          {
              eInt[0] = 0.0f;     // prevent integral wind up
              eInt[1] = 0.0f;
              eInt[2] = 0.0f;
          }

          // Apply feedback terms
          gx = gx + kp * ex + ki * eInt[0];
          gy = gy + kp * ey + ki * eInt[1];
          gz = gz + kp * ez + ki * eInt[2];

          // Integrate rate of change of quaternion
          pa = q2;
          pb = q3;
          pc = q4;
          q1 = q1 + (-q2 * gx - q3 * gy - q4 * gz) * (0.5f * samplePeriod);
          q2 = pa + (q1 * gx + pb * gz - pc * gy) * (0.5f * samplePeriod);
          q3 = pb + (q1 * gy - pa * gz + pc * gx) * (0.5f * samplePeriod);
          q4 = pc + (q1 * gz + pa * gy - pb * gx) * (0.5f * samplePeriod);

          // Normalise quaternion
          norm = (float)sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);
          norm = 1.0f / norm;
          quaternion[0] = q1 * norm;
          quaternion[1] = q2 * norm;
          quaternion[2] = q3 * norm;
          quaternion[3] = q4 * norm;
}






