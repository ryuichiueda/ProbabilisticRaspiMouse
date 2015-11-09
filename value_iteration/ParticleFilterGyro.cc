#include <iostream>
#include <limits.h>
#include <cmath>
#include "Map.h"
#include "ParticleFilterGyro.h"
#include "SensorGyro.h"
using namespace std;

ParticleFilterGyro::ParticleFilterGyro(int num, ifstream *ifs,string mapfile) 
	: ParticleFilter(num,ifs,mapfile)
{
	m_distance_max_noise_ratio = 0.10; // 5% noise
	m_direction_max_noise_ratio = 0.001; // 0.1% noise
}

ParticleFilterGyro::~ParticleFilterGyro()
{
}

void ParticleFilterGyro::motionUpdate(double fw_delta_mm,double side_delta_mm,double t_delta_deg)
{
        double t_delta_rad = t_delta_deg * 3.141592 / 180;

        for(auto &p : m_particles){
                //double t_shift = 0.0;//change of direction due to foward action

                if(fw_delta_mm > 0.1){ //xy方向
                	double noise = 1.0 + m_distance_max_noise_ratio * getGaussRand();

                	double after_x = p.x_mm + fw_delta_mm * noise * cos(p.t_rad)
				+ side_delta_mm * noise * sin(p.t_rad);
                	double after_y = p.y_mm + fw_delta_mm * noise * sin(p.t_rad)
				+ side_delta_mm * noise * cos(p.t_rad);


                        bool col[4];
                        m_map->collision(p.x_mm,p.y_mm,after_x,after_y,col);

                        if(col[1] || col[2]){
                       //         p.t_rad += (getDoubleRand() - 0.5)*3.141592;
                        }else{
                                p.x_mm = after_x;
                        }

                        if(col[0] || col[3]){
                        //        p.t_rad += (getDoubleRand() - 0.5)*3.141592;
                        }else{
                                p.y_mm = after_y;
                        }
                }

                //theta方向
                double noise = 1.0 + m_direction_max_noise_ratio * getGaussRand();
                p.t_rad += t_delta_rad * noise;
                normalizeTheta(&(p.t_rad));
        }
        m_step++;
}

