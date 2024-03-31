#include "JVisLib/JVisStatus.h"

JVisStatus::JVisStatus()
{

}

JVisStatus::~JVisStatus()
{

}

bool JVisStatus::init()
{
    m_simInit = false;
    m_calibInit = false;
    m_recInit = false;

    return true;
}
