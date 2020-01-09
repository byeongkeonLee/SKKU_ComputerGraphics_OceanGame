#pragma once
#ifndef __TRACKBALL_H__
#define __TRACKBALL_H__
#include "cgmath.h"

vec2 cur;

struct camera
{
	vec3	set_eye = vec3(80, 0, 160);
	vec3	set_at = vec3(0, 0, 0);
	vec3	set_up = vec3(0, 0, 1);
	float	move_spd = 0.04f;
	vec3	eye = vec3(80, 0, 160);
	vec3	at = vec3(0, 0, 0);
	vec3	up = vec3(0, 0, 1);
	mat4	view_matrix = mat4::look_at(eye, at, up);

	float	fovy = PI / 4.0f; // must be in radian
	float	aspect_ratio;
	float	dNear = 1.0f;
	float	dFar = 5000.0f;
	mat4	projection_matrix;

	bool	radial_blur=false;
	float	radial_intensity = 0.0f;
};
camera cam;
struct light
{
	vec3	position = vec3(0, 0, 1000);
	float	intensity = 100.0f;
};
light sun;

struct trackball
{
	bool	b_tracking = false;
	float	scale;			// controls how much rotation is applied
	mat4	view_matrix0;	// initial view matrix
	vec2	m0;				// the last mouse position
	int button;
	int mods;
	vec3 eye, at, up;
	vec3 *eyep, *atp, *upp;
	vec3 new_eye, new_at, new_up;

	trackball( float rot_scale=1.0f ):scale(rot_scale){}
	bool is_tracking() const { return b_tracking; }
	void begin( const mat4& view_matrix, float x, float y, int _button ,int _mods)
	{
		b_tracking = true;			// enable trackball tracking
		m0 = vec2(x,y)*2.0f-1.0f;	// convert (x,y) in [0,1] to [-1,1]
		view_matrix0 = view_matrix;	// save current view matrix
		button = _button;
		mods = _mods;
		eye= cam.eye;
		at= cam.at;
		up= cam.up;
	}
	void end() { 
		b_tracking = false;
	}

	mat4 update_trackball( float x, float y )
	{
		// retrive the current mouse position
		vec2 m = vec2(x,y)*2.0f - 1.0f; // normalize xy
		//m0 = vec2(1.0f, 1.0f);

		//eye = cam.eye;
		//at = cam.at;
		//up = cam.up;

		// project a 2D mouse position to a unit sphere
		static const vec3 p0 = vec3(0, 0, 1.0f);//vec3(at.x,at.y,1.0f);	// reference position on sphere
		vec3 p1 = vec3(m.x-m0.x, m0.y-m.y,0);	// displacement with vertical swap
		if(!b_tracking||length(p1)<0.0001f) return view_matrix0;			// ignore subtle movement
		p1 *= scale;														// apply rotation scale
		p1 = vec3(p1.x,p1.y,sqrtf(max(0,1.0f-length2(p1)))).normalize();	// back-project z=0 onto the unit sphere

		vec3 n = p0.cross(p1)*mat3(cam.view_matrix);

		float angle = asin(min(n.length(), 0.999f));// *0.01f;
		
		cam.eye = eye * mat4::rotate(n.normalize(), angle);
		cam.up = up * mat4::rotate(n.normalize(), angle);
		return mat4::look_at(cam.eye,cam.at,cam.up);
	}
	mat4 update_zoom(float x, float y)
	{
		vec2 m = vec2(x, y)*2.0f - 1.0f; // normalize xy
		vec3 p2 = vec3(0, m0.y - m.y, 0);	// displacement with vertical swap
		if (!b_tracking || length(p2) < 0.0001f) return view_matrix0;			// ignore subtle movement
		p2.y *= 0.5f;
		if (p2.y < -0.99f) p2.y =-0.99f;
		cam.eye = vec3(eye.x+ (eye.x-at.x) * p2.y, eye.y + (eye.y-at.y)* p2.y, eye.z + (eye.z-at.z)*p2.y);

		return mat4::look_at(cam.eye, cam.at, cam.up);
	}
	mat4 update_panning(float x, float y)
	{
		vec2 m = vec2(x, y)*2.0f - 1.0f; 
		vec3 p1 = vec3(m.x - m0.x, m0.y - m.y, 0);	
		if (!b_tracking || length(p1) < 0.0001f) return view_matrix0;			
		p1 *= scale*(eye-at).length();

		vec4 p2 = view_matrix0.inverse()*vec4(p1,0);
		vec3 p3 = vec3(p2.x, p2.y, p2.z);
		cam.eye = eye - p3;
		cam.at = at - p3;

		return mat4::look_at(cam.eye, cam.at, cam.up);
	}
};
#endif // __TRACKBALL_H__

