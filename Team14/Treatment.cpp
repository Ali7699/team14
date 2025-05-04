#pragma once
#include "Treatment.h"
#include "ETherapy.h"
#include "UTherapy.h"
#include "XTherapy.h"
#include "Scheduler.h"
#include"Resource.h"
#include "Patient.h"

Treatment* Treatment::createTreatment(char type, int duration) {
    switch (type) {
    case 'X':
        return new XTherapy(duration);
    case 'E':
        return new ETherapy(duration);
    case 'U':
        return new UTherapy(duration);
    default:
        return nullptr; // Invalid type
    }
}