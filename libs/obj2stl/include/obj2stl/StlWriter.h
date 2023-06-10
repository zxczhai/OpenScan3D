#pragma once

#include "StlModel.h"

#include <ostream>

class StlWriter
{
public:
    StlWriter(const StlModel& model) : model_(model) {}

    void WriteToFile(const std::string& fname) const;
    void WriteToStream(std::ostream& os) const;

private:
    const StlModel& model_;

    void WriteHeader(std::ostream& os) const;
    void WriteBody(std::ostream& os) const;
};
