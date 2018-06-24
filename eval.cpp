#include "stdlib.h"
#include <algorithm>
#include "sailshade.hpp"

/**
 ** Evaluate all the population
 */
void SailShade::eval_all()
{
    double coeff_shadow = rate_shad;
    double coeff_area = 1 - rate_shad;
    
    Triangle sail;
    this -> m_average_fitness = 0.;
    int j(0);
    for(unsigned int i(0); i < m_size*m_genome_size; i+=9) {
        sail.p1.x = m_pop[i];
        sail.p1.y = m_pop[i+1];
        sail.p1.z = m_pop[i+2];
        
        sail.p2.x = m_pop[i+3];
        sail.p2.y = m_pop[i+4];
        sail.p2.z = m_pop[i+5];
        
        sail.p3.x = m_pop[i+6];
        sail.p3.y = m_pop[i+7];
        sail.p3.z = m_pop[i+8];
        
        m_shadow[i/9] = 0;
        for (int j(0); j < m_nb_azimuth*3; j+=3) {
            double director_ray[3] = {m_director_ray[(int)j], m_director_ray[(int)j+1], m_director_ray[(int)j+2]};
            m_shadow[i/9] += getCoverPercentage(sail, director_ray, m_floor);
        }
        m_area[i/9] = this->area(sail);
        (m_area[i/9] > m_max_area) ? m_max_area = m_area[i/9] : 0;
        m_shadow[i/9] = m_shadow[i/9] / m_nb_azimuth;
        m_fitness[i/9] = coeff_shadow * m_shadow[i/9] + coeff_area * (1-(m_area[i/9] / (m_max_area == 0 ? 1 : m_max_area)));
        m_average_fitness += m_fitness[i/9];
        ++j;
    }
    m_average_fitness /= j;
}

/**
 ** Evaluate the individual ind where ind is the first coordinate of the first point
 */
float SailShade::eval_one(float *ind)
{    
    double coeff_shadow = rate_shad;
    double coeff_area = 1 - rate_shad;
    
    float shadow = 0;
    float area = 0;
    float max_area = 0;

    Triangle sail;
    sail.p1.x = ind[0];
    sail.p1.y = ind[1];
    sail.p1.z = ind[2];
    
    sail.p2.x = ind[3];
    sail.p2.y = ind[4];
    sail.p2.z = ind[5];
    
    sail.p3.x = ind[6];
    sail.p3.y = ind[7];
    sail.p3.z = ind[8];
    
    for (int j(0); j < m_nb_azimuth*3; j+=3) {
        double director_ray[3] = {m_director_ray[(int)j], m_director_ray[(int)j+1], m_director_ray[(int)j+2]};
        shadow += getCoverPercentage(sail, director_ray, m_floor);
    }
    shadow = shadow / m_nb_azimuth;
    area = this->area(sail);
    max_area = (area > m_max_area) ? area : m_max_area;
    return coeff_shadow * shadow + coeff_area * (1-(area / (max_area == 0 ? 1 : max_area)));
}

/*
** Calcul and return the percentage of shadow made by the sail (with ONE director vector of the sun) on the choosen "floor" (represents by n points)
*/
float SailShade::getCoverPercentage(Triangle sail, double *director, Rectangle floor) const
{
    Triangle projection = getProjectTriangle(sail, director);
    Point point;
    float amount = .0;
    float i = .0;
    
    Point center;
    center.x = (floor.p1.x + floor.p2.x)/2;
    center.y = (floor.p1.y + floor.p4.y)/2;
    float distanceFromCenter;
    float distanceMax = sqrt(((center.y - floor.p1.y) * (center.y - floor.p1.y)) + ((center.x - floor.p1.x) * (center.x - floor.p1.x)));
    
    for(point.y = floor.p1.y; point.y <= floor.p4.y; point.y += m_step) {
        for(point.x = floor.p1.x; point.x <= floor.p2.x; point.x += m_step) {
            distanceFromCenter = sqrt(((center.y - point.y) * (center.y - point.y)) + ((center.x - point.x) * (center.x - point.x)));
            i += 1-(distanceFromCenter/distanceMax);
            isInside(point, projection) ? amount += 1-(distanceFromCenter/distanceMax) : 0;
        }
    }

    return (float)amount/i;
}

/**
 ** Compute the sail projection on the floor where z=0 for 1 director vector given
 */
SailShade::Triangle SailShade::getProjectTriangle(Triangle sail, double *director) const
{
    Triangle projection;
    projection.p1.x = ( -director[0] * sail.p1.z / -director[2] ) + sail.p1.x;
    projection.p1.y = ( -director[1] * sail.p1.z / -director[2] ) + sail.p1.y;
    projection.p1.z = 0;
    
    projection.p2.x = ( -director[0] * sail.p2.z / -director[2] ) + sail.p2.x;
    projection.p2.y = ( -director[1] * sail.p2.z / -director[2] ) + sail.p2.y;
    projection.p2.z = 0;
    
    projection.p3.x = ( -director[0] * sail.p3.z / -director[2] ) + sail.p3.x;
    projection.p3.y = ( -director[1] * sail.p3.z / -director[2] ) + sail.p3.y;
    projection.p3.z = 0;

    return projection;
}

/*
** Check if a point is inside the triangle
*/
bool SailShade::isInside(Point p, Triangle projection) const
{
    double area_triangle = area(projection);
   
    float area_point1_triangle = area(p.x, p.y, projection.p2.x, projection.p2.y, projection.p3.x, projection.p3.y);
    float area_point2_triangle = area(projection.p1.x, projection.p1.y, p.x, p.y, projection.p3.x, projection.p3.y);
    float area_point3_triangle = area(projection.p1.x, projection.p1.y, projection.p2.x, projection.p2.y, p.x, p.y);
    
    double sum_areas = area_point1_triangle + area_point2_triangle + area_point3_triangle;
    
//    std::cout << "Area sail projection :" << area_triangle << std::endl;
//    std::cout << "Area point1 :" << area_point1_triangle << std::endl;
//    std::cout << "Area point2 :" << area_point2_triangle << std::endl;
//    std::cout << "Area point3 :" << area_point3_triangle << std::endl;
//    std::cout << "Area point total :" << sum_areas << std::endl;
    if(round(area_triangle*100)/100 == 0)
        return false;
    
    return round(area_triangle*100)/100 == round(sum_areas*100)/100;
}

/*
** Calcul area of triangle
*/
float SailShade::area(Triangle projection) const
{
    float long_p1_p2 = sqrt(((projection.p2.x - projection.p1.x) * (projection.p2.x - projection.p1.x)) + ((projection.p2.y - projection.p1.y) * (projection.p2.y - projection.p1.y)) + ((projection.p2.z - projection.p1.z) * (projection.p2.z - projection.p1.z)));
    float long_p3_p2 = sqrt(((projection.p3.x - projection.p2.x) * (projection.p3.x - projection.p2.x)) + ((projection.p3.y - projection.p2.y) * (projection.p3.y - projection.p2.y)) + ((projection.p3.z - projection.p2.z) * (projection.p3.z - projection.p2.z)));
    float long_p3_p1 = sqrt(((projection.p3.x - projection.p1.x) * (projection.p3.x - projection.p1.x)) + ((projection.p3.y - projection.p1.y) * (projection.p3.y - projection.p1.y)) + ((projection.p3.z - projection.p1.z) * (projection.p3.z - projection.p1.z)));
    float p = long_p1_p2 + long_p3_p2 + long_p3_p1;
    float s = p/2.0;

    float area = sqrt(s * (s - long_p1_p2) * (s - long_p3_p2) * (s - long_p3_p1));
    
    return area;
}

/*
 ** Calcul area of triangle by giving 3 points
 */
float SailShade::area(float x1, float y1, float x2, float y2, float x3, float y3) const
{
    Triangle triangle;
    triangle.p1.x = x1;
    triangle.p1.y = y1;
    triangle.p1.z = 0;
    triangle.p2.x = x2;
    triangle.p2.y = y2;
    triangle.p2.z = 0;
    triangle.p3.x = x3;
    triangle.p3.y = y3;
    triangle.p3.z = 0;

    return this->area(triangle);
}
