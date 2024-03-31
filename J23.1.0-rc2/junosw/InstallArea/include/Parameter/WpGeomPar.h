#ifndef JUNO_WP_GEO_PAR_H
#define JUNO_WP_GEO_PAR_H

#include <iostream>

class WpGeomPar
{
    public:

        WpGeomPar() { std::cout << "WpGeomPar constructor" << std::endl; }
        ~WpGeomPar() { std::cout << "WpGeomPar destructor" << std::endl; }

	double GetWpHeight() { return m_WpHeight; }
        double GetWpRadius() { return m_WpRadius; }

	void InitWpVariables();

    private:

	double m_WpHeight;
	double m_WpRadius;

	void SetWpHeight(double WpHeight) { m_WpHeight = WpHeight; }
	void SetWpRadius(double WpRadius) { m_WpRadius = WpRadius; }

};

#endif
