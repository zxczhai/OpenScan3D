#ifndef _OPENSCANCAMERA_H
#define _OPENSCANCAMERA_H

#include <string>

class CustomCamera {
public:
    CustomCamera() : m_modelName("Default Model"), m_sensorSize(36.0), m_focal(50.0) {}
    CustomCamera(const std::string& modelName, double sensorSize = 36.0, double focal = 50.0)
        : m_modelName(modelName), m_sensorSize(sensorSize), m_focal(focal) {}

    const std::string& getModelName() const { return m_modelName; }
    double getSensorSize() const { return m_sensorSize; }
    double getFocal() const { return m_focal; }

    void setModelName(const std::string& modelName) { m_modelName = modelName; }
    void setSensorSize(double sensorSize) { m_sensorSize = sensorSize; }
    void setFocal(double focal) { m_focal = focal; }

private:
    std::string m_modelName;
    double m_sensorSize;
    double m_focal;
};

#endif
